#include <SFML/Graphics.hpp>
#include <ctime>
#include <queue>
#include <iostream>
using namespace sf;

const int SIZE = 12;
const int MINE = 9;
const int FLAG = 11;
const int EMPTY = 0;

void revealArea(int x, int y, int gridLogic[SIZE][SIZE], int gridView[SIZE][SIZE]);
void gameOver(RenderWindow& app, Font& font);

int main()
{
    // Генератор случайных чисел
    srand(static_cast<unsigned>(time(0)));

    RenderWindow app(VideoMode(400, 400), "Minesweeper!");

    // Ширина клетки
    int w = 32;
    int gridLogic[SIZE][SIZE];
    int gridView[SIZE][SIZE];

    // Загрузка текстуры и создание спрайта
    Texture t;
    if (!t.loadFromFile("tiles.jpg")) {
        std::cerr << "Error loading texture" << std::endl;
        return -1;
    }
    Sprite s(t);

    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return -1;
    }

    bool gameLost = false;
    bool restartRequested = false;

    auto initializeGame = [&]() {
        // Инициализация сетки
        int bombCount = 10;  // Установите желаемое количество бомб
        for (int i = 1; i <= 10; i++)
            for (int j = 1; j <= 10; j++)
            {
                gridView[i][j] = 10;
                gridLogic[i][j] = EMPTY;
            }

        // Размещение бомб
        int placedBombs = 0;
        while (placedBombs < bombCount)
        {
            int i = rand() % 10 + 1;
            int j = rand() % 10 + 1;
            if (gridLogic[i][j] != MINE)
            {
                gridLogic[i][j] = MINE;
                placedBombs++;
            }
        }

        // Подсчет мин вокруг каждой клетки
        for (int i = 1; i <= 10; i++)
            for (int j = 1; j <= 10; j++)
            {
                int n = 0;
                if (gridLogic[i][j] == MINE) continue;
                for (int dx = -1; dx <= 1; dx++)
                    for (int dy = -1; dy <= 1; dy++)
                        if (gridLogic[i + dx][j + dy] == MINE)
                            n++;
                gridLogic[i][j] = n;
            }

        gameLost = false;
        restartRequested = false;
        };

    initializeGame(); // Инициализация игры при запуске

    while (app.isOpen())
    {
        // Получаем координаты мышки относительно окна нашего приложения
        Vector2i pos = Mouse::getPosition(app);
        int x = pos.x / w;
        int y = pos.y / w;

        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();

            if (e.type == Event::KeyPressed)
            {
                if (e.key.code == Keyboard::R && gameLost)
                {
                    initializeGame();
                }
            }

            // Была нажата кнопка мыши…?
            if (e.type == Event::MouseButtonPressed)
            {
                // Если это - левая кнопка мыши, то открываем клетку
                if (e.mouseButton.button == Mouse::Left && !gameLost)
                {
                    if (x >= 1 && x <= 10 && y >= 1 && y <= 10) // Проверка на допустимость координат
                    {
                        if (gridLogic[x][y] == MINE)
                        {
                            gameLost = true;
                            // Открыть все клетки для демонстрации проигрыша
                            for (int i = 1; i <= 10; i++)
                                for (int j = 1; j <= 10; j++)
                                    if (gridLogic[i][j] == MINE)
                                        gridView[i][j] = MINE;
                        }
                        else
                        {
                            // Открыть все связные пустые области
                            revealArea(x, y, gridLogic, gridView);
                        }
                    }
                }
                // Если это – правая кнопка мыши, то отображаем флажок
                else if (e.mouseButton.button == Mouse::Right && !gameLost)
                {
                    if (x >= 1 && x <= 10 && y >= 1 && y <= 10) // Проверка на допустимость координат
                    {
                        gridView[x][y] = (gridView[x][y] == FLAG) ? 10 : FLAG;
                    }
                }
            }
        }

        // Устанавливаем белый фон
        app.clear(Color::White);

        for (int i = 1; i <= 10; i++)
            for (int j = 1; j <= 10; j++)
            {
                // Вырезаем из спрайта нужный нам квадратик текстуры
                s.setTextureRect(IntRect(gridView[i][j] * w, 0, w, w));

                // Устанавливаем его заданную позицию…
                s.setPosition(i * w, j * w);

                // … и отрисовываем
                app.draw(s);
            }

        if (gameLost)
        {
            gameOver(app, font);
        }

        // Отображаем всю композицию на экране
        app.display();
    }

    return 0;
}

// Функция для открытия всей связанной пустой области
void revealArea(int x, int y, int gridLogic[SIZE][SIZE], int gridView[SIZE][SIZE])
{
    std::queue<std::pair<int, int>> cells; // Создание очереди для хранения координат клеток
    cells.push(std::make_pair(x, y)); // Добавление начальной клетки в очередь
    gridView[x][y] = gridLogic[x][y]; // Открываем начальную клетку

    while (!cells.empty()) // Пока в очереди есть клетки для обработки
    {
        std::pair<int, int> cell = cells.front(); // Получаем координаты клетки из начала очереди
        cells.pop(); // Удаляем эту клетку из очереди
        int cx = cell.first; // Получаем x-координату
        int cy = cell.second; // Получаем y-координату

        if (gridLogic[cx][cy] != EMPTY)
            continue; // Если клетка не пустая, пропускаем её

        // Проверяем соседние клетки
        for (int dx = -1; dx <= 1; dx++)
            for (int dy = -1; dy <= 1; dy++)
            {
                int nx = cx + dx; // Новая x-координата
                int ny = cy + dy; // Новая y-координата

                // Проверяем, что новая клетка в пределах границ поля и ещё не открыта
                if (nx >= 1 && nx <= 10 && ny >= 1 && ny <= 10 && gridView[nx][ny] == 10)
                {
                    gridView[nx][ny] = gridLogic[nx][ny]; // Открываем новую клетку

                    // Если новая клетка пустая, добавляем её в очередь для дальнейшей обработки
                    if (gridLogic[nx][ny] == EMPTY)
                        cells.push(std::make_pair(nx, ny));
                }
            }
    }
}

// Функция для обработки проигрыша
void gameOver(RenderWindow& app, Font& font)
{
    Text text;
    text.setFont(font);
    text.setString("Game Over!\nPress R to Restart");
    text.setCharacterSize(15);
    text.setFillColor(Color::Red);
    text.setPosition(150, 350);

    app.draw(text);
}
