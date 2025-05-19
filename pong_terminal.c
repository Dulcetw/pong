#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

#define DELAY 30000 // Controla la velocidad del juego

typedef struct {
    int x, y;
    int width, height;
} Paddle;

typedef struct {
    int x, y;
    int dir_x, dir_y;
} Ball;

// Dibuja un paddle
void draw_paddle(Paddle *p) {
    for (int i = 0; i < p->height; i++) {
        mvprintw(p->y + i, p->x, "||||");
    }
}

// Dibuja la pelota
void draw_ball(Ball *b) {
    mvprintw(b->y, b->x, "O");
}

// Lógica principal del juego
int main() {
    // Inicializar ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    // Dimensiones de la terminal
    int max_y = 0, max_x = 0;
    getmaxyx(stdscr, max_y, max_x);

    // Inicializar paddles y pelota
    Paddle left_paddle = {2, max_y / 2 - 2, 4, 4};
    Paddle right_paddle = {max_x - 6, max_y / 2 - 2, 4, 4};
    Ball ball = {max_x / 2, max_y / 2, 1, 1};

    int left_score = 0, right_score = 0;
    int ch;

    // Bucle principal del juego
    while (1) {
        clear();

        // Dibujar elementos
        draw_paddle(&left_paddle);
        draw_paddle(&right_paddle);
        draw_ball(&ball);

        // Dibujar marcador
        mvprintw(1, max_x / 2 - 5, "P1: %d - P2: %d", left_score, right_score);

        // Dibujar bordes
        for (int i = 0; i < max_x; i++) {
            mvprintw(0, i, "-");
            mvprintw(max_y - 1, i, "-");
        }

        // Mover pelota
        ball.x += ball.dir_x;
        ball.y += ball.dir_y;

        // Rebote en bordes superior e inferior
        if (ball.y <= 1 || ball.y >= max_y - 2) {
            ball.dir_y *= -1;
        }

        // Rebote en paddles
        if ((ball.x <= left_paddle.x + 4 && ball.y >= left_paddle.y && ball.y <= left_paddle.y + left_paddle.height) ||
            (ball.x >= right_paddle.x - 1 && ball.y >= right_paddle.y && ball.y <= right_paddle.y + right_paddle.height)) {
            ball.dir_x *= -1;
        }

        // Punto para P1 (la pelota sale por la derecha)
        if (ball.x >= max_x - 1) {
            left_score++;
            ball.x = max_x / 2;
            ball.y = max_y / 2;
            ball.dir_x *= -1;
        }

        // Punto para P2 (la pelota sale por la izquierda)
        if (ball.x <= 0) {
            right_score++;
            ball.x = max_x / 2;
            ball.y = max_y / 2;
            ball.dir_x *= -1;
        }

        // Control de paddles con teclado
        ch = getch();
        switch (ch) {
            case 'w': // P1: arriba
                if (left_paddle.y > 1) left_paddle.y--;
                break;
            case 's': // P1: abajo
                if (left_paddle.y < max_y - left_paddle.height - 1) left_paddle.y++;
                break;
            case KEY_UP: // P2: arriba
                if (right_paddle.y > 1) right_paddle.y--;
                break;
            case KEY_DOWN: // P2: abajo
                if (right_paddle.y < max_y - right_paddle.height - 1) right_paddle.y++;
                break;
            case 'q': // Salir del juego
                endwin();
                return 0;
        }

        refresh();
        usleep(DELAY); // Controla la velocidad del juego
    }

    endwin();
    return 0;
}
