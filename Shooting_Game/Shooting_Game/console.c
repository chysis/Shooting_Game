#include <Windows.h>
#include <stdio.h>
#include "console.h"

HANDLE hConsole;

void Initial() {            // 커서 안보이게 해주는 함수
	CONSOLE_CURSOR_INFO csCursor;

	csCursor.bVisible = FALSE;
	csCursor.dwSize = 1;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hConsole, &csCursor);
}

void MoveCursor(int x, int y) {            // 커서 움직여주는 함수
	COORD coord;
	coord.X = x;
	coord.Y = y;

	SetConsoleCursorPosition(hConsole, coord);

}

void ClearScreen() {            // 스크린을 공백으로채워 빈화면으로 만들어주는 함수
	int x, y;

	for (y = 0; y < HEIGHT; y++) {
		MoveCursor(0, y);
		for (x = 0; x < WIDTH; x++) {
			printf("%c", ' ');
		}
	}
}

void SetConsoleSize() {         // 콘솔 창 크기를 조정 
	system("mode con:lines=31 cols=101"); // lines=세로, cols=가로
}

void SetColor(int txt, int bg) { // txt: 텍스트 색상, bg: 배경 색상. 색상표는 열거형 ColorSet 참고.
	SetConsoleTextAttribute(stdhdl, txt + bg * 16);
}