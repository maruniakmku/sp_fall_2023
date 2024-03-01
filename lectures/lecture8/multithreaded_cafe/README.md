# Симулятор кав'ярні

Демонстраційна програма використання потоків (threads) для побудови інтерфейсу
користувача зі швидким реагуванням та одночасного виконання різних запитів.

Програма складається з трьох потоків:

 1. основний - очікує від користувача вводу замовлень на приготування гарячих
   напоїв або команди на закінчення роботи програми;
 2. касир - першим обслуговує чергу замовлень, витрачаючи 5 секунд на кожне;
 3. бариста - готує напої за чергою замовлень витрачаючи різний час на кожне
   кожне замовлення в залежності від напою.
   
Всі потоки працюють незалежно і не блокують один одного.

Програма дозволяє створювати ситуації коли біля касира та/чи бариста створюється
черга замовлень при цьому інтерфейс користувача залишається активним і дозволяє
додавати ще більше замовлень.

Для передачі замовлень між потоками використовуються черги у вигляді циклічного
буфера.

В програмі визначено дві черги:

 1. Черга касира. Основний потік додає замовлення. Потік касира обробляє
    замовлення.
 2. Черга баристи. Потік касира додає замовлення. Потік баристи обробляє
    замовлення.

Недоліки поточної реалізації:

  1. Обидві черги мають фіксований максимальний розмір в 100 замовлень при
     досягнення якого програма "падає" з помилкою.
  2. Програма не використовує механізми синхронзації потоків для захисту черг
     від одночасної зміни їх вмісту декількома потоками в наслідок чого
     не можливо розширити кількість одночасно працюючих касирів чи бариста.

Додаткові посилання:

 * [Циклічний буфер](https://uk.wikipedia.org/wiki/%D0%A6%D0%B8%D0%BA%D0%BB%D1%96%D1%87%D0%BD%D0%B8%D0%B9_%D0%B1%D1%83%D1%84%D0%B5%D1%80)