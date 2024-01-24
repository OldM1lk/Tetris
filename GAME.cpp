
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <ctime>
#include "windows.h"
#include <vector>

using namespace std;

bool gameOver = false;

const int blocksCount = 7;
const char blocksSprites[blocksCount]{ 'A', 'B', 'C', 'D', 'E', 'F', 'G' };

const char wallSprite = '#';

int score = 0;

vector< vector< vector<int> > > blocksShapes = {
  {
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
  },
  {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
  },
  {
    {0, 1, 0, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
  },
  {
    {0, 1, 0, 0},
    {0, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
  },
  {
    {0, 0, 0, 0},
    {0, 0, 1, 1},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
  },
  {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 0, 0},
  },
  {
    {0, 0, 0, 1},
    {0, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
  }
};

int generateBlock() {
  srand(time(NULL));
  return rand() % blocksCount;
}

vector<string> map{
 string("*******************"),
 string("                  *"),
 string("                  *"),
 string("                  *"),
 string("                  *"),
 string("                  *"),
 string("                  *"),
 string("                  *"),
 string("                  *"),
 string("                  *"),
 string("                  *"),
 string("                  *"),
 string("*******************"),
};


class block {
public:
  vector <vector<int> > shape;
  int type;
  int positionX;
  int positionY;
  char sprite;


  block(int positionX = 0, int positionY = 5) {
    this->type = generateBlock();
    this->shape = blocksShapes[type];
    this->positionX = positionX;
    this->positionY = positionY;
    this->sprite = blocksSprites[type];
  }
};

block realBlock;

bool canMoveBlockRight() {
  for (int x = realBlock.positionX + 1; x <= realBlock.positionX + 4; ++x) {
    for (int y = realBlock.positionY; y <= realBlock.positionY + 3; ++y) {
      if (y >= map.size() || x >= map[0].size() || y < 0 || x < 0) {
        continue;
      }
      if (map[y][x] != ' ' && realBlock.shape[y - (realBlock.positionY)][x - (realBlock.positionX + 1)] == 1) {
        return false;
      }
    }
  }
  return true;
}

bool canMoveBlockUp() {
  for (int x = realBlock.positionX; x <= realBlock.positionX + 3; ++x) {
    for (int y = realBlock.positionY + 2; y >= realBlock.positionY - 1; --y) {
      if (y >= map.size() || x >= map[0].size() || y < 0 || x < 0) {
        continue;
      }
      if (map[y][x] != ' ' && realBlock.shape[y - (realBlock.positionY) + 1][x - (realBlock.positionX)] == 1) {
        return false;
      }
    }
  }
  return true;
}

bool canFall() {
  for (int y = realBlock.positionY; y <= realBlock.positionY + 3; ++y) {
    for (int x = realBlock.positionX; x <= realBlock.positionX + 3; ++x) {
      if (y >= map.size() || x >= map[0].size() || y < 0 || x < 0) {
        continue;
      }
      if (realBlock.shape[y - realBlock.positionY][x - realBlock.positionX] == 1 && map[y + 1][x] != ' ') {
        return false;
      }
    }
  }
  return true;
}

void doFall() {
  ++realBlock.positionY;
}

void doMoveBlockUp() {
  --realBlock.positionY;
}

void doMoveBlockRight() {
  ++realBlock.positionX;
}

void clearMap() {
  for (int examRowIndex = 0; examRowIndex < map.size() - 1; ++examRowIndex) {
    int rowLength = map[examRowIndex].size();

    for (int columnIndex = 0; columnIndex < map[examRowIndex].size(); ++columnIndex) {
      if (map[examRowIndex][columnIndex] != ' ') {
        --rowLength;
      }
    }

    if (rowLength == 0) {
      ++score;
      for (int toClerrRowIndex = examRowIndex; toClerrRowIndex > 0; --toClerrRowIndex) {
        map[toClerrRowIndex] = map[toClerrRowIndex - 1];
      }
    }

  }
}

void logic() {
  if (canMoveBlockRight()) {
    doMoveBlockRight();
  } else {
    for (int rowIndex = 0; rowIndex < map.size(); ++rowIndex) {
      for (int columnIndex = 0; columnIndex < map[rowIndex].size(); ++columnIndex) {
        if (columnIndex >= realBlock.positionX && columnIndex <= realBlock.positionX + 3 && rowIndex >= realBlock.positionY && rowIndex <= realBlock.positionY + 3) {
          if (realBlock.shape[rowIndex - realBlock.positionY][columnIndex - realBlock.positionX] == 1) {
            map[rowIndex][columnIndex] = realBlock.sprite;
          }
        }
      }
    }

    clearMap();

    block a;
    realBlock = a;

    if (!canMoveBlockRight()) {
      gameOver = true;
    }
  }

}

void drawMap() {
  system("cls");

  for (int rowIndex = 0; rowIndex < map.size(); ++rowIndex) {

    for (int columnIndex = 0; columnIndex < map[rowIndex].size(); ++columnIndex) {
      if (columnIndex >= realBlock.positionX && columnIndex <= realBlock.positionX + 3 && rowIndex >= realBlock.positionY && rowIndex <= realBlock.positionY + 3) {
        if (realBlock.shape[rowIndex - realBlock.positionY][columnIndex - realBlock.positionX] == 1) {
          cout << realBlock.sprite;
        } else if (map[rowIndex][columnIndex] == '*') {
          cout << wallSprite;
        } else {
          cout << map[rowIndex][columnIndex];
        }

      } else if (map[rowIndex][columnIndex] == '*') {
        cout << wallSprite;
      } else {
        cout << map[rowIndex][columnIndex];
      }

    }
    cout << endl;

  }
};

void tryRotate() {
  vector <vector <int> > rotatedOne = realBlock.shape;
  for (int x = 0; x < realBlock.shape.size(); ++x) {
    for (int y = realBlock.shape[x].size() - 1; y >= 0; --y) {
      rotatedOne[x][y] = realBlock.shape[3 - y][x];
    }
  }

  for (int y = realBlock.positionY; y <= realBlock.positionY + 3; ++y) {
    for (int x = realBlock.positionX; x <= realBlock.positionX + 3; ++x) {
      if (y >= map.size() || x >= map[0].size() || y < 0 || x < 0) {
        continue;
      }
      if (rotatedOne[y - realBlock.positionY][x - realBlock.positionX] == 1 && map[y][x] != ' ') {
        return;
      }
    }
  }
  realBlock.shape = rotatedOne;
  drawMap();
};

void controls() {
  if (_kbhit()) {
    switch (_getch()) {
    case 'w':
    case 'W':
      if (canMoveBlockUp()) {
        doMoveBlockUp();
        drawMap();
      }
      break;
    case 'D':
    case 'd':
      if (canMoveBlockRight()) {
        doMoveBlockRight();
        drawMap();
      }
      break;
    case 's':
    case 'S':
      if (canFall()) {
        doFall();
        drawMap();
      }
      break;
    case 'A':
    case 'a':
      tryRotate();
      break;
    }
  }
}

int main() {
  int timer = time(NULL);

  while (!gameOver) {
    controls();

    if (time(NULL) - timer > 0) {
      logic();
      drawMap();

      timer = time(NULL);
    }

  }

  cout << "score: " << score << endl;
  system("pause");

  return 0;
}
