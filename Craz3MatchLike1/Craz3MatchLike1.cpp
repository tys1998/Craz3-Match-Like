#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <graphics.h>

#define KEY_DOWN(vk_c) (GetAsyncKeyState(vk_c) & 0x8000)

time_t initialTime = time(NULL);
time_t currentTime;
time_t leftTime;

typedef struct POSITION {
    int x;
    int y;
} POS;

POS cur;

int cnt;
POS balls[180];

int score = 0;

COLORREF color[6] = { RGB(34, 139, 34), // DeepGreen
RGB(153, 50, 204), // Pruple
RGB(0, 255, 127), // Green
RGB(30, 144, 255), // Blue
RGB(255, 255, 0), // Yellow
RGB(255, 99, 71) // Red
        };

COLORREF white = RGB(255, 248, 220); // Cornsilk
COLORREF black = RGB(0, 0, 0); // Black
COLORREF gold = RGB(255, 215, 0); // Gold
COLORREF red = RGB(255, 20, 20); // Red
COLORREF grey = RGB(50, 50, 50); // Grey

void init(void);
void gamebegin(void);
void gameplay(void);
void close(void);

void drawcursor(COLORREF);
void drawtime(int, COLORREF);
void drawscore(int);
void getSameColorBalls(void);
void ballsFallDown(void);

int getSameColorBall_up(POS, COLORREF);
int getSameColorBall_down(POS, COLORREF);
int getSameColorBall_left(POS, COLORREF);
int getSameColorBall_right(POS, COLORREF);

int main(void) {
    init();
    gamebegin();
    gameplay();
    close();
    return 0;
}

void init(void) {
    initgraph(1000, 700);
}

void gamebegin(void) {
    setlinecolor(grey);
    setlinestyle(PS_SOLID, 10);
    rectangle(255, 45, 745, 655);

    setlinestyle(PS_SOLID, 1);
    srand((unsigned) time(NULL));
    for (int x = 260 + 20; x <= 740 - 20; x += 40) {
        for (int y = 50 + 20; y <= 650 - 20; y += 40) {
            setfillcolor(color[rand() % 6]);
            solidcircle(x, y, 18);
        }
    }

    cur.x = 280 + 40 * 5;
    cur.y = 70 + 40 * 7;
    drawcursor(white);

    settextstyle(22, 0, _T("YaHei Consolas Hybrid"));
    currentTime = time(NULL);
    leftTime = 90 - (int) difftime(currentTime, initialTime);
    drawtime(leftTime, gold);
    drawscore(score);
}

void gameplay(void) {
    while (1) {
        // change time
        currentTime = time(NULL);
        leftTime = 90 - (int) difftime(currentTime, initialTime);
        if (leftTime == 9) {
            drawtime(10, black);
        }
        drawtime(leftTime, gold);
        if (leftTime == 0) {
            break;
        }
        // move cursor
        // 1. set the old one to black
        // 2. draw a new white one in the new position
        if (KEY_DOWN(VK_UP) && cur.y > 50 + 20) {
            drawcursor(black);
            cur.y -= 40;
            drawcursor(white);
        }
        if (KEY_DOWN(VK_DOWN) && cur.y < 650 - 20) {
            drawcursor(black);
            cur.y += 40;
            drawcursor(white);
        }
        if (KEY_DOWN(VK_LEFT) && cur.x > 260 + 20) {
            drawcursor(black);
            cur.x -= 40;
            drawcursor(white);
        }
        if (KEY_DOWN(VK_RIGHT) && cur.x < 740 - 20) {
            drawcursor(black);
            cur.x += 40;
            drawcursor(white);
        }
        if (KEY_DOWN(VK_RETURN) || KEY_DOWN(VK_SPACE)) {
            // find the nearby balls with the same color
            getSameColorBalls();
            if (cnt > 1) {
                // set these balls to black (disappear)
                setfillcolor(black);
                for (int i = 0; i <= cnt - 1; i++) {
                    solidcircle(balls[i].x, balls[i].y, 18);
                }
                Sleep(180);
                // fall down & draw new balls
                ballsFallDown();
                Sleep(180);
                // change score
                score += cnt;
                drawscore(score);
            }
        }
        Sleep(100);
        // Sleep --> ms
        // 100 ms = 0.1 s
    }
}

