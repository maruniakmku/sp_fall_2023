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
	order_queue_empty,
	order_queue_busy,
	order_queue_full,
};

struct order_queue {
	int head;
	int tail;
	enum order_queue_status status;
	struct order items[100];
};

struct order_queue order_queue_new()
{
	return (struct order_queue) {
		.head = 0,
		.tail = 0,
		.status = order_queue_empty,
		.items = {},
	};
}

void order_queue_push(struct order_queue* q, struct order o)
{
	assert(q->status != order_queue_full);
	q->items[q->head] = o;
	q->head = (q->head + 1) % (sizeof(q->items) / sizeof(q->items[0]));
	if (q->head == q->tail)
		q->status = order_queue_full;
	else
		q->status = order_queue_busy;
}

struct order order_queue_pop(struct order_queue* q)
{
	assert(q->status != order_queue_empty);
	struct order o = q->items[q->tail];
	q->tail = (q->tail + 1) % (sizeof(q->items) / sizeof(q->items[0]));
	if (q->head == q->tail)
		q->status = order_queue_empty;
	else
		q->status = order_queue_busy;
	return o;
}

struct cashier_thread_params {
	const char* name;
	struct order_queue* in_queue;
	struct order_queue* out_queue;
	const int* const cafe_opened;
};

struct barista_thread_params {
	const char* name;
	struct order_queue* in_queue;
	const int* const cafe_opened;
};

void do_work(int seconds)
{
	thrd_sleep(&(struct timespec) { .tv_sec = seconds }, NULL);
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

int cashier_thread_func(void* arg)
{
	struct cashier_thread_params* p = arg;
	printf("Cashier %s arrived.\n", p->name);
	while (*(p->cafe_opened)) {
		if (p->in_queue->status == order_queue_empty) {
			printf("Cashier %s has nothing to do.\n", p->name);
			while (p->in_queue->status == order_queue_empty && *(p->cafe_opened))
				do_work(1);
		}
		else {
			struct order o = order_queue_pop(p->in_queue);
			printf("Cashier %s is serving %s with %s.\n",
					p->name,
					o.customer_name,
					beverage_str(o.beverage)
			      );
			do_work(5);
			order_queue_push(p->out_queue, o);
			printf("Cashier %s has completed the order.\n", p->name);
		}
	}
	printf("Cashier %s went home.\n", p->name);
	return 0;
}

int barista_thread_func(void* arg)
{
	struct barista_thread_params* p = arg;
	printf("Barista %s arrived.\n", p->name);
	while (*(p->cafe_opened)) {
		if (p->in_queue->status == order_queue_empty) {
			printf("Barista %s has nothing to do.\n", p->name);
			while (p->in_queue->status == order_queue_empty && *(p->cafe_opened))
				do_work(1);
		}
		else {
			struct order o = order_queue_pop(p->in_queue);
			printf("Barista %s is serving %s with %s.\n",
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
			printf("Barista %s has completed the order.\n", p->name);
		}
	}
	printf("Barista %s went home.\n", p->name);
	return 0;
}

void input_loop(struct order_queue* out_queue, int* cafe_opened)
{
	while (*cafe_opened) {
		char command;
		int r3 = scanf("%c", &command);
		assert(r3 == 1);
		if (command == 'q') {
			*cafe_opened = 0;
		}
		else if (command == 'e' || command == 'c' || command == 't'|| command == 'o') {
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
			int r4 = scanf("%20s", o.customer_name);
			assert(r4 == 1);
			order_queue_push(out_queue, o);
			printf("Customer %s asked for %s.\n", o.customer_name, beverage_str(o.beverage));
		}
		else if (command == 's') {
			for (int i = 0; i < 10; ++i) {
				struct order o;
				o.beverage = beverage_cocoa;
				sprintf(o.customer_name, "Kid #%d", i);
				order_queue_push(out_queue, o);
				printf("Customer %s asked for %s.\n", o.customer_name, beverage_str(o.beverage));
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
	printf("Type s for a stress test of 10 orders.\n");
	printf("Type q to exit.\n");

	int cafe_opened = 1;
	struct order_queue cashier_queue = order_queue_new();
	struct order_queue barista_queue = order_queue_new();
	struct cashier_thread_params cashier_thread_params = {
		.name = "Carol",
		.cafe_opened = &cafe_opened,
		.in_queue = &cashier_queue,
		.out_queue = &barista_queue,
	};
	struct barista_thread_params barista_thread_params = {
		.name = "Bob",
		.cafe_opened = &cafe_opened,
		.in_queue = &barista_queue,
	};
	thrd_t cashier_thread;
	thrd_t barista_thread;

	int r1 = thrd_create(&cashier_thread, cashier_thread_func, &cashier_thread_params);
	assert(r1 == thrd_success);
	int r2 = thrd_create(&barista_thread, barista_thread_func, &barista_thread_params);
	assert(r2 == thrd_success);

	input_loop(&cashier_queue, &cafe_opened);

	printf("Cafe is closing.\n");
	thrd_join(cashier_thread, NULL);
	thrd_join(barista_thread, NULL);
	printf("Cafe closed.\n");
	return 0;
}
