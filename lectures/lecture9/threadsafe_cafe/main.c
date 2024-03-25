#include <stdio.h>
#include <threads.h>
#include <assert.h>
#include <time.h>

enum beverage {
	beverage_espresso,
	beverage_cappuccino,
	beverage_tea,
	beverage_cocoa,
};

struct order {
	char customer_name[20];
	enum beverage beverage;
};

enum order_queue_status {
	order_queue_status_empty,
	order_queue_status_busy,
	order_queue_status_full,
	order_queue_status_closed,
};

struct order_queue {
	int head;
	int tail;
	enum order_queue_status status;
	struct order items[100];
	int capacity;
	mtx_t mutex;
	cnd_t changed;
};

enum order_queue_result {
	order_queue_success,
	order_queue_timedout,
	order_queue_closed,
};

struct order_queue order_queue_new()
{
	struct order_queue q = {
		.head = 0,
		.tail = 0,
		.status = order_queue_status_empty,
		.items = {},
		.capacity = sizeof(q.items) / sizeof(q.items[0]),
	};

	int mtx_init_result = mtx_init(&q.mutex, mtx_recursive);
	assert(mtx_init_result == thrd_success);

	int cnd_init_result = cnd_init(&q.changed);
	assert(cnd_init_result == thrd_success);

	return q;
}

void order_queue_close(struct order_queue* q)
{
	mtx_lock(&q->mutex);
	q->status = order_queue_status_closed;
	cnd_signal(&q->changed);
	mtx_unlock(&q->mutex);
}

void order_queue_destroy(struct order_queue* q)
{
	cnd_destroy(&q->changed);
	mtx_destroy(&q->mutex);
}

enum order_queue_result order_queue_push(
	struct order_queue* q,
	struct order* o)
{
	enum order_queue_result result;
	mtx_lock(&q->mutex);
	switch (q->status) {
		case order_queue_status_closed:
			result = order_queue_closed;
			break;
		case order_queue_status_empty:
		case order_queue_status_busy:
			q->items[q->head] = *o;
			q->head = (q->head + 1) % q->capacity;
			if (q->head == q->tail)
				q->status = order_queue_status_full;
			else
				q->status = order_queue_status_busy;
			cnd_signal(&q->changed);
			result = order_queue_success;
			break;
		case order_queue_status_full:
			assert(0);
		default:
			assert(0);
	}
	mtx_unlock(&q->mutex);
	return result;
}

enum order_queue_result order_queue_trypop(
	struct order_queue* q,
	struct order* order,
	time_t seconds)
{
	enum order_queue_result result;
	mtx_lock(&q->mutex);

	switch (q->status) {
	case order_queue_status_closed:
		result = order_queue_closed;
		break;
	case order_queue_status_empty:
		if (seconds > 0) {
			struct timespec ts;
			int timespec_get_result = timespec_get(
					&ts,
					TIME_UTC
				);
			assert(timespec_get_result == TIME_UTC);
			ts.tv_sec += seconds;

			cnd_timedwait(
				&q->changed,
				&q->mutex,
				&ts
			);
			result = order_queue_trypop(q, order, 0);
		} else {
			result = order_queue_timedout;
		}
		break;
	case order_queue_status_busy:
	case order_queue_status_full:
		*order = q->items[q->tail];
		q->tail = (q->tail + 1) % q->capacity;
		if (q->head == q->tail)
			q->status = order_queue_status_empty;
		else
			q->status = order_queue_status_busy;
		result = order_queue_success;
		break;
	default:
		assert(0);
	}
	mtx_unlock(&q->mutex);
	return result;
}

const char* beverage_str(enum beverage b)
{
	switch (b) {
	case beverage_espresso:
		return "espresso";
	case beverage_cappuccino:
		return "cappuccino";
	case beverage_tea:
		return "tea";
	case beverage_cocoa:
		return "cocoa";
	}
	assert(0);
}

void do_work(int seconds)
{
	thrd_sleep(&(struct timespec) { .tv_sec = seconds }, NULL);
}

struct cashier_thread_params {
	const char* name;
	struct order_queue* in_queue;
	struct order_queue* out_queue;
};

int cashier_thread_func(void* arg)
{
	struct cashier_thread_params* p = arg;
	printf("Cashier %s arrived.\n", p->name);
	int queue_open = 1;
	while (queue_open) {
		struct order o;
		enum order_queue_result pop_result = order_queue_trypop(
				p->in_queue,
				&o,
				5
			);

		switch (pop_result) {
		case order_queue_success:
			printf(
				"Cashier %s is serving %s with %s.\n",
				p->name,
				o.customer_name,
				beverage_str(o.beverage)
			);
			do_work(5);
			enum order_queue_result push_result = order_queue_push(
					p->out_queue,
					&o
				);
			if (push_result == order_queue_closed) {
				queue_open = 0;
				break;
			}
			printf(
				"Cashier %s has completed the order.\n",
				p->name
			);
			break;
		case order_queue_timedout:
			printf("Cashier %s has nothing to do.\n", p->name);
			break;
		case order_queue_closed:
			queue_open = 0;
			break;
		default:
			assert(0);
		}
	}
	printf("Cashier %s went home.\n", p->name);
	return 0;
}

struct barista_thread_params {
	const char* name;
	struct order_queue* in_queue;
};

