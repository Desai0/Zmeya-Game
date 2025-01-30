#include <list>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <thread>
#include <atomic>
#include <iterator>
#include <clocale>
#include <iostream>

enum Direction {
    UP = 1,
    DOWN = -1,
    LEFT = 2,
    RIGHT = -2,
    NONE = 0
};


int w = 48;
int h = 20;
int apple_coordX = 0;
int apple_coordY = 0;
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); //возвращает

struct Snake {
    std::list<COORD> snake; //.front - голова .back - конец, push_front, push_back, pop_front, pop_back
    Direction direction;

    Snake() {
        snake = { { 20, 17}, {20, 18}, {20, 19} };
        direction = Direction::UP;
    }


    void move() {
        COORD h = snake.front();
        COORD b = snake.front();
        COORD z = snake.back();
        if (direction == UP) {
            b.X = h.X;
            b.Y = h.Y - 1;
        }
        else if (direction == DOWN) {
            b.X = h.X;
            b.Y = h.Y + 1;
        }
        else if (direction == LEFT) {
            b.X = h.X - 1;
            b.Y = h.Y;
        }
        else if (direction == RIGHT) {
            b.X = h.X + 1;
            b.Y = h.Y;
        }

        snake.push_front(b);
        snake.pop_back();
        SetConsoleCursorPosition(console, { short(z.X), short(z.Y) }); // убрать цвет на прошлом пути змейки
        printf("%c", ' ');
    }

    void eat() {
        snake.push_back(snake.back());
    }


    bool crash() {
        COORD h = snake.front();
        if (h.X == 0 || h.X == 47 || h.Y == 0 || h.Y == 19) {
            direction = NONE;
            /*SetConsoleCursorPosition(console, { short(h.X), short(h.Y) });
            printf("Game Over u loh");*/

            return true;
        }
        return false;
    }

    void ru_draw() {
        COORD c = snake.front();
        int colors[] = { 41, 47, 44 }; // Красный, белый, синий
        int randomColor = colors[rand() % 3];
        SetConsoleCursorPosition(console, c);
        printf("\033[%dm%c\033[0m", randomColor, ' ');
        //for (COORD c : snake) { //цикл по всей змее, (с.xy будет)
        //    SetConsoleCursorPosition(console, c);
        //    printf("\033[41m%c\033[0m", ' ');
        //}
    }

    void def_draw() {
        for (COORD c : snake) { //цикл по всей змее, (с.xy будет)
            SetConsoleCursorPosition(console, c);
            printf("\033[41m%c\033[0m", ' ');
        }
    }

    void rgb_draw() {
        for (COORD c : snake) { // цикл по всей змее
            SetConsoleCursorPosition(console, c);

            // Генерация случайного цвета в диапазоне от 41 до 47 (фоновые цвета ANSI)
            int randomColor = 41 + rand() % 7;

            // Печать с использованием случайного цвета
            printf("\033[%dm%c\033[0m", randomColor, ' ');
        }
    }

    //void draw() {
    //    srand(time(NULL));
    //    for (COORD c : snake) { //цикл по всей змее, (с.xy будет)
    //        // Задаём массив доступных цветов
    //        int colors[] = { 41, 42, 44 }; // Красный, зелёный, синий
    //        int randomColor = colors[rand() % 3]; // Выбираем случайный цвет из массива
    //        SetConsoleCursorPosition(console, c);
    //        printf("\033[%dm%c\033[0m", randomColor, ' ');/*printf("\033[41m%c\033[0m", ' ');*/
    //    }
    //}

    bool check() {
        COORD h = snake.front();
        if (short(h.X) == apple_coordX && short(h.Y) == apple_coordY) {
            return true;
        }
        return false;
    }

    bool samokill() {
        if (snake.size() <= 3) return false; // Если змейка слишком короткая, самоуничтожение невозможно

        COORD h = snake.front(); // Координаты головы

        // Проверяем голову против тела
        auto it = snake.begin();
        ++it; // Пропускаем голову
        for (; it != snake.end(); ++it) {
            if (h.X == it->X && h.Y == it->Y) {
                return true; // Если координаты совпадают, змейка врезалась в себя
            }
        }

        return false; // Если совпадений не найдено
    }


};

