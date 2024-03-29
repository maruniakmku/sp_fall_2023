#include <assert.h>
#include <stdio.h>
#include <threads.h>

int account_a = 1000;
int account_b = 1000;

//mtx_t transfer_lock;

int thread_func1(void *params)
{
	const char *thread_name = params;
	//mtx_lock(&transfer_lock);
	int a = account_a;
	int b = account_b;
	printf(
		"%s: Transfering 200$ from account A to account B...\n",
		thread_name
	);
	a -= 200;
	b += 200;
	account_a = a;
	account_b = b;
	printf("%s: Done\n", thread_name);
	//mtx_unlock(&transfer_lock);
	return 0;
}

int thread_func2(void *params)
{
	const char *thread_name = params;
	//mtx_lock(&transfer_lock);
	int a = account_a;
	int b = account_b;
	printf(
		"%s: Transfering 200$ from account B to account A...\n",
		thread_name
	);
	a += 200;
	b -= 200;
	account_a = a;
	account_b = b;
	printf("%s: Done\n", thread_name);
	//mtx_unlock(&transfer_lock);
	return 0;
}

int main()
{
	thrd_t t1, t2;
	int thrd_result;

	//thrd_result = mtx_init(&transfer_lock, mtx_plain);
	//assert(thrd_result == thrd_success);

	printf("Account A balance: %d\n", account_a);
	printf("Account B balance: %d\n", account_b);

	thrd_result = thrd_create(&t1, thread_func1, "Thread 1");
	assert(thrd_result == thrd_success);

	thrd_result = thrd_create(&t2, thread_func2, "Thread 2");
	assert(thrd_result == thrd_success);

	thrd_join(t1, NULL);
	thrd_join(t2, NULL);

	printf("Account A balance: %d\n", account_a);
	printf("Account B balance: %d\n", account_b);

	//mtx_destroy(&transfer_lock);

	return 0;
}
