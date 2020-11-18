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
#define plane_cursor 14                // 비행기 선택화면 커서의 y좌표 값                
#define MAX_ENEMY 10                   // 적군 최대 수
#define MAX_BALL 20                    // 적군 최대 총알 수

// 콘솔 관리 함수

void Gotoxy(short x, short y);         // 커서 이동 함수
void SetConsoleSize();                 // 콘솔창 크기 조절 함수
void HideCursor(bool visibility);      // 커서 숨기는 함수
void SetColor(int txt, int bg);        // 커서 색상, 배경 색상 지정 함수
BOOL IsKeyDown(int Key);               // 키보드가 눌려있는지 판단

									   // 메인 메뉴 관련 함수

void DrawTitle();                      // 타이틀 화면 출력 함수
void PrintMenu();                      // 메인 메뉴 출력 함수
void DeleteCursor(int d, int type);    // 메인 화면 '>>' 커서 지우는 함수
void ArrowCursor(int a, int type);     // 메인 화면 '>>' 커서 출력 함수
void GetTitleArrowKey();               // 메인 화면 커서 입력받는 함수

									   // 게임 구현 함수

void HowToPlay();                      // 플레이 방법 출력 함수
void SelectPlane();                    // 플레이어 유닛 선택 함수 (UI 포함)
void GetPlaneArrowKey();               // 유닛 선택 화면 커서 함수
void ShootingGameUI();                 // 게임 UI출력 함수
void ShootingGame();                   // 게임 함수
void GetPlayerName();                  // 게임 종료 후 이름 입력 받는 함수

									   // 랭킹 구현 함수

void RankingUI();                      // 랭킹 화면 UI 출력 함수

// 전역 변수, 열거형, 구조체

int planeCode;      // 플레이어가 선택한 비행기 코드
int area = 0;   // 모드 스위치 (0: title, 1: How to Play, 2: Select Plane, 3: Game Field, 4: Ranking) #TODO
int score;      // 게임 점수
int title_default_cur = 25;   // 타이틀 화면 커서 초기 위치.

int titleCursor[3] = { 25, 30, 35 };      // 타이틀 커서의 y좌표값 배열
int planeCursor[4] = { 19, 39, 59, 79 };  // 비행기 선택 커서의 x좌표값 배열

enum ColorSet { // 콘솔 색상 모음 (0~15)
	Black = 0, Blue, Green, Cyan, Red, Magenta, Brown, Lightgray, Darkgray,
	Lightblue, Lightgreen, Lightcyan, Lightred, Lightmagenta, Yellow, White = 15
};

struct StartInfo
{
	int x, y;
};

struct PlayerInfo
{
	int x, y;
	int liveFlag;   // 기본값 1(alive), 0: death
};

struct BossInfo
{
	int x, y;
	int liveFlag;
	int moveFlag;
	int startX;
};

struct EnemyInfo
{
	int x, y;
	int liveFlag;
	int initX;     // 초기 x 좌표값
	int initY;     // 초기 y 좌표값
	int moveFlag;
};

struct ShotInfo
{
	int x, y;
	int type;
	int useFlag;
};

struct ItemInfo
{
	int x, y;
	int type;
	int useFlag;
};

char *arEnemy[] = { "oOoOo", "[-@-]", "^_*_^" };  // 적 유닛 배열

int plane[4][3][3] = { // 플레이어 유닛 배열
	{
		{ 1,0,1 },
		{ 0,1,0 },
		{ 1,0,1 }
	},
	{
		{ 1,0,1 },
		{ 1,1,1 },
		{ 1,0,1 }
	},
	{
		{ 1,1,1 },
		{ 0,1,0 },
		{ 0,1,0 }
	},
	{
		{ 0,1,0 },
		{ 1,0,1 },
		{ 0,1,0 }
	}
};

/*
const int plane1[3][3] = { { 1,0,1 },{ 0,1,0 },{ 1,0,1 } }; //X
const int plane2[3][3] = { { 1,0,1 },{ 1,1,1 },{ 1,0,1 } }; //H
const int plane3[3][3] = { { 1,1,1 },{ 0,1,0 },{ 0,1,0 } }; //T
const int plane4[3][3] = { { 0,1,0 },{ 1,0,1 },{ 0,1,0 } }; //O
*/

/* 구현 */

BOOL IsKeyDown(int Key)
{
	return ((GetAsyncKeyState(Key) & 0x8000) != 0);
}

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
// txt: 텍스트 색상, bg: 배경 색상. 색상표는 열거형 ColorSet 참고.
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

void DeleteCursor(int d, int type)
// 커서 제거 (type 1: 메인화면, 2: 비행기 선택화면)
{
	if (type == 1)
	{
		Gotoxy(title_cursor, d);
		printf("  ");
	}
	else if (type == 2)
	{
		Gotoxy(d, plane_cursor);
		printf("  ");
	}
	else
		return;
}

