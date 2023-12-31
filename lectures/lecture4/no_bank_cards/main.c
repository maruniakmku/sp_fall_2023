﻿#include <stdio.h>

/*
 * Програма no_bank_cards.
 * 
 * Шукає у вхідному тексті номера банківських карт, 16 чисел що йдуть підряд.
 * 
 * Завершує виконання з кодом що дорівнює кількості знайдених номерів.
 * 
 * Сфера використання - інформаційна безпека, допомагає автоматично перевірити
 * відсутність даних користувача при передачі текстових даних,
 * наприклад журналів виконання (logs).
 */
int main()
{
	char c;
	int digits = 0;
	int bank_cards_found = 0;

	while ((c = getchar()) != EOF) {
		if (c >= '0' && c <= '9') {
			digits += 1;
		} else {
			digits = 0;
		}
		if (digits == 16) {
			bank_cards_found += 1;
			digits = 0;
		}
	}
	return bank_cards_found;
}
