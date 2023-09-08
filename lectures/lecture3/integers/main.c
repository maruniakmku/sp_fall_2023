#include <stdio.h>

void demo1()
{
	printf("\nInteger size in bytes\n");
	printf("char: %d\n", sizeof(char));
	printf("short: %d\n", sizeof(short));
	printf("int: %d\n", sizeof(int));
	printf("long: %d\n", sizeof(long));
	printf("long long: %d\n", sizeof(long long));
}

void demo2()
{
	int numbers[] = { 0, 1, 7, 15, 16, 17, 127, -1, -7, -128 };
	int i;
	printf("\nHexademical representation of positive and negative integeres\n");
	for (i = 0; i < sizeof(numbers) / sizeof(numbers[0]); ++i) {
		printf("%d = 0x%x\n", numbers[i], numbers[i]);
	}
}


void demo3()
{
	int i = 65539;
	short i_as_short = i;
	char i_as_char = i;
	int j = -65533;
	short j_as_short = j;
	char j_as_char = j;
	printf("\nImplicit conversion to smaller size\n");
	printf("int: %d = 0x%x\n", i, i);
	printf("short: %d = 0x%hx\n", i_as_short, i_as_short);
	printf("char: %d = 0x%hhx\n", i_as_char, i_as_char);
	printf("\n");
	printf("int: %d = 0x%x\n", j, j);
	printf("short: %d = 0x%hx\n", j_as_short, j_as_short);
	printf("char: %d = 0x%hhx\n", j_as_char, j_as_char);
}

void demo4()
{
	char i = 5;
	short i_as_short = i;
	int i_as_int = i;
	char j = -5;
	short j_as_short = j;
	int j_as_int = j;
	printf("\nImplicit conversion to bigger size\n");
	printf("char: %hhd = 0x%hhx\n", i, i);
	printf("short: %hd = 0x%hx\n", i_as_short, i_as_short);
	printf("int: %d = 0x%x\n", i_as_int, i_as_int);
	printf("\n");
	printf("char: %hhd = 0x%hhx\n", j, j);
	printf("short: %hd = 0x%hx\n", j_as_short, j_as_short);
	printf("int: %d = 0x%x\n", j_as_int, j_as_int);
}

int main()
{
	demo1();
	demo2();
	demo3();
	demo4();
	return 0;
}

