#ifdef _WIN32
    #include "windows.h"
    #include "includes.h"
    #define PATH "../"
    // Windows
#else
    #include <GL/glut.h>
    #define PATH ""
    // Unix
#endif _WIN32

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include "GameOfLife.h"

using namespace std;

const string filename = "out.bmp";


static int randomInt(int left, int right) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(left, right);
    return dist(gen);
}

size_t mapSize = 100; // Размер карты по Х и Y. Можно менять от 4 до 1000
float mainScale = 10.0f / float(mapSize); // сохраним размер для дальнейшего использования при скролле мышкой
float scale = mainScale;
unsigned int chance = 50;

// Смещение камеры по Х и Y
float x_offset = 0.0f;
float y_offset = 0.0f;

// Прототипы
void display();
void reshape(GLsizei width, GLsizei height);
void keyboard(unsigned char c, int x, int y);
void simulation();


GameOfLife gameOfLife(mapSize, &chance);
// инициализация шанса на спавн по указателю на chance
// по этому будет достаточно поменять переменную chance
// без вызова метода setChance();

// Отрисовка карты по вектору карты
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //glScalef(0.01f + scale, 0.01f + scale, 0.01f + scale);
    glTranslatef(
        -float(mapSize) / 20 + x_offset -0.1f,
        float(mapSize) / 20 - y_offset,
        -float(mapSize) / 10 + scale
    );

    float posX = 0;
    float posY = 0;
    vector<vector<bool>>& temp = gameOfLife();

    for (auto& x : temp) {
        for (auto y : x) {
            glPushMatrix();
            glBegin(GL_QUADS);
                float color = float(int(!y));           // Преобразование true -> 0.0f и false -> 1.0f
                glColor3f(color, color, color);
                glVertex2f(0.0f + posX, 0.0f - posY);
                glVertex2f(0.1f + posX, 0.0f - posY);
                glVertex2f(0.1f + posX, 0.1f - posY);
                glVertex2f(0.0f + posX, 0.1f - posY);
            glEnd();
            glPopMatrix();
            posX += 0.1f;
        }
        posX = 0.0f;
        posY += 0.1f;
    }
    glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (GLdouble)width / (GLdouble)height, (GLdouble)0.01, (GLdouble)100.0f);
    glutPostRedisplay();
}

static void changeMap(size_t size) {
    mapSize = size;   

    if (size >= 0 && size <= 50) scale = 0;
    else scale = 15.0f / float(mapSize);

    gameOfLife.reInit(size);

    string rules = gameOfLife.getRules();
    glutSetWindowTitle(("Cave generator | " + rules).c_str());

    x_offset = 0.1f;
    y_offset = 0.1f;
}

static void changeMap() noexcept {
    gameOfLife.reInit(mapSize, chance);
    gameOfLife.setB(5, 8);
    gameOfLife.setS(4, 8);

    string rules = gameOfLife.getRules();
    glutSetWindowTitle(("Cave generator | " + rules).c_str());

    x_offset = +0.1f;
    y_offset = +0.1f;
}

