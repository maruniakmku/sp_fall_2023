#include <stdio.h>
#include <limits.h>

void demo1()
{
	printf("\nDemo 1: Integer size in bytes\n");
	printf("char:      %zd\n", sizeof(char));
	printf("short:     %zd\n", sizeof(short));
	printf("int:       %zd\n", sizeof(int));
	printf("long:      %zd\n", sizeof(long));
	printf("long long: %zd\n", sizeof(long long));
}

void demo2()
{
	int numbers[] = { 0, 1, 7, 15, 16, 17, 127, -1, -7, -128 };
	int i;
	printf("\nDemo 2: Hexademical representation of positive and negative integeres\n");
	for (i = 0; i < sizeof(numbers) / sizeof(numbers[0]); ++i) {
		printf("%4hhd = 0x%02hhx\n", numbers[i], numbers[i]);
	}
}


void demo3()
{
	long positive = 0x12345678;
	short short_positive = positive;
	long negative = 0xFFFF1234;
	short short_negative = negative;
	printf("\nDemo 3: Implicit conversion to smaller size\n");
	printf("Example 1: Positive number\n");
	printf("long:  %ld = 0x%08lx\n", positive, positive);
	printf("short: %hd = 0x%04hx\n", short_positive, short_positive);
	printf("Example 2: Negative number\n");
	printf("long:  %ld = 0x%08lx\n", negative, negative);
	printf("short: %hd = 0x%04hx\n", short_negative, short_negative);
}

void demo4()
{
	short positive = 0x1234;
	long long_positive = positive;
	short negative = 0x8123;
	long long_negative = negative;
	printf("\nDemo 4: Implicit conversion to bigger size\n");
	printf("Example 1: Positive number\n");
	printf("short: %hd = 0x%04hx\n", positive, positive);
	printf("long: %ld = 0x%08lx\n", long_positive, long_positive);
	printf("Example 2: Negative number\n");
	printf("short: %hd = 0x%04hx\n", negative, negative);
	printf("long: %ld = 0x%08lx\n", long_negative, long_negative);
}

void demo5()
{
	short short_max = SHRT_MAX;
	short short_max_plus_one = short_max + 1;
	short short_min = SHRT_MIN;
	short short_min_minus_one = short_min - 1;
	unsigned short ushort_max = USHRT_MAX;
	unsigned short ushort_max_plus_one = ushort_max + 1;
	unsigned short ushort_min = 0;
	unsigned short ushort_min_minus_one = ushort_min - 1;
	printf("\nDemo 5: Integer overflow\n");
	printf("Example 1: Overflow of signed types\n");
	printf("signed short min:     %hd\n", short_min);
	printf("signed short min - 1: %hd\n", short_min_minus_one);
	printf("signed short max:     %hd\n", short_max);
	printf("signed short max + 1: %hd\n", short_max_plus_one);
	printf("Example 2: Overflow of unsigned types\n");
	printf("unsigned short min:     %hu\n", ushort_min);
	printf("unsigned short min - 1: %hu\n", ushort_min_minus_one);
	printf("unsigned short max:     %hu\n", ushort_max);
	printf("unsigned short max + 1: %hu\n", ushort_max_plus_one);
}

int main()
{
	demo1();
	demo2();
	demo3();
	demo4();
	demo5();
	return 0;
}

