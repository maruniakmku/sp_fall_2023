# Практична робота 1

Терміни здачі - 21 жовтня 2023р.

Для заліку задач 1.2 та 1.3 додайте файли main.c з розв'язком задач до відповідних завдань в Moodle.

## Задача 1.1 (не потребує заліку)

Ознайомтесь з мовою C та середовищем для розробки самостійно набравши та запустивши всі приклади з першого розділу книги "Мова програмування C, друге видання".

## Задача 1.2

Відкрийте в Visual Studio проект home_assignment1_2\home_assignment1_2.sln

В файлі main.c напишіть програму що рахує середнє арифметичне цілих 32 бітних чисел більше 0.

Програма має в першому рядку очікувати ціле число N менше 100 - кількість вхідних чисел. З другого по N + 1 рядки програма має очікувати по одному вхідному числу.

Програма має вивести на екран лише один рядок з одним числом.

Тестові вхідні послідовності завжди будуть мати ціле середнє арефметичне.

Для перевірки свого рішення, запустіть скрипт home_assignment1_2\tests\run_tests.cmd.

### Приклад 1
Вхідні дані
```
0
```

Очікуваний результат
```
0
```

### Приклад 2
Вхідні дані
```
1
7
```

Очікуваний результат
```
7
```

### Приклад 3
Вхідні дані
```
3
2
4
3
```

Очікуваний результат
```
3
```

## Задача 1.3

Відкрийте в Visual Studio проект home_assignment1_3\home_assignment1_3.sln

В файлі main.c напишіть програму що конвертує цілі числа з big-endian в little-endian формат.

Програма має в першому рядку очікувати ціле число N менше 100 - кількість вхідних чисел. З другого по N + 1 рядки програма має очікувати по одному вхідному числу.

На кожне вхідне число, програма має вивести його сконвертоване значення в окремому рядку.

**Увага**: Для збереження числа, використовуйте беззнаковий тип даних `unsigned int`, а для його вводу та відображення на екрані формат `%u` функцій `printf` та `scanf`.

Для перевірки свого рішення, запустіть скрипт home_assignment1_3\tests\run_tests.cmd.

### Приклад
Вхідні дані
```
3
0
1
3892510720
```

Очікуваний результат
```
0
16777216
1000
```

Пояснення:

* 0 в обох форматах має однакове значення.
* 1 конвертується в 16777216 (0x01000000 або 2 ^ 24).
* 3892510720 це 0xe8030000, а 1000 це 0x000003e8.
