#include <stdio.h>

int main() {
    int N;
    printf("Введіть кількість чисел (N менше 100): ");
    scanf("%d", &N);

    if (N <= 0 || N >= 100) {
        printf("Неправильне значення N.\n");
        return 1; 
    }

    int sum = 0;
    for (int i = 0; i < N; i++) {
        int num;
        printf("Введіть число %d: ", i + 1);
        scanf("%d", &num);

        if (num <= 0) {
            printf("Введені числа повинні бути більше 0.\n");
            return 1; 
        }

        sum += num;
    }

    double average = (double)sum / N; 
    printf("Середнє арифметичне: %.2f\n", average);

    return 0;
}