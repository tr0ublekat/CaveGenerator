#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

unsigned int mapSize = 20; // Размер карты по Х и Y. Можно менять от 4 до 1000
float scale = 10.0f / float(mapSize); // Не трогать

// Смещение камеры по Х и Y
float x_offset = 0.0f;
float y_offset = 0.0f;

// Карта
vector<vector<bool>> map;

void startApp() {
    mapSize += 2;
}

// Пересоздание карты
void init() {
    map.clear();
    for (int i = 0; i < mapSize; ++i) {
        vector<bool> row;
        for (int j = 0; j < mapSize; ++j) {
            if ((i == 0) || (j == 0) || (i == mapSize-1) || (j == mapSize-1)) {
                bool value = false;
                row.push_back(value);
            }
            else {
                bool value = (rand() % 2 == 0);
                row.push_back(value);
            }
        }
        map.push_back(row);
    }
}

// Логика рождения и умирания клеток
void next_iteration() {
    init(); // Тест на нажатие кнопки
}

// Получить кол-во соседей клетки
int getNeighbourCount(int i, int j) {
    uint count = 0;
    if (map[i-1][j-1]) ++count;
    if (map[i][j-1]) ++count;
    if (map[i+1][j-1]) ++count;
    if (map[i+1][j]) ++count;
    if (map[i+1][j+1]) ++count;
    if (map[i][j+1]) ++count;
    if (map[i-1][j+1]) ++count;
    if (map[i-1][j]) ++count;

    return count;
}

// Отрисовка карты по вектору карты
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glScalef(0.01f + scale, 0.01f + scale, 0.01f + scale);
    glTranslatef(-float(mapSize)/20 + x_offset, float(mapSize)/20 + y_offset, -float(mapSize)/20);

    float posX = 0;
    float posY = 0;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map.size(); j++) {
            if (map[i][j]) {
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

    cout << getNeighbourCount(1, 2) << endl;

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
        scale += 0.005;
        if (scale >= 10) scale = 10;
    }
    if (c == '-') {
        scale -= 0.005;
        if (scale <= 0.005) scale = 0.005;
    }
    if (c == 'w') {
        y_offset -= 0.1;
    }
    if (c == 's') {
        y_offset += 0.1;
    }
    if (c == 'a') {
        x_offset += 0.1;
    }
    if (c == 'd') {
        x_offset -= 0.1;
    }
    if (c == ' ') {
        next_iteration();
    }
    cout << scale << endl;
}

void simulation() {
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    startApp();

    init();

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