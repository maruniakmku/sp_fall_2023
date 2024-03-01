#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <threads.h>

/*
 * This function is a part of the Merge Sort algorithm.
 * Merges two sorted parts of array `a` indicating by indexes `l`, `m`, `r` into the
 * corresponding indexes of array `b`.
 */
void merge(int* a, int* b, int l, int m, int r)
{
	int i = l;
	int j = m;
	for (int k = l; k < r; ++k) {
		if (i < m && (j >= r || a[i] < a[j])) {
			b[k] = a[i];
			i += 1;
		}
		else {
			b[k] = a[j];
			j += 1;
		}
	}
}

/*
 * Returns the middle point of the range [l; r).
 */
int middle_point(int l, int r)
{
	return l + (r - l) / 2;
}

/*
 * Auxilary, recursive function of the Merge Sort algorithm that does most of the sorting.
 * Sorts the part of array `a` pointed by indexes `l` and `r` into the corresponding
 * indexes of array `b`. The right index `r` is not inclusive.
 * 
 * Important prerequisite: on the first invocation array `b` MUST be a copy of array `a`.
 */
void split_and_sort(int* a, int* b, int l, int r)
{
	if (r - l <= 1)
		return;

	// Find the middle point `m` to split the array in half.
	int m = middle_point(l, r);

	// Recursively sort the left part of `b` into `a`.
	split_and_sort(b, a, l, m);
	
	// Recursively sort the right part of `b` into `a`.
	split_and_sort(b, a, m, r);

	// Merge the left and right parts of `a` into `b`.
	merge(a, b, l, m, r);
}

/*
 * Sorts array `a` of size `n` using the Merge Sort algorithm.
 * https://en.wikipedia.org/wiki/Merge_sort
 */
void merge_sort(int* a, int n) {
	// Allocate a temporary buffer to store the sorted result;
	int* b = calloc(n, sizeof(b[0]));
	assert(b != NULL);

	// Copy unsorted array `a` into the buffer.
	memcpy(b, a, n * sizeof(b[0]));

	// Perform the recursive algorithm.
	split_and_sort(a, b, 0, n);
	
	// Copy the result back into `a`.
	memcpy(a, b, n * sizeof(b[0]));

	free(b);
}

struct sort_thread_params {
	int* a;
	int* b;
	int l;
	int r;
};

int sort_thread_func(void* arg)
{
	struct sort_thread_params* p = arg;
	split_and_sort(p->a, p->b, p->l, p->r);
	return 0;
}

/*
 * Multithreaded (degree 2) version of the Merge Sort algorithm.
 */
void mt_merge_sort(int* a, int n) {
	// Allocate a temporary buffer to store the sorted result;
	int* b = calloc(n, sizeof(b[0]));
	assert(b != NULL);

	// Copy unsorted array `a` into the buffer.
	memcpy(b, a, n * sizeof(b[0]));

	// Find the middle point `m` to split the array in half.
	int m = middle_point(0, n);

	// Offload sorting of the right part to aux thread.
	thrd_t right_thread;
	struct sort_thread_params right_thread_params = {
		.a = b,
		.b = a,
		.l = m,
		.r = n
	};
	
	int r1 = thrd_create(&right_thread, sort_thread_func, &right_thread_params);
	assert(r1 == thrd_success);

	// Perform the recursive algorithm on the left part.
	split_and_sort(b, a, 0, m);

	// Wait till the aux thread completes.
	thrd_join(right_thread, NULL);

	// Merge two parts together.
	merge(a, b, 0, m, n);

	// Copy the result back into `a`.
	memcpy(a, b, n * sizeof(b[0]));

	free(b);
}

/*
 * Returns 1 if array `a` of size `n` is in ascenging order, 0 otherwise.
 */
int is_sorted(int* a, int n) {
	if (n < 2)
		return 1;
	int result = 1;
	for (int i = 1; i < n; ++i) {
		if (a[i] < a[i - 1]) {
			result = 0;
			break;
		}
	}
	return result;
}

/*
 * Prints the current time on the screen in the following format "Mon Feb 3 12:15:35 2000\n".
 */
void print_current_time()
{
	time_t seconds = time(NULL);
	struct tm *time_parts = localtime(&seconds);
	assert(time_parts != NULL);
	char asctime_buffer[26];
	char* time_str = asctime(time_parts);
	assert(time_str != NULL);
	printf("%s", time_str);
}

int main()
{
	int n = 250000000;

	int* a1 = calloc(n, sizeof(a1[0]));
	assert(a1 != NULL);

	int* a2 = calloc(n, sizeof(a2[0]));
	assert(a2 != NULL);

	print_current_time();
	printf("Arrays generation started.\n");

	for (int i = 0; i < n; ++i) {
		// Generates a random number in range [0, n]
		int rn = (int)(((double)rand() / RAND_MAX) * n);
		a1[i] = rn;
		a2[i] = rn;
	}
	
	print_current_time();
	printf("Two random arrays of size %d generated.\n", n);
	
	merge_sort(a1, n);
	
	print_current_time();
	printf("Single threaded sorting completed.\n");
	
	printf("Correctness test: %s\n", is_sorted(a1, n) ? "PASSED" : "FAILED");

	mt_merge_sort(a2, n);

	print_current_time();
	printf("Multi threaded sorting completed.\n");

	printf("Correctness test: %s\n", is_sorted(a2, n) ? "PASSED" : "FAILED");

	free(a2);
	free(a1);
	return 0;
}
