#include <Windows.h>
#include <stdio.h>
#include <stdbool.h>
#include "console.h"

HANDLE hConsole;

void Initial(bool vis) {            // Ŀ�� ���� �Լ�
	CONSOLE_CURSOR_INFO csCursor;

	csCursor.bVisible = vis;
	csCursor.dwSize = 1;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hConsole, &csCursor);
}

void MoveCursor(int x, int y) {            // Ŀ�� �������ִ� �Լ�
	COORD coord;
	coord.X = x;
	coord.Y = y;

	SetConsoleCursorPosition(hConsole, coord);

}

void ClearScreen() {            // ��ũ���� ��������ä�� ��ȭ������ ������ִ� �Լ�
	int x, y;

	for (y = 0; y < HEIGHT; y++) {
		MoveCursor(0, y);
		for (x = 0; x < WIDTH; x++) {
			printf("%c", ' ');
		}
	}
}

void SetConsoleSize() {         // �ܼ� â ũ�⸦ ���� 
	system("mode con:lines=31 cols=101"); // lines=����, cols=����
}

void SetColor(int txt, int bg) { // txt: �ؽ�Ʈ ����, bg: ��� ����. ����ǥ�� ������ ColorSet ����.
	SetConsoleTextAttribute(stdhdl, txt + bg * 16);
}