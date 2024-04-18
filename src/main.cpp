#include <GL/freeglut.h>
#include <math.h>

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "funciones.h"
using namespace std;
// caso 01

int n1 = 5;
vector<nodo> nodos1(n1);
vector<par> indice1(n1);
vector<par> indice2(n1);

vector<vector<int>> aristas1 = {
    {0, 1, 0, 1, 1}, {1, 0, 1, 0, 0}, {0, 1, 0, 1, 1}, {1, 0, 1, 0, 1}, {1, 0, 1, 1, 0}};
vector<vector<int>> positions = {
    {0, 0}, {0, 300}, {300, 300}, {300, 0}, {600, 150},
};
vector<vector<nodo>> restric;
void mas_restrictiva(vector<nodo> &nodos, vector<vector<int>> aristas, vector<par> indice, int n) {
  int result = 0;
  bool sw = true;
  vector<nodo> tmp = nodos;
  while (sw) {
    nodos = tmp;
    result += 1;
    sw = false;
    for (int i = 0; i < n; i++) {
      int temp = 1;
      int j = indice[i].index;
      nodos[j].color = temp;
      while (comprob(nodos, nodos[j], aristas[j], n)) {
        temp += 1;
        nodos[j].color = temp;
        if (temp > result) {
          sw = true;
          break;
        }
      }
      restric.push_back(nodos);
      if (sw) {
        restric.clear();
        break;
      }
    }
  }
  // print(nodos);
}
void mas_restringida(vector<nodo> &nodos, vector<vector<int>> aristas, vector<par> indice, int n) {
  int result = 0;
  bool sw = true;
  vector<nodo> tmp = nodos;
  vector<par> tmp2 = indice;
  while (sw) {
    nodos = tmp;
    indice = tmp2;
    result += 1;
    sw = false;
    int cant = 0;
    vector<int> fuera(n);
    while (cant++ < n) {
      int temp = 1;
      int j = 0;
      for (; j < n; j++) {
        if (!fuera[indice[j].index]) {
          j = indice[j].index;
          break;
        }
      }
      nodos[j].color = temp;
      while (comprob(nodos, nodos[j], aristas[j], n)) {
        temp += 1;
        nodos[j].color = temp;
        if (temp > result) {
          sw = true;
          break;
        }
      }
      restric.push_back(nodos);
      if (sw) {
        restric.clear();
        break;
      }
      fuera[j] = 1;
      var(indice, nodos, aristas, result, n);
    }
  }
  // print(nodos);
}

int windowWidth = 1600;
int windowHeight = 900;

// Botones
int button1X = 50;
int button1Y = windowHeight / 2 - 150;
int button1Width = windowWidth / 4;
int button1Height = 100;
int button2X = 50;
int button2Y = windowHeight / 2 + 50;
int button2Width = windowWidth / 4;
int button2Height = 100;
int button3X = 50;
int button3Y = 100;
int button3Width = 100;
int button3Height = 50;
const char *button3Text = "Caso 1";
int button4X = 200;
int button4Y = 100;
int button4Width = 100;
int button4Height = 50;
const char *button4Text = "Caso 2";

// Colores
float colors[6][3] = {{1.0f, 1.0f, 1.0f},   // Blanco
                      {0.0f, 1.0f, 0.0f},   // Verde
                      {1.0f, 0.0f, 0.0f},   // Rojo
                      {0.0f, 0.0f, 1.0f},   // Azul
                      {1.0f, 1.0f, 0.0f},   // Amarillo
                      {0.0f, 0.0f, 0.0f}};  // Negro
const char *button1Text = "+ Renstringida";
const char *button2Text = "+ Restrictiva";
string percentageInput = "";
void drawText(float x, float y, string text, float color[3]) {
  glColor3fv(color);
  glRasterPos2f(x, y);
  for (char &c : text) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
  }
}
void keyboard(unsigned char key, int x, int y) {
  if (key == 13) {
    percentageInput += "\n";
  } else if (key == 8) {
    if (!percentageInput.empty()) {
      percentageInput.pop_back();
    }
  } else {
    percentageInput += key;
  }
  glutPostRedisplay();
}
bool drawGraphC = false;
int nprocess = 0;
void timer(int value) {
  if (nprocess < n1 - 1)
    nprocess++;
  else
    drawGraphC = false;
  glutPostRedisplay();
}

