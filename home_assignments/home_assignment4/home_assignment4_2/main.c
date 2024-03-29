#include <assert.h>
#include <stdio.h>
#include <threads.h>
#include <stdlib.h>

struct semaphore {
	// TODO
};

void semaphore_init(struct semaphore* s, int n)
{
	// TODO
}

void semaphore_destroy(struct semaphore* s)
{
	// TODO
}

void semaphore_acquire(struct semaphore* s)
{
	// TODO
}

void semaphore_release(struct semaphore* s)
{
	// TODO
}

#define NPHILOSOPHERS 5

mtx_t forks[NPHILOSOPHERS];

int stop = 0;

struct semaphore waiter;

int left_fork(int philosopher)
{
	if (philosopher == 0)
		return NPHILOSOPHERS - 1;
	else
		return philosopher - 1;
}

int right_fork(int philosopher)
{
	return philosopher;
}

void sleep()
{
	// Sleep randomly for [0.1, 0.2, ..., 1.0] seconds.
	int rand_nsec = 100000000 * (1 + rand() % 10);
	thrd_sleep(&(struct timespec){ .tv_nsec = rand_nsec }, NULL);
}

int philosopher_thread(void* param)
{
	int philosopher = param;
	int fork1 = left_fork(philosopher);
	int fork2 = right_fork(philosopher);
	int mtx_result;
	while (!stop) {
		printf("Philosopher %d is thinking.\n", philosopher);
		sleep();

		printf("Philosopher %d is hungry.\n", philosopher);

		semaphore_acquire(&waiter);

		mtx_result = mtx_lock(&forks[fork1]);
		assert(mtx_result == thrd_success);
		printf("Philosoper %d took fork %d.\n", philosopher, fork1);
		mtx_result = mtx_lock(&forks[fork2]);
		assert(mtx_result == thrd_success);
		printf("Philosoper %d took fork %d.\n", philosopher, fork2);

		printf("Philosopher %d is eating.\n", philosopher);
		sleep();

		mtx_result = mtx_unlock(&forks[fork2]);
		assert(mtx_result == thrd_success);
		printf("Philosoper %d put fork %d.\n", philosopher, fork2);
		mtx_result = mtx_unlock(&forks[fork1]);
		assert(mtx_result == thrd_success);
		printf("Philosoper %d put fork %d.\n", philosopher, fork1);

		semaphore_release(&waiter);
	}
}

int main()
{
	thrd_t threads[NPHILOSOPHERS];
	semaphore_init(&waiter, NPHILOSOPHERS - 1);
	for (int i = 0; i < NPHILOSOPHERS; ++i) {
		int r = mtx_init(&forks[i], mtx_plain);
		assert(r == thrd_success);
	}
	for (int i = 0; i < NPHILOSOPHERS; ++i) {
		int r = thrd_create(&threads[i], philosopher_thread, i);
		assert(r == thrd_success);
	}
	printf("Philosophers ready. Press <ENTER> to exit.\n");
	while (fgetc(stdin) != '\n');
	stop = 1;
	printf("Waiting for philosophers to finish their job.\n");
	for (int i = 0; i < NPHILOSOPHERS; ++i) {
		thrd_join(threads[i], NULL);
	}
	for (int i = 0; i < NPHILOSOPHERS; ++i) {
		mtx_destroy(&forks[i]);
	}
	semaphore_destroy(&waiter);
	return 0;
}