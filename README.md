# Игра сапер

Сапер – популярная компьютерная игра, которая требует от игрока логического мыш-ления и умения принимать решения на основе предоставленной информации.
Цель игры заключается в раскрытии всех безопасных клеток на игровом поле, избегая взрывов мин. Сапер является классическим представителем жанра "головоломки"и широко известен благодаря включению в стандартные компьютерные операционные системы, такие как Windows.

![bandicam-2024-08-25-03-56-52-560-_online-video-cutter com_](https://github.com/user-attachments/assets/bbc4d386-89f9-405b-990c-731c23c4d4e5)

## Основные компоненты проекта
Подключение библиотек и определение констант:

#include <SFML/Graphics.hpp>: Подключает библиотеку SFML для работы с графикой.
#include <ctime>: Используется для генерации случайных чисел.
#include <queue>: Используется для реализации очереди, необходимой для алгоритма поиска в ширину.
#include <iostream>: Для вывода сообщений об ошибках.
Константы SIZE, MINE, FLAG, и EMPTY определяют размеры игрового поля и специальные значения для бомб, флажков и пустых клеток.

### Основная функция (main):

- Создает окно игры размером 400x400 пикселей.
- Загружает текстуру для отображения клеток и шрифт для текста.
- Инициализирует игровое поле и размещает бомбы.
- Основной игровой цикл:
- Обрабатывает события (закрытие окна, нажатие клавиш и кнопок мыши).
- Выполняет обновление состояния игры и отрисовку клеток на экране.
- При проигрыше отображает сообщение "Game Over" и предлагает нажать R для перезапуска игры.

### Функция initializeGame:

- Инициализирует игровое поле, размещает бомбы и вычисляет количество бомб вокруг каждой клетки.
- Вызывается при запуске игры и при перезапуске.

###  Функция revealArea:

- Открывает все связанные пустые клетки, начиная с указанной координаты, используя алгоритм поиска в ширину (BFS).
- Добавляет пустые клетки в очередь и обрабатывает их, открывая их и их соседние пустые клетки.

###  Функция gameOver:

- Отображает сообщение о завершении игры и инструкцию для перезапуска.
- Использует шрифт для отображения текста на экране.
