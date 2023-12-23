
#include <iostream>
#include <thread>
#include <vector>
#include <stdio.h>
#include <Windows.h>

using namespace std;

int screenWidth = 80;				// Размер экрана консоли по X (столбцы)
int screenHeight = 30;			// Размер экрана консоли по Y (строки)
int fieldWidth = 12;				// Размер игрового поля по X (стобцы)
int fieldHeight = 18;				// Размер игрового поля по Y (строки)
int sizeOfFigure = 4;				// Размер фигур
int keyCount = 4;

wstring tetromino[7];

unsigned char* field = nullptr;

int rotate(int x, int y, int flipAngle) {
	int pi = 0;

	switch (flipAngle % sizeOfFigure) {
	case 0: // 0 градусов			// 0  1  2  3
		pi = y * sizeOfFigure + x;					// 4  5  6  7
		break;									// 8  9  10 11
														// 12 13 14 15

	case 1: // 90 градусов			// 12  8  4  0
		pi = 12 + y - (x * sizeOfFigure);		// 13  9  5  1
		break;										// 14 10  6  2
															// 15 11  7  3

	case 2: // 180 градусов			// 15 14 13 12
		pi = 15 - (y * sizeOfFigure) - x;		// 11 10  9  8
		break;										//  7  6  5  4
															//  3  2  1  0

	case 3: // 270 градусов			// 3  7  11  15
		pi = 3 - y + (x * sizeOfFigure);			// 2  6  10  14
		break;										// 1  5   9  13
	}														// 0  4   8  12

	return pi;
}

bool doesFigureFit(int tetrominoIndex, int rotation, int positionX, int positionY) {
	// Все ячейки поля > 0 заняты
	for (int px = 0; px < sizeOfFigure; ++px) {
		for (int py = 0; py < sizeOfFigure; ++py) {
			// Собрать индекс в единое целое
			int pi = rotate(px, py, rotation);

			// Получить индекс в поле
			int fi = (positionY + py) * fieldWidth + (positionX + px);

			// Проверка, что тест находится в границах
			// выход за границы не обязательно означает неудачу,
			// поскольку длинный вертикальный фрагмент может содержать ячейки,
			// которые лежат за пределами границы

			if (positionX + px >= 0 && positionX + px < fieldWidth) {
				if (positionY + py >= 0 && positionY + py < fieldHeight) {
					// Проверка на столкновение
					if (tetromino[tetrominoIndex][pi] != L'.' && field[fi] != 0) {
						return false; // Сбой при первом попадании
					}
				}
			}
		}
	}
	return true;
}

