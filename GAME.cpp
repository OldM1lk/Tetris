
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <ctime>
#include "windows.h"
#include <vector>

using namespace std;

bool isGameRunning = true;

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
  for (int delColumnIndex = 0; delColumnIndex < map[0].size() - 1; ++delColumnIndex) {
    int columnLength = map.size();

    for (int rowIndex = 0; rowIndex < map.size(); ++rowIndex) {
      if (map[rowIndex][delColumnIndex] != ' ') {
        --columnLength;
      }
    }

    if (columnLength == 0) {
      ++score;

      for (int toClearColumnIndex = delColumnIndex; toClearColumnIndex > 0; --toClearColumnIndex) {
        for (int toClearRowIndex = 0; toClearRowIndex < map.size(); ++toClearRowIndex) {
          map[toClearRowIndex][toClearColumnIndex] = map[toClearRowIndex][toClearColumnIndex - 1];
        }
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
      isGameRunning = false;
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
    case 'W':
    case 'w':
    case 72:
      if (canMoveBlockUp()) {
        doMoveBlockUp();
        drawMap();
      }
      break;
    case 'D':
    case 'd':
    case 77:
      if (canMoveBlockRight()) {
        doMoveBlockRight();
        drawMap();
      }
      break;
    case 'S':
    case 's':
    case 80:
      if (canFall()) {
        doFall();
        drawMap();
      }
      break;
    case 'A':
    case 'a':
    case 75:
      tryRotate();
      break;
    }
  }
}

int main() {
  int timer = time(NULL);

  while (isGameRunning) {
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
