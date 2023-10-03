#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

int kbhit();
char kbin[256];
void kbget();
bool key(char k) { return (kbin[0] == k && !kbin[1]); }
int inputxy(char ARR[][81]);
void drow(char ARR[][81]);
void remake(char ARRold[][81], char ARRnew[][81]);
int counter(char ARR[][81], int i, int j);
void terms(char ARRold[][81], char ARRnew[][81]);
void life(char ARRold[][81], char ARRnew[][81], int i, int j);
void dodo(char ARRold[][81], char ARRnew[][81], int i, int j, int status);

int main(void) {
    char ARRold[25][81];
    for (int i = 0; i < 25; i++) {
        sprintf(ARRold[i],
                "                                                       "
                "                         ");
    }
    if (inputxy(ARRold) == 0) {
        drow(ARRold);
        int delay1 = 20;
        do {
            kbget();
            if (key('w') || key('W')) {
                if (delay1 < 500) delay1 += 5;
            }
            if (key('e') || key('E')) {
                if (delay1 > 6) delay1 -= 5;
            }
            char ARRnew[25][81];
            remake(ARRold, ARRnew);
            usleep(delay1 * 10000);
            system("clear");
            terms(ARRold, ARRnew);
            printf("\033[45m\033[30m");
            drow(ARRnew);
            printf("\033[0m\033[91m");
            printf("\n Q - exit, W - speed down, E - speed up\n");
            printf("\033[0m");
            remake(ARRnew, ARRold);
        } while (!(key('q') || key('Q')));
    } else {
        printf("n/a");
    }

    return 0;
}
int kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;
    if (!initialized) {
        struct termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }
    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}
void kbget() {
    int tmp;
    kbin[0] = 0;
    if (!(tmp = kbhit())) return;
    int obp = 0;
    while (obp < tmp) {
        kbin[obp] = 0;
        kbin[obp] = getchar();
        if (kbin[obp] == 0) {
            break;
        }
        obp++;
    }
    kbin[obp] = 0;
}

int inputxy(char ARR[][81]) {
    char sym;
    int x, y, number, err = 0;
    FILE *file = NULL;
    printf("input the file number\n");
    if (scanf("%d%c", &number, &sym) == 2 && number > 0 && sym == '\n' && number < 6) {
        char *filename;
        if (number == 1) {
            filename = "game1.txt";
        } else if (number == 2) {
            filename = "game2.txt";
        } else if (number == 3) {
            filename = "game3.txt";
        } else if (number == 4) {
            filename = "game4.txt";
        } else {
            filename = "game5.txt";
        }
        file = fopen(filename, "r");
        if (file != NULL) {
            do {
                if (fscanf(file, "%d %d%c", &x, &y, &sym) == 3 &&
                    ((x >= 0 && x < 25 && y >= 0 && y < 80) || (x == 100 && y == 100))) {
                    if (sym == '\n') {
                        if (x != 100 && y != 100) {
                            ARR[x][y] = '+';
                        }
                    } else
                        err = 1;
                } else {
                    err = 1;
                    break;
                }
            } while (x != 100 && y != 100);
        } else {
            err = 1;
        }
        fclose(file);
    } else
        err = 1;
    return err;
}

void drow(char ARR[][81]) {
    for (int i = 0; i < 25; i++) {
        printf("%s\n", ARR[i]);
    }
}

void remake(char ARRold[][81], char ARRnew[][81]) {
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 81; j++) {
            ARRnew[i][j] = ARRold[i][j];
        }
    }
}

int counter(char ARR[][81], int i, int j) {
    int counter = 0;
    for (int x = i - 1; x < i + 2; x++) {
        for (int y = j - 1; y < j + 2; y++) {
            if (x == i && y == j) {
                continue;
            }
            int x1 = x, y1 = y;
            if (x < 0) {
                x1 = 24;
            }
            if (y < 0) {
                y1 = 79;
            }
            if (x >= 25) {
                x1 = 0;
            }
            if (y >= 80) {
                y1 = 0;
            }
            if (ARR[x1][y1] == '+') {
                counter++;
            }
        }
    }
    return counter;
}

void terms(char ARRold[][81], char ARRnew[][81]) {
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 80; j++) {
            life(ARRold, ARRnew, i, j);
        }
    }
}

void life(char ARRold[][81], char ARRnew[][81], int i, int j) {
    int status;
    status = counter(ARRold, i, j);
    dodo(ARRold, ARRnew, i, j, status);
}
void dodo(char ARRold[][81], char ARRnew[][81], int i, int j, int status) {
    if (ARRold[i][j] == '+') {
        if (status != 3 && status != 2) {
            ARRnew[i][j] = ' ';
        }
    } else if (status == 3) {
        ARRnew[i][j] = '+';
    }
}