void close() {
    setfillcolor(black);
    solidrectangle(0, 0, 1000, 700);

    char str1[] = "GAME OVER!!";

    settextstyle(150, 0, _T("YaHei Consolas Hybrid"));
    settextcolor(gold);
    outtextxy(100, 250, str1);

    char str2[] = "press \"q\" to quit";
    settextstyle(20, 0, _T("consale"));
    settextcolor(white);
    outtextxy(800, 650, str2);

    while (1) {
        if (getch() == 'q') {
            closegraph();
            break;
        }
    }
}

void drawcursor(COLORREF cl) {
    setlinecolor(cl);
    rectangle(cur.x - 20, cur.y - 20, cur.x + 20, cur.y + 20);
}

void drawtime(int sec, COLORREF cl) {
    char str[30];
    sprintf(str, "Time Left : %d s", sec);

    settextcolor(cl);
    outtextxy(40, 90, str);

}

void drawscore(int score) {
    char str[30];
    sprintf(str, "Score : %d", score);

    settextcolor(red);
    outtextxy(40, 700 - 90, str);
}

void getSameColorBalls(void) {
    int i;
    for (i = 0; i <= 179; i++) {
        balls[i].x = 0;
        balls[i].y = 0;
    }

    cnt = 0;

    COLORREF cl = getpixel(cur.x, cur.y);

    balls[cnt] = cur;
    cnt++;

    for (i = 0; balls[i].x != 0; i++) {
        if (balls[i].y > 50 + 20) {
            getSameColorBall_up(balls[i], cl);
        }
        if (balls[i].y < 650 - 20) {
            getSameColorBall_down(balls[i], cl);
        }
        if (balls[i].x > 260 + 20) {
            getSameColorBall_left(balls[i], cl);
        }
        if (balls[i].x < 740 - 20) {
            getSameColorBall_right(balls[i], cl);
        }
    }
}

int getSameColorBall_up(POS tempPos, COLORREF cl) {
    tempPos.y -= 40;
    if (getpixel(tempPos.x, tempPos.y) == cl) {
        int j;
        for (j = 0; balls[j].x != 0; j++) {
            if (tempPos.x == balls[j].x && tempPos.y == balls[j].y) {
                return 0;
            }
        }
        balls[j] = tempPos;
        cnt++;
    }
    return 0;
}

int getSameColorBall_down(POS tempPos, COLORREF cl) {
    tempPos.y += 40;
    if (getpixel(tempPos.x, tempPos.y) == cl) {
        int j;
        for (j = 0; balls[j].x != 0; j++) {
            if (tempPos.x == balls[j].x && tempPos.y == balls[j].y) {
                return 0;
            }
        }
        balls[j] = tempPos;
        cnt++;
    }
    return 0;
}

int getSameColorBall_left(POS tempPos, COLORREF cl) {
    tempPos.x -= 40;
    if (getpixel(tempPos.x, tempPos.y) == cl) {
        int j;
        for (j = 0; balls[j].x != 0; j++) {
            if (tempPos.x == balls[j].x && tempPos.y == balls[j].y) {
                return 0;
            }
        }
        balls[j] = tempPos;
        cnt++;
    }
    return 0;
}

int getSameColorBall_right(POS tempPos, COLORREF cl) {
    tempPos.x += 40;
    if (getpixel(tempPos.x, tempPos.y) == cl) {
        int j;
        for (j = 0; balls[j].x != 0; j++) {
            if (tempPos.x == balls[j].x && tempPos.y == balls[j].y) {
                return 0;
            }
        }
        balls[j] = tempPos;
        cnt++;
    }
    return 0;
}

void ballsFallDown(void) {
    int tempX, tempY;
    // sort balls
    for (int step = 1; step <= cnt - 1; step++) {
        for (int i = 0; i <= cnt - step - 1; i++) {
            // the top one first
            if (balls[i].y > balls[i + 1].y) {
                tempX = balls[i].x;
                balls[i].x = balls[i + 1].x;
                balls[i + 1].x = tempX;
                tempY = balls[i].y;
                balls[i].y = balls[i + 1].y;
                balls[i + 1].y = tempY;
            }
        }
    }
    // fall down one by one
    for (int i = 0; i <= cnt - 1; i++) {
        for (int y = balls[i].y; y > 50 + 20; y -= 40) {
            setfillcolor(getpixel(balls[i].x, y - 40));
            solidcircle(balls[i].x, y, 18);
        }
        // set the top one a new one
        setfillcolor(color[rand() % 6]);
        solidcircle(balls[i].x, 50 + 20, 18);
    }
}