int barista_thread_func(void* arg)
{
	struct barista_thread_params* p = arg;
	printf("Barista %s arrived.\n", p->name);
	int queue_open = 1;
	while (queue_open) {
		struct order o;
		enum order_queue_result pop_result = order_queue_trypop(
				p->in_queue,
				&o,
				5
			);
		switch (pop_result) {
		case order_queue_success:
			printf(
				"Barista %s is serving %s with %s.\n",
				p->name,
				o.customer_name,
				beverage_str(o.beverage)
			);
			switch (o.beverage) {
			case beverage_espresso:
				do_work(3);
				break;
			case beverage_cappuccino:
				do_work(5);
				break;
			case beverage_tea:
				do_work(2);
				break;
			case beverage_cocoa:
				do_work(4);
				break;
			default:
				assert(0);
			}
			printf(
				"Barista %s has completed the order.\n",
				p->name
			);
			break;
		case order_queue_timedout:
			printf("Barista %s has nothing to do.\n", p->name);
			break;
		case order_queue_closed:
			queue_open = 0;
			break;
		default:
			assert(0);
		}
	}
	printf("Barista %s went home.\n", p->name);
	return 0;
}

void push_customer_order(struct order_queue* q, struct order* o) {
	enum order_queue_result push_result;
	push_result = order_queue_push(q, o);
	assert(push_result == order_queue_success);
	printf(
		"Customer %s asked for %s.\n",
		o->customer_name,
		beverage_str(o->beverage)
	);
}

void input_loop(struct order_queue* out_queue)
{
	int cafe_opened = 1;
	while (cafe_opened) {
		char command;
		int scanf_command_result = scanf("%c", &command);
		assert(scanf_command_result == 1);
		if (command == 'q') {
			cafe_opened = 0;
		}
		else if (
			command == 'e'
			|| command == 'c'
			|| command == 't'
			|| command == 'o'
		) {
			struct order o;
			switch (command) {
			case 'e':
				o.beverage = beverage_espresso;
				break;
			case 'c':
				o.beverage = beverage_cappuccino;
				break;
			case 't':
				o.beverage = beverage_tea;
				break;
			case 'o':
				o.beverage = beverage_cocoa;
				break;
			default:
				assert(0);
			}
			int scanf_name_result = scanf(
					"%20s",
					o.customer_name
				);
			assert(scanf_name_result == 1);
			push_customer_order(out_queue, &o);
		}
		else if (command == 's') {
			for (int i = 0; i < 10; ++i) {
				struct order o;
				o.beverage = beverage_cocoa;
				sprintf(o.customer_name, "Kid #%d", i);
				push_customer_order(out_queue, &o);
			}
		}
		else {
			printf("Unknown command %c.\n", command);
		}
		while (getchar() != '\n') {
			;
		}
	}
}

int main()
{
	printf("Cafe opened.\n");
	printf("Type <beverage> <customer_name> to place a new order.\n");
	printf("  Where <beverage> is:\n");
	printf("     e - espresso\n");
	printf("     c - cappuccino\n");
	printf("     t - tea\n");
	printf("     o - cocoa\n");
	printf("Type s for a stress test with 10 orders.\n");
	printf("Type q to exit.\n");

	struct order_queue cashier_queue = order_queue_new();
	struct order_queue barista_queue = order_queue_new();

	struct cashier_thread_params cashier_carol_thread_params = {
			.name = "Carol",
			.in_queue = &cashier_queue,
			.out_queue = &barista_queue,
		};
	struct cashier_thread_params cashier_chris_thread_params = {
			.name = "Chris",
			.in_queue = &cashier_queue,
			.out_queue = &barista_queue,
		};
	struct barista_thread_params barista_bob_thread_params = {
			.name = "Bob",
			.in_queue = &barista_queue,
		};
	struct barista_thread_params barista_bill_thread_params = {
			.name = "Bill",
			.in_queue = &barista_queue,
		};
	thrd_t cashier_carol_thread;
	thrd_t cashier_chris_thread;
	thrd_t barista_bob_thread;
	thrd_t barista_bill_thread;
	int thrd_create_result;

	thrd_create_result = thrd_create(
			&cashier_carol_thread,
			cashier_thread_func,
			&cashier_carol_thread_params
		);
	assert(thrd_create_result == thrd_success);

	thrd_create_result = thrd_create(
			&cashier_chris_thread,
			cashier_thread_func,
			&cashier_chris_thread_params
		);
	assert(thrd_create_result == thrd_success);

	thrd_create_result = thrd_create(
			&barista_bob_thread,
			barista_thread_func,
			&barista_bob_thread_params
		);
	assert(thrd_create_result == thrd_success);

	thrd_create_result = thrd_create(
			&barista_bill_thread,
			barista_thread_func,
			&barista_bill_thread_params
		);
	assert(thrd_create_result == thrd_success);

	input_loop(&cashier_queue);

	printf("Cafe is closing.\n");

	order_queue_close(&cashier_queue);
	order_queue_close(&barista_queue);

	thrd_join(cashier_carol_thread, NULL);
	thrd_join(cashier_chris_thread, NULL);
	thrd_join(barista_bob_thread, NULL);
	thrd_join(barista_bill_thread, NULL);

	order_queue_destroy(&cashier_queue);
	order_queue_destroy(&barista_queue);

	printf("Cafe closed.\n");
	return 0;
}