void drawFrame() {
    COORD pos = { 0, 0 };
    SetConsoleCursorPosition(console, pos);

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 48; j++) {
            SetConsoleCursorPosition(console, { short(j), short(i) });
            if (i != 0 && i != 19) {
                printf("\033[101m%c\033[0m", ' ');
                j += 47;
                SetConsoleCursorPosition(console, { short(j), short(i) });
                printf("\033[101m%c\033[0m", ' ');
                break;
            }
            printf("\033[101m%c\033[0m", ' ');
        }
    }

}

void ru_apple() {
    srand(time(NULL));
    int a_xcord = rand();
    int a_ycord = rand();
    if (a_xcord >= 48) {
        a_xcord %= 48;
    }

    if (a_xcord == 0) {
        a_xcord += 1;
    }
    else if (a_xcord == 47) {
        a_xcord -= 1;
    }


    if (a_ycord >= 20) {
        a_ycord %= 20;
    }

    if (a_ycord == 0) {
        a_ycord += 1;
    }
    else if (a_ycord == 19) {
        a_ycord -= 1;
    };

    SetConsoleCursorPosition(console, { short(a_xcord), short(a_ycord) });
    printf("\033[92m%c\033[0m", 'Z');
    SetConsoleCursorPosition(console, { short(48), short(20) });
    apple_coordX = a_xcord;
    apple_coordY = a_ycord;
}

void apple() {
    srand(time(NULL));
    int a_xcord = rand();
    int a_ycord = rand();
    if (a_xcord >= 48) {
        a_xcord %= 48;
    }

    if (a_xcord == 0) {
        a_xcord += 1;
    }
    else if (a_xcord == 47) {
        a_xcord -= 1;
    }


    if (a_ycord >= 20) {
        a_ycord %= 20;
    }

    if (a_ycord == 0) {
        a_ycord += 1;
    }
    else if (a_ycord == 19) {
        a_ycord -= 1;
    };

    SetConsoleCursorPosition(console, { short(a_xcord), short(a_ycord) });
    printf("\033[92m%c\033[0m", '@');
    SetConsoleCursorPosition(console, { short(48), short(20) });
    apple_coordX = a_xcord;
    apple_coordY = a_ycord;
}

void getKey(Snake* snake) {
    Direction newDir = Direction::NONE;
    if (GetAsyncKeyState(VK_UP)) newDir = Direction::UP;
    else if (GetAsyncKeyState(VK_LEFT)) newDir = Direction::LEFT;
    else if (GetAsyncKeyState(VK_DOWN)) newDir = Direction::DOWN;
    else if (GetAsyncKeyState(VK_RIGHT)) newDir = Direction::RIGHT; //если направление противоположно - игнорируем
    if (newDir == Direction::NONE) {
        return;
    }

    if (newDir != -snake->direction) snake->direction = newDir;
}



/// ////////////////////////////////
std::atomic<bool> isMainThreadRunning{ true };
std::atomic<bool> shouldStopSecondThread{ false };

void secondThreadFunc(Snake* snake) {
    while (!shouldStopSecondThread.load()) { // Проверяем флаг остановки
        if (!isMainThreadRunning.load()) {  // Проверяем, работает ли основной поток
            break; // Завершаем второй поток
        }
        getKey(snake);
        Sleep(90); // Уменьшение нагрузки
    }
}




