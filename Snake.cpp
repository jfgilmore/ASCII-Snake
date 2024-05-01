// Snake.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>

using namespace std;

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Position {
    int x;
    int y;

    Position() : x(0), y(0) {}

    Position(int x, int y) : x(x), y(y) {}
};

class Snake {
public:
    Snake() {
        positions.push_back(Position(4, 2));
        positions.push_back(Position(3, 2));
        positions.push_back(Position(2, 2));
        direction = RIGHT;
    }

    void move() {
        Position head = positions.front();
        switch (direction) {
        case UP:
            head.y--;
            break;
        case DOWN:
            head.y++;
            break;
        case LEFT:
            head.x--;
            break;
        case RIGHT:
            head.x++;
            break;
        }

        positions.insert(positions.begin(), head);
        if (!ate_food) {
            positions.pop_back();
        }
        else {
            ate_food = false;
        }
    }

    void change_direction(Direction new_direction) {
        if (direction == UP && new_direction == DOWN ||
            direction == DOWN && new_direction == UP ||
            direction == LEFT && new_direction == RIGHT ||
            direction == RIGHT && new_direction == LEFT) {
            return;
        }

        direction = new_direction;
    }

    bool is_dead(int WIDTH, int HEIGHT) {
        Position head = positions.front();
        for (int i = 1; i < positions.size(); i++) {
            if (head.x == positions[i].x && head.y == positions[i].y) {
                return true;
            }
        }

        if (head.x <= 0 || head.x >= WIDTH || head.y <= 0 || head.y >= HEIGHT) {
            return true;
        }

        return false;
    }

    bool has_eaten_food(Position food) {
        Position head = positions.front();
        return head.x == food.x && head.y == food.y;
    }

    void set_ate_food(bool value) {
        ate_food = value;
    }

    int size() {
        return positions.size();
    }

    vector<Position> get_positions() {
        return positions;
    }

private:
    vector<Position> positions;
    Direction direction;
    bool ate_food = false;
};

class Food {
public:
    Food() {}

    Food(int WIDTH, int HEIGHT) {
        generate_new_position(WIDTH, HEIGHT);
    }

    Position get_position() {
        return position;
    }

    void generate_new_position(int WIDTH, int HEIGHT) {
        position.x = rand() % (WIDTH - 2) + 1;
        position.y = rand() % (HEIGHT - 2) + 1;
    }

private:
    Position position;
};

class Game {
public:
    Game() {
        init_console();
        init_game();
    }

    void run() {
        while (!snake.is_dead(WIDTH, HEIGHT)) {
            draw();
            handle_input();
            snake.move();
            if (snake.has_eaten_food(food.get_position())) {
                snake.set_ate_food(true);
                food.generate_new_position(WIDTH, HEIGHT);
            }
            Sleep(100 - (int)(snake.size() / 10));
        }

        end_game(snake.size() - 3);
    }

private:
    void init_console() {
        system("mode con cols=40 lines=25");
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursor_info;
        cursor_info.dwSize = 100;
        cursor_info.bVisible = FALSE;
        SetConsoleCursorInfo(hConsole, &cursor_info);
    }

    void init_game() {
        snake = Snake();
        food = Food(WIDTH, HEIGHT);
    }

    void draw() {
        system("cls");
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                    cout << "#";
                }
                else {
                    bool is_snake = false;
                    for (Position position : snake.get_positions()) {
                        if (position.x == j && position.y == i) {
                            is_snake = true;
                            break;
                        }
                    }

                    if (is_snake) {
                        cout << "O";
                    }
                    else if (food.get_position().x == j && food.get_position().y == i) {
                        cout << "$";
                    }
                    else {
                        cout << " ";
                    }
                }
            }
            cout << endl;
        }
    }

    void handle_input() {
        if (_kbhit()) {
            switch (_getch()) {
            case 'w':
                snake.change_direction(UP);
                break;
            case 's':
                snake.change_direction(DOWN);
                break;
            case 'a':
                snake.change_direction(LEFT);
                break;
            case 'd':
                snake.change_direction(RIGHT);
                break;
            }
        }
    }

    void end_game(int score) {
        cout << "Game over!" << endl;
        printf("Score: %i\n", score);
        system("pause");
    }

    const int WIDTH = 40;
    const int HEIGHT = 25;
    Snake snake;
    Food food;
};

int main() {
    Game game;
    game.run();

    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
