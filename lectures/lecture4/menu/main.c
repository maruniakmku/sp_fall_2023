#include <conio.h>  /* Warning: Windows only */
#include <stdio.h>

/*
 * Програма menu.
 *
 * Демонстрація використання констант та елементів контролю за виконанням програми.
 * 
 * Увага: Працює лише на платформі Windows.
 */
int main()
{
	const char active_fmt[] =   "--> %s\n";
	const char inactive_fmt[] = "    %s\n";
	const char* items[] = {
		"Start new game",
		"Load savepoint",
		"Options",
		"Help",
		"Exit",
	};
	const int nitems = sizeof(items) / sizeof(items[0]);

	int i, c;
	int exit = 0;
	int current_item = 0;

	while (!exit) {
		printf("\033[2J");  /* Спец. послідовність VT100: очистити екран */
		printf("\033[1;1H");  /* Спец. послідовність VT100: встановити курсов в верхній лівий кут екрану */

		/* Виводимо на екран всі опції меню */
		for (i = 0; i < nitems; ++i)
			printf(i == current_item ? active_fmt : inactive_fmt, items[i]);

		/* Очікуємо вводу з клавіатури */
		c = _getch();

		/* Оновлюємо стан меню згідно коду клавіші */
		switch (c) {
		case 'w':
		case 'W':
		case 72:  /* UP */
			current_item = current_item == 0 ? nitems - 1 : current_item - 1;
			break;
		case 's':
		case 'S':
		case 80:  /* DOWN */
			current_item = (current_item + 1) % nitems;
			break;
		case 13:  /* ENTER */
		case 32:  /* SPACE */
			if (current_item == nitems - 1)
				exit = 1;
			break;
		case 27:  /* ESC */
			exit = 1;
		default:
			break;
		}
	}
	return 0;
}