int main()
{
    setlocale(0, "");
    std::cout << "Выберите сложность: 1 - Легкая, 2 - средняя, 3 - сложная" << std::endl;
    int difficult = 0;
    std::cin >> difficult;
    std::cout << "Выберите оформление: 1 - Обычное, 2 - RGB, 3 - Russian Edition" << std::endl;
    int visual = 0;
    std::cin >> visual;
    /*system("cls");*/


    Snake a;
    a.direction = Direction::UP; // Устанавливаем начальное направление
    int smerti = 0;
    int schet = 0;
    std::thread secondThread(secondThreadFunc, &a); //запуск 2 потока

    while (true)
    {
        system("cls");
        schet = 0;
        SetConsoleCursorPosition(console, { 55, 10 });
        printf("Press strelochka to start game");
        SetConsoleCursorPosition(console, { 55, 14 });
        printf("Кол-во смертей:");
        SetConsoleCursorPosition(console, { 62, 15 });
        printf("%d \n", smerti);
        drawFrame();
        apple();
        if (visual == 1 && difficult == 1) {
            while (true) {
                /*getKey(&a);*/
                a.move();
                /*getKey(&a);*/
                a.def_draw();
                /*getKey(&a);*/
                if (a.check()) {
                    apple();
                    a.eat();
                    schet++;
                    SetConsoleCursorPosition(console, { 55, 12 });
                    printf("Ваш счет: %d \n", schet);
                }
                /*getKey(&a);*/
                if (a.crash()) {
                    SetConsoleCursorPosition(console, { 20, 10 });
                    printf("Game Over u loh");
                    a.snake = { { 20, 17 }, { 20, 18 }, { 20, 19 } };
                    smerti++;
                    Sleep(1500);
                    break;
                }
                if (a.samokill()) {
                    SetConsoleCursorPosition(console, { 20, 10 });
                    printf("ti v sebya vreZalsya ");
                    a.snake = { { 20, 17 }, { 20, 18 }, { 20, 19 } };
                    smerti++;
                    Sleep(1500);
                    break;
                }
                /*getKey(&a);*/
                if (a.direction == Direction::RIGHT || a.direction == Direction::LEFT) {
                    Sleep(125);
                }
                else {
                    Sleep(250);
                }
            }
        }

        if (visual == 2 && difficult == 1) {
            while (true) {
                /*getKey(&a);*/
                a.move();
                /*getKey(&a);*/
                a.rgb_draw();
                /*getKey(&a);*/
                if (a.check()) {
                    apple();
                    a.eat();
                    schet++;
                    SetConsoleCursorPosition(console, { 55, 12 });
                    printf("Ваш счет: %d \n", schet);
                }
                /*getKey(&a);*/
                if (a.crash()) {
                    SetConsoleCursorPosition(console, { 20, 10 });
                    printf("Game Over u loh");
                    a.snake = { { 20, 17 }, { 20, 18 }, { 20, 19 } };
                    smerti++;
                    Sleep(1500);
                    break;
                }
                if (a.samokill()) {
                    SetConsoleCursorPosition(console, { 20, 10 });
                    printf("ti v sebya vreZalsya ");
                    a.snake = { { 20, 17 }, { 20, 18 }, { 20, 19 } };
                    smerti++;
                    Sleep(1500);
                    break;
                }
                /*getKey(&a);*/
                if (a.direction == Direction::RIGHT || a.direction == Direction::LEFT) {
                    Sleep(125);
                }
                else {
                    Sleep(250);
                }
            }
        }

        if (visual == 3 && difficult == 1) {
            while (true) {
                /*getKey(&a);*/
                a.move();
                /*getKey(&a);*/
                a.ru_draw();
                /*getKey(&a);*/
                if (a.check()) {
                    ru_apple();
                    a.eat();
                    schet++;
                    SetConsoleCursorPosition(console, { 55, 12 });
                    printf("Ваш счет: %d \n", schet);
                }
                /*getKey(&a);*/
                if (a.crash()) {
                    SetConsoleCursorPosition(console, { 20, 10 });
                    printf("Game Over u loh");
                    a.snake = { { 20, 17 }, { 20, 18 }, { 20, 19 } };
                    smerti++;
                    Sleep(1500);
                    break;
                }
                if (a.samokill()) {
                    SetConsoleCursorPosition(console, { 20, 10 });
                    printf("ti v sebya vreZalsya ");
                    a.snake = { { 20, 17 }, { 20, 18 }, { 20, 19 } };
                    smerti++;
                    Sleep(1500);
                    break;
                }
                /*getKey(&a);*/
                if (a.direction == Direction::RIGHT || a.direction == Direction::LEFT) {
                    Sleep(125);
                }
                else {
                    Sleep(250);
                }
            }
        }

        if (visual == 1 && difficult == 2) {
            while (true) {
                /*getKey(&a);*/
                a.move();
                /*getKey(&a);*/
                a.def_draw();
                /*getKey(&a);*/
                if (a.check()) {
                    apple();
                    a.eat();
                    schet++;
                    SetConsoleCursorPosition(console, { 55, 12 });
                    printf("Ваш счет: %d \n", schet);
                }
                /*getKey(&a);*/
                if (a.crash()) {
                    SetConsoleCursorPosition(console, { 20, 10 });
                    printf("Game Over u loh");
                    a.snake = { { 20, 17 }, { 20, 18 }, { 20, 19 } };
                    smerti++;
                    Sleep(1500);
                    break;
                }
                if (a.samokill()) {
                    SetConsoleCursorPosition(console, { 20, 10 });
                    printf("ti v sebya vreZalsya ");
                    a.snake = { { 20, 17 }, { 20, 18 }, { 20, 19 } };
                    smerti++;
                    Sleep(1500);
                    break;
                }
                /*getKey(&a);*/
                if (a.direction == Direction::RIGHT || a.direction == Direction::LEFT) {
                    Sleep(75);
                }
                else {
                    Sleep(150);
                }
            }
        }

        if (visual == 2 && difficult == 2) {
            while (true) {
                /*getKey(&a);*/
                a.move();
                /*getKey(&a);*/
                a.rgb_draw();
                /*getKey(&a);*/
                if (a.check()) {
                    apple();
                    a.eat();
                    schet++;
                    SetConsoleCursorPosition(console, { 55, 12 });
                    printf("Ваш счет: %d \n", schet);
                }
                /*getKey(&a);*/
                if (a.crash()) {
                    SetConsoleCursorPosition(console, { 20, 10 });
                    printf("Game Over u loh");
                    a.snake = { { 20, 17 }, { 20, 18 }, { 20, 19 } };
                    smerti++;
                    Sleep(1500);
                    break;
                }
                if (a.samokill()) {
                    SetConsoleCursorPosition(console, { 20, 10 });
                    printf("ti v sebya vreZalsya ");
                    a.snake = { { 20, 17 }, { 20, 18 }, { 20, 19 } };
                    smerti++;
                    Sleep(1500);
                    break;
                }
                /*getKey(&a);*/
                if (a.direction == Direction::RIGHT || a.direction == Direction::LEFT) {
                    Sleep(75);
                }
                else {
                    Sleep(150);
                }
            }
        }

        if (visual == 3 && difficult == 2) {
            while (true) {
                /*getKey(&a);*/
                a.move();
                /*getKey(&a);*/
                a.ru_draw();
                /*getKey(&a);*/
                if (a.check()) {
                    apple();
                    a.eat();
                    schet++;
                    SetConsoleCursorPosition(console, { 55, 12 });
                    printf("Ваш счет: %d \n", schet);
                }
                /*getKey(&a);*/
                if (a.crash()) {
                    SetConsoleCursorPosition(console, { 20, 10 });
                    printf("Game Over u loh");
                    a.snake = { { 20, 17 }, { 20, 18 }, { 20, 19 } };
                    smerti++;
                    Sleep(1500);
                    break;
                }
                if (a.samokill()) {
                    SetConsoleCursorPosition(console, { 20, 10 });
                    printf("ti v sebya vreZalsya ");
                    a.snake = { { 20, 17 }, { 20, 18 }, { 20, 19 } };
                    smerti++;
                    Sleep(1500);
                    break;
                }
                /*getKey(&a);*/
                if (a.direction == Direction::RIGHT || a.direction == Direction::LEFT) {
                    Sleep(75);
                }
                else {
                    Sleep(150);
                }
            }
        }

        if (visual == 1 && difficult == 3) {
            while (true) {
                /*getKey(&a);*/
                a.move();
                /*getKey(&a);*/
                a.def_draw();
                /*getKey(&a);*/
                if (a.check()) {
                    apple();
                    a.eat();
                    schet++;
                    SetConsoleCursorPosition(console, { 55, 12 });
                    printf("Ваш счет: %d \n", schet);
                }
                /*getKey(&a);*/
                if (a.crash()) {
                    SetConsoleCursorPosition(console, { 20, 10 });
                    printf("Game Over u loh");
                    a.snake = { { 20, 17 }, { 20, 18 }, { 20, 19 } };
                    smerti++;
                    Sleep(1500);
                    break;
                }
                if (a.samokill()) {
                    SetConsoleCursorPosition(console, { 20, 10 });
                    printf("ti v sebya vreZalsya ");
                    a.snake = { { 20, 17 }, { 20, 18 }, { 20, 19 } };
                    smerti++;
                    Sleep(1500);
                    break;
                }
                /*getKey(&a);*/
                if (a.direction == Direction::RIGHT || a.direction == Direction::LEFT) {
                    Sleep(45);
                }
                else {
                    Sleep(90);
                }
            }
        }

        if (visual == 2 && difficult == 3) {
            while (true) {
                /*getKey(&a);*/
                a.move();
                /*getKey(&a);*/
                a.rgb_draw();
                /*getKey(&a);*/
                if (a.check()) {
                    apple();
                    a.eat();
                    schet++;
                    SetConsoleCursorPosition(console, { 55, 12 });
                    printf("Ваш счет: %d \n", schet);
                }
                /*getKey(&a);*/
                if (a.crash()) {
                    SetConsoleCursorPosition(console, { 20, 10 });
                    printf("Game Over u loh");
                    a.snake = { { 20, 17 }, { 20, 18 }, { 20, 19 } };
                    smerti++;
                    Sleep(1500);
                    break;
                }
                if (a.samokill()) {
                    SetConsoleCursorPosition(console, { 20, 10 });
                    printf("ti v sebya vreZalsya ");
                    a.snake = { { 20, 17 }, { 20, 18 }, { 20, 19 } };
                    smerti++;
                    Sleep(1500);
                    break;
                }
                /*getKey(&a);*/
                if (a.direction == Direction::RIGHT || a.direction == Direction::LEFT) {
                    Sleep(45);
                }
                else {
                    Sleep(90);
                }
            }
        }

        if (visual == 3 && difficult == 3) {
            while (true) {
                /*getKey(&a);*/
                a.move();
                /*getKey(&a);*/
                a.ru_draw();
                /*getKey(&a);*/
                if (a.check()) {
                    ru_apple();
                    a.eat();
                    schet++;
                    SetConsoleCursorPosition(console, { 55, 12 });
                    printf("Ваш счет: %d \n", schet);
                }
                /*getKey(&a);*/
                if (a.crash()) {
                    SetConsoleCursorPosition(console, { 20, 10 });
                    printf("Game Over u loh");
                    a.snake = { { 20, 17 }, { 20, 18 }, { 20, 19 } };
                    smerti++;
                    Sleep(1500);
                    break;
                }
                if (a.samokill()) {
                    SetConsoleCursorPosition(console, { 20, 10 });
                    printf("ti v sebya vreZalsya ");
                    a.snake = { { 20, 17 }, { 20, 18 }, { 20, 19 } };
                    smerti++;
                    Sleep(1500);
                    break;
                }
                /*getKey(&a);*/
                if (a.direction == Direction::RIGHT || a.direction == Direction::LEFT) {
                    Sleep(45);
                }
                else {
                    Sleep(90);
                }
            }
        }
    }




    // Сообщаем, что основной поток завершился
    isMainThreadRunning.store(false);

    // Ожидаем завершения второго потока
    secondThread.join();

    return 0;
}