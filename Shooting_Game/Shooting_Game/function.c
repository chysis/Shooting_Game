/* Class Team Project */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#define stdhdl GetStdHandle(STD_OUTPUT_HANDLE)

#define title_cursor 40

int area = 0;   // 모드 스위치 (0: title, 1: Game Field, 2: Ranking, 3: How to Play) #TODO
int title_default_cur = 25;   // 타이틀 화면 커서 초기 위치

							  // 콘솔 색상 모음 (0~15)
enum colorset {
	Black = 0, Blue, Green, Cyan, Red, Magenta, Brown, Lightgray, Darkgray,
	Lightblue, Lightgreen, Lightcyan, Lightred, Lightmagenta, Yellow, White = 15
};

void gotoxy(short x, short y)
// 커서를 (x, y)좌표로 이동. 
{
	COORD pos = { x, y }; // x, y좌표
	SetConsoleCursorPosition(stdhdl, pos); // set cursor
}

void SetConsolesize()
// 콘솔 창 크기를 조정
{
	system("mode con:lines=50 cols=100"); // lines=세로, cols=가로
}

void HideCursor(bool visibility)
// 커서를 숨긴다.(visibility true: 보임, false: 안 보임)
{
	CONSOLE_CURSOR_INFO ConsoleCursor = { 0, };
	ConsoleCursor.dwSize = 1;
	ConsoleCursor.bVisible = visibility;
	SetConsoleCursorInfo(stdhdl, &ConsoleCursor);
}

void Setcolor(int txt, int bg)
// txt: 텍스트 색상, bg: 배경 색상. 색상표는 열거형 colorset 참고.
{
	SetConsoleTextAttribute(stdhdl, txt + bg * 16);
}

void DrawTitle()
// 게임 타이틀 화면 구성 (임시)
{
	int x = 15;
	int y = 10;
	gotoxy(x, y);

	printf("          ########         #         ##         ##   ##########"); gotoxy(x, y + 1);
	printf("         ##               # #        # #       # #   #         "); gotoxy(x, y + 2);
	printf("        ##               #   #       #  #     #  #   #         "); gotoxy(x, y + 3);
	printf("       ##    #####      #######      #   #   #   #   ##########"); gotoxy(x, y + 4);
	printf("        ##      ##     #       #     #   #   #   #   #         "); gotoxy(x, y + 5);
	printf("         ##     ##    #         #    #    # #    #   #         "); gotoxy(x, y + 6);
	printf("          ########   #           #   #     #     #   ##########"); gotoxy(x, y + 7);
}

void PrintMenu()
// 메뉴 출력
{
	int x = 45;
	int y = 25;
	gotoxy(x, y);
	Setcolor(Lightred, Black);
	printf("Game Start");

	gotoxy(x, y + 5);
	printf("Ranking");

	gotoxy(x, y + 10);
	printf("How to Play?");

	gotoxy(x, y + 15);
	printf("Exit");
	Setcolor(White, Black);
	gotoxy(0, 0);
}

void DeleteCursor(int d)
// 커서 제거
{
	gotoxy(title_cursor, d);
	printf("  ");
}

void ArrowCursor(int a)
// 메뉴 선택용 화살표 커서
{
	gotoxy(title_cursor, a);  // 초기 위치: Game Start
	printf(">>");
}

int Showcur()
// 전역변수 cur값 불러오기
{
	return title_default_cur;
}

void Updowncur(int num)
// 전역변수 cur값 조정
{
	title_default_cur += num;
}

void GetArrowKey()
// 타이틀 화면에서 방향키를 입력받아 커서를 움직인다.
{
	while (area == 0)  // 초기 화면에서만 동작 
	{
		int a = Showcur();  // 변경 이전의 커서값 저장
		int temp;
		temp = _getch();
		if (temp == 224)  // 방향키를 입력받았을 때
		{
			temp = _getch();
			if (temp == 72)  // 위 방향키
			{
				if (Showcur() != 25)
				{
					Updowncur(-5);
					DeleteCursor(a);
					ArrowCursor(Showcur());
					continue;
				}
			}
			else if (temp == 80)  // 아래 방향키
			{
				if (Showcur() != 40)
				{
					Updowncur(5);
					DeleteCursor(a);
					ArrowCursor(Showcur());
					continue;
				}
			}
		}
		else if (temp == 13) // 엔터키를 입력받았을 때
		{
			if (a == 25) // #TODO 게임 시작
			{
				area = 1;  // 초기화면에서 벗어남
				system("cls");
			}
			else if (a == 30)  // Ranking
			{
				// #TODO Ranking 기능
			}
			else if (a == 35)  // How to Play?
			{
				// #TODO How to Play 기능
			}
			else if (a == 40)  // Exit
			{
				exit(1);
			}
		}
	}
}

int main(void)
{
	HideCursor(0);  // 커서 숨기기
	SetConsolesize();  // 화면 크기 설정
	DrawTitle();
	PrintMenu();
	ArrowCursor(25);
	GetArrowKey();

	return 0;
}