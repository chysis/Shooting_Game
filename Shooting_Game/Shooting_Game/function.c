/* Class Team Project */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#define stdhdl GetStdHandle(STD_OUTPUT_HANDLE)

#define title_cursor 40                // 메인 화면 커서의 x좌표 값

/* 이곳에서 모든 함수를 확인하세요 */

// 콘솔 관리 함수

void Gotoxy(short x, short y);         // 커서 이동 함수
void SetConsoleSize();                 // 콘솔창 크기 조절 함수
void HideCursor(bool visibility);      // 커서 숨기는 함수
void SetColor(int txt, int bg);        // 커서 색상, 배경 색상 지정 함수

// 메인 메뉴 관련 함수

void DrawTitle();                      // 타이틀 화면 출력 함수
void PrintMenu();                      // 메인 메뉴 출력 함수
void DeleteCursor(int d);              // 메인 화면 '>>' 커서 지우는 함수
void ArrowCursor(int a);               // 메인 화면 '>>' 커서 출력 함수
int ShowCur();                         // 메인 화면의 '>>' 커서 위치 불러오기
void UpdownCur(int num);               // 메인 화면 커서 이동 함수
void GetArrowKey(); 

// 게임 구현 함수

void HowToPlay();                      // 플레이 방법 출력 함수
void SelectPlaneUI();                  // 플레이어 유닛 선택 UI 출력 함수
void SelectPlane();                    // 플레이어 유닛 선택 함수

// 랭킹 구현 함수

void RankingUI();                      // 랭킹 화면 UI 출력 함수

// 전역 변수, 열거형, 구조체

int area = 0;   // 모드 스위치 (0: title, 1: How to Play, 2: Game Field, 3: Ranking) #TODO
int title_default_cur = 25;   // 타이틀 화면 커서 초기 위치.
						  
enum eColorSet { // 콘솔 색상 모음 (0~15)
	Black = 0, Blue, Green, Cyan, Red, Magenta, Brown, Lightgray, Darkgray,
	Lightblue, Lightgreen, Lightcyan, Lightred, Lightmagenta, Yellow, White = 15
};

/* 구현 */

void Gotoxy(short x, short y)
// 커서를 (x, y)좌표로 이동. 
{
	COORD pos = { x, y }; // x, y좌표
	SetConsoleCursorPosition(stdhdl, pos); // set cursor
}

void SetConsoleSize()
// 콘솔 창 크기를 조정
{
	system("mode con:lines=40 cols=100"); // lines=세로, cols=가로
}

void HideCursor(bool visibility)
// 커서를 숨긴다. (visibility true: 보임, false: 안 보임)
{
	CONSOLE_CURSOR_INFO ConsoleCursor = { 0, };
	ConsoleCursor.dwSize = 1;
	ConsoleCursor.bVisible = visibility;
	SetConsoleCursorInfo(stdhdl, &ConsoleCursor);
}

void SetColor(int txt, int bg)
// txt: 텍스트 색상, bg: 배경 색상. 색상표는 열거형 eColorSet 참고.
{
	SetConsoleTextAttribute(stdhdl, txt + bg * 16);
}

void DrawTitle()
// 게임 타이틀 화면 구성 (임시)
{
	int x = 15;
	int y = 10;
	Gotoxy(x, y);

	printf("          ########         #         ##         ##   ##########"); Gotoxy(x, y + 1);
	printf("         ##               # #        # #       # #   #         "); Gotoxy(x, y + 2);
	printf("        ##               #   #       #  #     #  #   #         "); Gotoxy(x, y + 3);
	printf("       ##    #####      #######      #   #   #   #   ##########"); Gotoxy(x, y + 4);
	printf("        ##      ##     #       #     #   #   #   #   #         "); Gotoxy(x, y + 5);
	printf("         ##     ##    #         #    #    # #    #   #         "); Gotoxy(x, y + 6);
	printf("          ########   #           #   #     #     #   ##########"); Gotoxy(x, y + 7);
}

void PrintMenu()
// 메뉴 출력
{
	int x = 45;
	int y = 25;
	Gotoxy(x, y);
	SetColor(Lightred, Black);
	printf("Game Start");

	Gotoxy(x, y + 5);
	printf("Ranking");

	Gotoxy(x, y + 10);
	printf("Exit");

	SetColor(White, Black);
	Gotoxy(0, 0);
}

void DeleteCursor(int d)
// 커서 제거
{
	Gotoxy(title_cursor, d);
	printf("  ");
}

void ArrowCursor(int a)
// 메뉴 선택용 화살표 커서
{
	Gotoxy(title_cursor, a);  // 초기 위치: Game Start
	printf(">>");
}

int ShowCur()
// 메인 화면의 >> 커서 값 불러오기
{
	return title_default_cur;
}

void UpdownCur(int num)
// 전역변수 cur값 조정
{
	title_default_cur += num;
}

void GetArrowKey()
// 타이틀 화면에서 방향키를 입력받아 커서를 움직인다.
{
	while (area == 0)  // 초기 화면에서만 동작 
	{
		int a = ShowCur();  // 변경 이전의 커서값 저장
		int temp;
		temp = _getch();
		if (temp == 224)  // 방향키를 입력받았을 때
		{
			temp = _getch();
			if (temp == 72)  // 위 방향키
			{
				if (ShowCur() != 25)
				{
					UpdownCur(-5);
					DeleteCursor(a);
					ArrowCursor(ShowCur());
					continue;
				}
			}
			else if (temp == 80)  // 아래 방향키
			{
				if (ShowCur() != 35)
				{
					UpdownCur(5);
					DeleteCursor(a);
					ArrowCursor(ShowCur());
					continue;
				}
			}
		}
		else if (temp == 13) // 엔터키를 입력받았을 때
		{
			if (a == 25) // #TODO 게임 시작
			{
				area = 1;  // 초기화면에서 벗어남
				HowToPlay();
			}
			else if (a == 30)  // Ranking
			{
				// #TODO Ranking 기능
			}
			else if (a == 35)  // Exit
			{
				exit(1);
			}
		}
	}
}

void HowToPlay()
{
	int x = 20, y = 10;
	system("cls");
	Gotoxy(x, y); 
	printf("************************************************************\n");
	for (int i = 0; i < 18; i++)
	{
		Gotoxy(x, y + i + 1);
		printf("*                                                          *\n");
	}
	Gotoxy(x, y + 19);
	printf("************************************************************\n");

	// 글자 출력 TODO
}

void SelectPlaneUI()
{
	//TODO
}

void SelectPlane()
{
	//TODO
}

void RankingUI()
{
	//TODO
}

int main(void)
{
	HideCursor(0);     // 커서 숨기기
	SetConsoleSize();  // 화면 크기 설정
	DrawTitle();       // 타이틀 출력
	PrintMenu();       // 메인 메뉴 출력
	ArrowCursor(25);   // 메인 메뉴 커서 출력
	GetArrowKey();     // 커서 이동 함수

	return 0;
}