void keyboard(unsigned char c, int x, int y) {
    if (c == '+' || c == '=') {
        scale += mainScale * 2;
    }
    else if (c == '-' || c == '_') {
        scale -= mainScale * 2;
    }
    else if (c == '[') {
        gameOfLife = GameOfLife(mapSize, chance);
        gameOfLife.setB(5, 8);
        gameOfLife.setS(4, 8);
        
        string rules = gameOfLife.getRules();
        glutSetWindowTitle(("Cave generator | " + rules).c_str());

        x_offset = +0.1f;
        y_offset = +0.1f;
    }
    else if (c == ']') {
        gameOfLife = GameOfLife(mapSize, chance);
        gameOfLife.setB(3, 3);
        gameOfLife.setS(2, 3);
        
        string rules = gameOfLife.getRules();
        glutSetWindowTitle(("Cave generator | " + rules).c_str());

        x_offset = +0.1f;
        y_offset = +0.1f;
    }
    else if (c == 'w') {
        y_offset += 0.1f;
    }
    else if (c == 's') {
        y_offset -= 0.1f;
    }
    else if (c == 'a') {
        x_offset += 0.1f;
    }
    else if (c == 'd') {
        x_offset -= 0.1f;
    }
    else if (c == 'x') {
        if (chance >= 99) return;
        chance += 1;
        changeMap();
    }
    else if (c == 'z') {
        if (chance <= 1) return;
        chance -= 1;
        changeMap();
    }
    else if (c == 'c') {
        gameOfLife.stepBack();
    }
    else if (c == ' ') {
        gameOfLife.life();
    }
    else if (c == 'n') {
        gameOfLife.reInit(mapSize);
        x_offset = +0.1f;
        y_offset = +0.1f;
    }
    else if (c == '\\') {
        gameOfLife.saveToBMP(PATH + filename);
    }
    else if (c == '1') {
        changeMap(10);
    }
    else if (c == '2') {
        changeMap(100);
    }
    else if (c == '3') {
        changeMap(200);
    }
    else if (c == '4') {
        changeMap(300);
    }
    else if (c == '5') {
        changeMap(500);
    }
    else if (c == '6') {
        changeMap(1000);
    }
    else if (c == '7') {
        mapSize = 200;
        scale = 15.0f / float(mapSize);
        gameOfLife.reInit(mapSize);
        gameOfLife.setB(3, 3);
        gameOfLife.setS(2, 3);

        string rules = gameOfLife.getRules();
        glutSetWindowTitle(("Cave generator | " + rules).c_str());
    }
    else if (c == 27) {
        glutDestroyWindow(glutGetWindow());
    }
    else if (c == 9) {
        for (int x = 0; x < 100; x++) {
            unsigned int sizeX = (unsigned int)(randomInt(0, mapSize - 2));
            unsigned int sizeY = (unsigned int)(randomInt(0, mapSize - 2));
            gameOfLife.fill(bool(randomInt(0, 1)), sizeX, sizeY, randomInt(0, mapSize/100));
        }
    }
}

static void specialKeyboard(int key, int mouseX, int mouseY) {
    switch (key) {
    case GLUT_KEY_F5:
        printf("pressed F5\n");
        gameOfLife.saveToBMP(PATH + filename);
        break;
    case GLUT_KEY_F6:
        printf("pressed F6\n");
        break;
    default:
        break;
    }
}



static void wheel(int dir, int pressed, int x, int y) noexcept {
    if (dir == 3) {
        scale += mainScale * 2;
    }
    else if (dir == 4) {
        scale -= mainScale * 2;
    }
}

static void mouse(int x, int y) noexcept {
    static int prev_x = x;
    static int prev_y = y;


    static float delta_x = 0;
    static float delta_y = 0;
    //cout << abs(prev_x) - abs(x) << " ";
    //cout << abs(prev_y) - abs(y) << "\n";
    if (abs(abs(prev_x) - abs(x)) > mapSize / 2) {
        prev_x = x;
        return;
    }
    if (abs(abs(prev_y) - abs(y)) > mapSize / 2) {
        prev_y = y;
        return;
    }

    delta_x = float(x - prev_x) / 50;
    delta_y = float(y - prev_y) / 50;

    //printf("DELTA: %f %f\t", delta_x, delta_y);
    //printf("size %d\n", mapSize);

    x_offset += delta_x;
    y_offset += delta_y;

    prev_x = x;
    prev_y = y;
    //glutPostRedisplay();
}

void simulation() {
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    setlocale(0, "");
    glutInit(&argc, argv);

    //gameOfLife.setBS({5, 6, 7, 8}, {4, 5, 6, 7, 8}); // можно и так
    //gameOfLife.setB({5,6,7,8}); // или так
    gameOfLife.setB(5, 8);
    gameOfLife.setS(4, 8);
    string rules = gameOfLife.getRules();

    GameOfLife::setThreadCount(0);          // 0 для количества потоков по системе

    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(50, 50);
    glutCreateWindow(("Cave generator | " + rules).c_str());

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(simulation);

    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);

    glutMouseFunc(wheel);
    glutMotionFunc(mouse);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutMainLoop();
    return 0;
}