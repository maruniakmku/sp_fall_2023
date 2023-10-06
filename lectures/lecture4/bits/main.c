#include <stdio.h>

/*
 * Програма bits.
 * 
 * Для кожного введеного цілого знакового 32-бітного числа відображає його бітове представлення.
 * 
 * Демонструє використання бітових операцій та функцій scanf, sscanf.
 */
int main()
{
	int scanf_result;
	int number;
	char input[20];
	while (1) {
		scanf_result = scanf("%19s", input);
		if (scanf_result == EOF)
			break;
		scanf_result = sscanf(input, "%d", &number);
		if (scanf_result == 0) {
			printf("ERROR: Input is not an integer.\n");
			continue;
		}
		int i;
		for (i = 31; i >= 0; --i) {
			/* Option 1 */
			int mask = 1 << i;
			int bit = number & mask ? 1 : 0;
			/* Option 2 */
			/* int bit = (number >> i) & 1; */
			printf("%d", bit);
		}
		printf("\n");
	}
	return 0;
}