void ArrowCursor(int a, int type)
// 메뉴 선택용 화살표 커서 (type 1: 메인화면, 2: 비행기 선택화면)
{
	if (type == 1)
	{
		Gotoxy(title_cursor, a);  // 초기 위치: Game Start
		printf(">>");
	}
	else if (type == 2)
	{
		Gotoxy(a, plane_cursor);
		printf("△");
	}
	else
		return;
}

void GetTitleArrowKey()
// 타이틀 화면에서 방향키를 입력받아 커서를 움직인다.
{
	int idx = 0;                  // 범위: 0~2
	ArrowCursor(titleCursor[0], 1);  // 초기 커서 위치
	while (area == 0)             // 초기 화면에서만 동작 
	{
		int temp;
		temp = _getch();
		if (temp == 224)          // 방향키를 입력받았을 때
		{
			temp = _getch();
			if (temp == 72)       // 위 방향키
			{
				if (idx != 0)
				{
					DeleteCursor(titleCursor[idx--], 1);
					ArrowCursor(titleCursor[idx], 1);
					continue;
				}
			}
			else if (temp == 80)   // 아래 방향키
			{
				if (idx != 2)
				{
					DeleteCursor(titleCursor[idx++], 1);
					ArrowCursor(titleCursor[idx], 1);
					continue;
				}
			}
		}
		else if (temp == 13)    // 엔터키를 입력받았을 때
		{
			if (idx == 0)         // #TODO 게임 시작
			{
				area = 1;
				HowToPlay();
			}
			else if (idx == 1)   // Ranking
			{
				area = 2;
				// #TODO Ranking 기능
			}
			else if (idx == 2)   // Exit
			{
				exit(1);
			}
		}
	}
}

void HowToPlay()
{
	int x = 20, y = 10;
	int temp;

	system("cls");
	Gotoxy(x, y);
	printf("***********************< How To Play? >*********************\n");
	for (int i = 0; i < 18; i++)
	{
		Gotoxy(x, y + i + 1);
		printf("*                                                          *\n");
	}
	Gotoxy(x, y + 19);
	printf("************************************************************\n");

	Gotoxy(35, 14); printf("위로 이동         :  W");
	Gotoxy(35, 16); printf("아래로 이동       :  S");
	Gotoxy(35, 18); printf("오른쪽으로 이동   :  D");
	Gotoxy(35, 20); printf("왼쪽으로 이동     :  A");
	Gotoxy(35, 22); printf("총알 발사         : Space Bar");

	SetColor(12, 0);
	Gotoxy(30, 26); printf("엔터키를 누르시면 다음 화면으로 넘어갑니다...");
	SetColor(0, 0);

	if (area == 1)
	{
		while (1)
		{
			temp = _getch();
			if (temp == 13)        // 엔터키 입력받았을 때
			{
				area = 2;
				SelectPlane();     // 비행기 선택 화면으로 이동
			}
		}
	}
}

void PrintPlane(int arr[][3][3], int type, int x, int y)
{
	// 비행기 프린트 해주는 함수
	Gotoxy(x, y);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (arr[type][i][j] == 1)
			{
				Gotoxy(x + j, y + i);
				if (type == 0)
				{
					SetColor(Yellow, Black);
					printf("X");
				}
				else if (type == 1)
				{
					SetColor(Lightblue, Black);
					printf("H");
				}
				else if (type == 2)
				{
					SetColor(Lightcyan, Black);
					printf("T");
				}
				else //type==3
				{
					SetColor(White, Black);
					printf("O");
				}
			}
		}
	}
}

void GetPlaneArrowKey()
{
	int idx = 0;                  // 범위: 0~3
	ArrowCursor(planeCursor[0], 2);  // 초기 커서 위치
	while (area == 2)
	{
		int temp;
		temp = _getch();
		if (temp == 224)          // 방향키를 입력받았을 때
		{
			temp = _getch();
			if (temp == 75)       // 왼쪽 방향키
			{
				if (idx != 0)
				{
					DeleteCursor(planeCursor[idx--], 2);
					ArrowCursor(planeCursor[idx], 2);
					continue;
				}
			}
			else if (temp == 77)   // 오른쪽 방향키
			{
				if (idx != 3)
				{
					DeleteCursor(planeCursor[idx++], 2);
					ArrowCursor(planeCursor[idx], 2);
					continue;
				}
			}
		}
		else if (temp == 13)    // 엔터키를 입력받았을 때
		{
			area = 3;
			planeCode = idx;
			ShootingGameUI();
		}
	}
}

void SelectPlane()
{
	system("cls");
	Gotoxy(20, 7); printf("방향키를 이용해 플레이할 비행기를 선택하세요.");   // 출력 안 됨 TODO

	PrintPlane(plane, 0, 18, 10);
	PrintPlane(plane, 1, 38, 10);
	PrintPlane(plane, 2, 58, 10);
	PrintPlane(plane, 3, 78, 10);

	GetPlaneArrowKey();
}

void ShootingGameUI()
{
	return;
}

void ShootingGame()
{

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
	ArrowCursor(25, 1);   // 메인 메뉴 커서 출력
	GetTitleArrowKey();     // 커서 이동 함수

	return 0;
}