#include <stdio.h>

int main()
{
	char characters[] = {'a', 'b', 'c', 'z', 'A', 'B', 'C', 'Z', '0', '1', '2', '3', '9', ' ', '\n'};
	int i;
	for (i = 0; i < sizeof(characters); ++i) {
		printf("\'%c\' = %hhd\n", characters[i], characters[i]);
	}
	return 0;
}