void drawGraph1(vector<nodo> nodos, int n, int x, int y, int width, int height,
                vector<vector<int>> positions, vector<vector<int>> edges) {
  int radius = 60;
  int centerX = x + width / 2;
  int centerY = y + height / 3;

  glLineWidth(3);
  glColor3f(1.0f, 1.0f, 1.0f);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (edges[i][j]) {
        float posX1 = centerX + positions[i][0];
        float posY1 = centerY + positions[i][1];
        float posX2 = centerX + positions[j][0];
        float posY2 = centerY + positions[j][1];
        glBegin(GL_LINES);
        glVertex2f(posX1, posY1);
        glVertex2f(posX2, posY2);
        glEnd();
      }
    }
  }
  for (int i = 0; i < n; i++) {
    float posX = centerX + positions[i][0];
    float posY = centerY + positions[i][1];
    float color[3] = {colors[nodos[i].color][0], colors[nodos[i].color][1],
                      colors[nodos[i].color][2]};
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_POLYGON);
    for (int j = 0; j < 360; j++) {
      float degInRad = j * 3.1415 / 180;
      glVertex2f(posX + cos(degInRad) * radius, posY + sin(degInRad) * radius);
    }
    glEnd();
    stringstream ss;
    ss << i;
    drawText(posX - 5, posY + 5, ss.str(), colors[5]);
  }
}
void drawButton(int x, int y, int width, int height, const char *text, float color[3]) {
  glColor3f(color[0], color[1], color[2]);
  glBegin(GL_QUADS);
  glVertex2f(x, y);
  glVertex2f(x + width, y);
  glVertex2f(x + width, y + height);
  glVertex2f(x, y + height);
  glEnd();

  // Draw text
  glColor3f(0, 0, 0);
  int textWidth = strlen(text) * 10;
  int textX = x + (width - textWidth) / 2;
  int textY = y + height / 2;
  glRasterPos2f(textX, textY);
  for (const char *c = text; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
  }
}

void mouseClick(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    if (x >= button1X && x <= button1X + button1Width && y >= button1Y &&
        y <= button1Y + button1Height) {
      // + Restringida
      drawGraphC = true;
      contar2(aristas1, indice1, n1);
      sort(indice1.begin(), indice1.end(), compara);
      mas_restringida(nodos1, aristas1, indice2, n1);
      glutPostRedisplay();

    } else if (x >= button2X && x <= button2X + button2Width && y >= button2Y &&
               y <= button2Y + button2Height) {
      // + Restrictiva
      drawGraphC = true;
      contar2(aristas1, indice1, n1);
      sort(indice1.begin(), indice1.end(), compara);
      mas_restrictiva(nodos1, aristas1, indice1, n1);
      glutPostRedisplay();
    } else if (x >= button3X && x <= button3X + button3Width && y >= button3Y &&
               y <= button3Y + button3Height) {
      // Caso 1
      n1 = 5;
      nprocess = 0;
      nodos1 = vector<nodo>(n1);
      indice1 = vector<par>(n1);
      indice2 = vector<par>(n1);
      aristas1 = {
          {0, 1, 0, 1, 1}, {1, 0, 1, 0, 0}, {0, 1, 0, 1, 1}, {1, 0, 1, 0, 1}, {1, 0, 1, 1, 0}};
      positions = {
          {0, 0}, {0, 300}, {300, 300}, {300, 0}, {600, 150},
      };
      restric.clear();
    } else if (x >= button4X && x <= button4X + button4Width && y >= button4Y &&
               y <= button4Y + button4Height) {
      // Caso 2
      nprocess = 0;
      n1 = 7;
      nodos1 = vector<nodo>(n1);
      indice1 = vector<par>(n1);
      indice2 = vector<par>(n1);
      aristas1 = {{0, 1, 0, 1, 0, 0, 0}, {1, 0, 1, 0, 0, 0, 1}, {0, 1, 0, 1, 0, 1, 0},
                  {1, 0, 1, 0, 1, 1, 0}, {0, 0, 0, 1, 0, 1, 1}, {0, 0, 1, 1, 1, 0, 1},
                  {0, 1, 0, 0, 1, 1, 0}};
      positions.clear();
      positions = {{0, 0}, {0, 200}, {200, 200}, {200, 0}, {400, 0}, {400, 200}, {600, 100}};
      restric.clear();
    }
  }
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  drawButton(button1X, button1Y, button1Width, button1Height, button1Text, colors[0]);
  drawButton(button2X, button2Y, button2Width, button2Height, button2Text, colors[0]);
  drawButton(button3X, button3Y, button3Width, button3Height, button3Text, colors[0]);
  drawButton(button4X, button4Y, button4Width, button4Height, button4Text, colors[0]);
  if (drawGraphC) {
    drawGraph1(restric[nprocess], n1, windowWidth / 4, 0, windowWidth / 2, windowHeight, positions,
               aristas1);
    glutTimerFunc(1000, timer, 0);  // Espera un segundo
  } else {
    drawGraph1(nodos1, n1, windowWidth / 4, 0, windowWidth / 2, windowHeight, positions, aristas1);
  }
  glutSwapBuffers();
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(1600, 900);
  glutCreateWindow("Coloreo de Grafos");
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, 1600, 900, 0);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouseClick);
  glutMainLoop();
  return 0;
}