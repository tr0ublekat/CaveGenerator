#ifdef _WIN32
    #include "windows.h"
    #include "includes.h"
    // Windows
#else
    #include <GL/glut.h>
    // Unix
#endif _WIN32

#include <iostream>
#include <vector>
#include <random>
#include "GameOfLife.h"

using namespace std;

size_t mapSize = 200; // Размер карты по Х и Y. Можно менять от 4 до 1000
float scale = 10.0f / float(mapSize); // Не трогать
int chance = 53;

// Смещение камеры по Х и Y
float x_offset = 0.0f;
float y_offset = 0.0f;

// Прототипы
void display();
void reshape(GLsizei width, GLsizei height);
void keyboard(unsigned char c, int x, int y);
void simulation();


GameOfLife gameOfLife(mapSize, 50);


// Отрисовка карты по вектору карты
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glScalef(0.01f + scale, 0.01f + scale, 0.01f + scale);
    glTranslatef(-float(mapSize)/20 + x_offset, float(mapSize)/20 + y_offset, -float(mapSize)/20);

    float posX = 0;
    float posY = 0;
    vector<vector<bool>>& temp = gameOfLife();
    for (size_t i = 0; i < temp.size(); i++) {
        for (size_t j = 0; j < temp.size(); j++) {
            if (temp[i][j]) {
                glPushMatrix();
                glBegin(GL_QUADS);
                glColor3f(0.0f, 0.7f, 0.0f);
                glVertex2f(0.0f+posX, 0.0f-posY);
                glVertex2f(0.1f+posX, 0.0f-posY);
                glVertex2f(0.1f+posX, 0.1f-posY);
                glVertex2f(0.0f+posX, 0.1f-posY);
                glEnd();
                glPopMatrix();
            }
            else {
                glPushMatrix();
                glBegin(GL_QUADS);
                glColor3f(1.0f, 1.0f, 1.0f);
                glVertex2f(0.0f+posX, 0.0f-posY);
                glVertex2f(0.1f+posX, 0.0f-posY);
                glVertex2f(0.1f+posX, 0.1f-posY);
                glVertex2f(0.0f+posX, 0.1f-posY);
                glEnd();
                glPopMatrix();
            }

            posX += 0.1f;
        }
        posX = 0.0f;
        posY += 0.1f;
    }

    glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1;
    GLfloat aspect = (GLfloat) width / (GLfloat) height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (width >= height) {
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }
}

void keyboard(unsigned char c, int x, int y) {
    if (c == '+') {
        scale += 0.005f;
        if (scale >= 10) scale = 10;
    }
    else if (c == '-') {
        scale -= 0.005f;
        if (scale <= 0.005f) scale = 0.005f;
    }
    else if (c == 'w') {
        y_offset -= 0.1f;
    }
    else if (c == 's') {
        y_offset += 0.1f;
    }
    else if (c == 'a') {
        x_offset += 0.1f;
    }
    else if (c == 'd') {
        x_offset -= 0.1f;
    }
    else if (c == ' ') {
        gameOfLife.life();
    }
    else if (c == 'n') {
        gameOfLife.init();
    }
    else if (c == '\\') {
        gameOfLife.deserialization();
    }
    else if (c == '1') {
        mapSize = 20;
        scale = 10.0f / float(mapSize);
        gameOfLife = GameOfLife(mapSize, chance);
        gameOfLife.setB(5, 8);
        gameOfLife.setS(4, 8);

    }
    else if (c == '2') {
        mapSize = 100;
        scale = 10.0f / float(mapSize);
        gameOfLife = GameOfLife(mapSize, chance);
        gameOfLife.setB(5, 8);
        gameOfLife.setS(4, 8);
    }
    else if (c == '3') {
        mapSize = 200;
        scale = 10.0f / float(mapSize);
        gameOfLife = GameOfLife(mapSize, chance);
        gameOfLife.setB(5, 8);
        gameOfLife.setS(4, 8);
    }
    else if (c == '4') {
        mapSize = 300;
        scale = 10.0f / float(mapSize);
        gameOfLife = GameOfLife(mapSize, chance);
        gameOfLife.setB(5, 8);
        gameOfLife.setS(4, 8);
    }
    else if (c == '5') {
        mapSize = 500;
        scale = 10.0f / float(mapSize);
        gameOfLife = GameOfLife(mapSize, chance);
        gameOfLife.setB(5, 8);
        gameOfLife.setS(4, 8);
    }
    else if (c == '6') {
        mapSize = 1000;
        scale = 10.0f / float(mapSize);
        gameOfLife = GameOfLife(mapSize, 50);
        gameOfLife.setB(5, 8);
        gameOfLife.setS(4, 8);
    }
}

void simulation() {
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    //gameOfLife.setBS({5, 6, 7, 8}, {4, 5, 6, 7, 8}); // можно и так
    //gameOfLife.setB({5,6,7,8}); // или так
    gameOfLife.setB(5, 8);
    gameOfLife.setS(4, 8);

    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(1366, 780);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Cave generator");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(simulation);
    glutKeyboardFunc(keyboard);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutMainLoop();
    return 0;
}