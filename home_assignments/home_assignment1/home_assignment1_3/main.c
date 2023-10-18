#include <stdio.h>


unsigned int convertEndian(unsigned int num) {
    return ((num >> 24) & 0xFF) |
           ((num >> 8) & 0xFF00) |
           ((num << 8) & 0xFF0000) |
           ((num << 24) & 0xFF000000);
}

int main() {
    int N;
    printf("Введіть кількість чисел (N менше 100): ");
    scanf("%d", &N);

    if (N <= 0 || N >= 100) {
        printf("Неправильне значення N.\n");
        return 1; 
    }

    for (int i = 0; i < N; i++) {
        unsigned int num;
        printf("Введіть число %d: ", i + 1);
        scanf("%u", &num);

        unsigned int convertedNum = convertEndian(num);
        printf("Конвертоване число: %u\n", convertedNum);
    }

    return 0;
}