int main() {
	// Создание экранного буфера
	wchar_t* screen = new wchar_t[screenWidth * screenHeight];
	for (int screenIndex = 0; screenIndex < screenWidth * screenHeight; ++screenIndex) {
		screen[screenIndex] = L' ';
	}
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
																							0, NULL, CONSOLE_TEXTMODE_BUFFER,
																							NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	tetromino[0].append(L"..X...X...X...X."); // Фигуры размера 4x4
	tetromino[1].append(L"..X..XX...X.....");
	tetromino[2].append(L".....XX..XX.....");
	tetromino[3].append(L"..X..XX..X......");
	tetromino[4].append(L".X...XX...X.....");
	tetromino[5].append(L".X...X...XX.....");
	tetromino[6].append(L"..X...X..XX.....");

	field = new unsigned char[fieldWidth * fieldHeight]; // Создание буфера игрового поля
	for (int x = 0; x < fieldWidth; ++x) { // Границы поля
		for (int y = 0; y < fieldHeight; ++y) {
			field[y * fieldWidth + x] = (x == 0 || x == fieldWidth - 1 ||
																	 y == fieldHeight - 1) ? 9 : 0;
		}
	}

	// Логика игры
	bool key[4];
	int currentFigure = 0;
	int currentRotation = 0;
	int currentX = fieldWidth / 2;
	int currentY = 0;
	int speed = 20;
	int speedCount = 0;
	bool forceDown = false;
	bool rotateHold = true;
	int figureCount = 0;
	int score = 0;
	vector<int> lines;
	bool isGameRunning = true;

	while (isGameRunning) { // Основной цикл
		// Синхронизация
		this_thread::sleep_for(50ms); // Маленький шаг - 1 игровой тик
		++speedCount;
		forceDown = (speedCount == speed);

		// Ввод
		for (int keyIndex = 0; keyIndex < keyCount; ++keyIndex) {
			key[keyIndex] = ( 0x8000 & GetAsyncKeyState ( (unsigned char)
											("\x27\x25\x28Z"[keyIndex]) ) ) != 0;
		}

		// Логика игры

		// Считывание действий игрока
		currentX += (key[0] && doesFigureFit(currentFigure, currentRotation,
								 currentX + 1, currentY)) ? 1 : 0;
		currentX -= (key[1] && doesFigureFit(currentFigure, currentRotation,
								 currentX - 1, currentY)) ? 1 : 0;
		currentY += (key[2] && doesFigureFit(currentFigure, currentRotation,
								 currentX, currentY + 1)) ? 1 : 0;

		// Вращение фигуры
		if (key[3]) {
			currentRotation += (rotateHold && doesFigureFit(currentFigure,
				                  currentRotation + 1, currentX, currentY)) ? 1 : 0;
			rotateHold = false;
		} else {
			rotateHold = true;
		}

		if (forceDown) {
			// Изменение скорости игры каждые 50 фигур
			speedCount = 0;

			++figureCount;

			if (figureCount % 50 == 0 && speed >= 10) {
				--speed;
			}

			// Проверка, можно ли сдвинуть фигуру вниз
			if (doesFigureFit(currentFigure, currentRotation, currentX, currentY + 1)) {
				++currentY;
			} else {
				// Если нельзя, фиксируем деталь на месте
				for (int px = 0; px < sizeOfFigure; ++px)
					for (int py = 0; py < sizeOfFigure; ++py)
						if (tetromino[currentFigure][rotate(px, py, currentRotation)] != L'.')
								field[(currentY + py) * fieldWidth + (currentX + px)] =
								currentFigure + 1;

				// Проверка наличия линии
				for (int py = 0; py < sizeOfFigure; ++py) {
					if (currentY + py < fieldHeight - 1) {
						bool line = true;

						for (int px = 1; px < fieldWidth - 1; ++px) {
							line &= (field[(currentY + py) * fieldWidth + px]) != 0;
						}
						if (line) {
							// Удаление линии
							for (int px = 1; px < fieldWidth - 1; ++px) {
								field[(currentY + py) * fieldWidth + px] = 8;
							}
							lines.push_back(currentY + py);
						}
					}
				}
				score += 25;
				if (!lines.empty()) {
					score += (1 << lines.size()) * 100;
				}

				// Выбор новой фигуры
				currentX = fieldWidth / 2;
				currentY = 0;
				currentRotation = 0;
				currentFigure = rand() % 7;

				// Если фигура не проходит, игра окончена
				isGameRunning = doesFigureFit(currentFigure, currentRotation, currentX, currentY);
			}
		}

		// Вывод

		// Отрисовка поля
		for (int x = 0; x < fieldWidth; ++x) {
			for (int y = 0; y < fieldHeight; ++y) {
				screen[(y + 2) * screenWidth + (x + 2)] = L" ABCDEFG=#"[field[y * fieldWidth + x]];
			}
		}

		// Отрисовка текущей фигуры
		for (int px = 0; px < sizeOfFigure; ++px) {
			for (int py = 0; py < sizeOfFigure; ++py) {
				if (tetromino[currentFigure][rotate(px, py, currentRotation)] != L'.') {
						screen[(currentY + py + 2) * screenWidth + (currentX + px + 2)] =
						currentFigure + 65;
				}
			}
		}

		// Отрисовка счета
		swprintf_s(&screen[2 * screenWidth + fieldWidth + 6], 16, L"SCORE: %8d", score);

		// Анимация удаления строки
		if (!lines.empty()) {
			WriteConsoleOutputCharacter(hConsole, screen, screenWidth * screenHeight,
																	{ 0,0 }, &dwBytesWritten);
			this_thread::sleep_for(400ms);

			for (auto& v : lines) {
				for (int px = 1; px < fieldWidth - 1; ++px) {
					for (int py = v; py > 0; --py) {
						field[py * fieldWidth + px] = field[(py - 1) * fieldWidth + px];
					}
					field[px] = 0;
				}
			}
			lines.clear();
		}

		// Отрисовка кадра
		WriteConsoleOutputCharacter(hConsole, screen, screenWidth * screenHeight,
																{ 0,0 }, &dwBytesWritten);
	}

	CloseHandle(hConsole);

	cout << "Game Over!! Score: " << score << endl;

	system("pause");

	return 0;
}
