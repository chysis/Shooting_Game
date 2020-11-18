/* Class Team Project */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>

#define stdhdl GetStdHandle(STD_OUTPUT_HANDLE)
#define title_cursor 40                // ���� ȭ�� Ŀ���� x��ǥ ��
#define plane_cursor 14                // ����� ����ȭ�� Ŀ���� y��ǥ ��                
#define MAX_ENEMY 10                   // ���� �ִ� ��
#define MAX_BALL 20                    // ���� �ִ� �Ѿ� ��

// �ܼ� ���� �Լ�

void Gotoxy(short x, short y);         // Ŀ�� �̵� �Լ�
void SetConsoleSize();                 // �ܼ�â ũ�� ���� �Լ�
void HideCursor(bool visibility);      // Ŀ�� ����� �Լ�
void SetColor(int txt, int bg);        // Ŀ�� ����, ��� ���� ���� �Լ�
BOOL IsKeyDown(int Key);               // Ű���尡 �����ִ��� �Ǵ�

									   // ���� �޴� ���� �Լ�

void DrawTitle();                      // Ÿ��Ʋ ȭ�� ��� �Լ�
void PrintMenu();                      // ���� �޴� ��� �Լ�
void DeleteCursor(int d, int type);    // ���� ȭ�� '>>' Ŀ�� ����� �Լ�
void ArrowCursor(int a, int type);     // ���� ȭ�� '>>' Ŀ�� ��� �Լ�
void GetTitleArrowKey();               // ���� ȭ�� Ŀ�� �Է¹޴� �Լ�

									   // ���� ���� �Լ�

void HowToPlay();                      // �÷��� ��� ��� �Լ�
void SelectPlane();                    // �÷��̾� ���� ���� �Լ� (UI ����)
void GetPlaneArrowKey();               // ���� ���� ȭ�� Ŀ�� �Լ�
void ShootingGameUI();                 // ���� UI��� �Լ�
void ShootingGame();                   // ���� �Լ�
void GetPlayerName();                  // ���� ���� �� �̸� �Է� �޴� �Լ�

									   // ��ŷ ���� �Լ�

void RankingUI();                      // ��ŷ ȭ�� UI ��� �Լ�

// ���� ����, ������, ����ü

int planeCode;      // �÷��̾ ������ ����� �ڵ�
int area = 0;   // ��� ����ġ (0: title, 1: How to Play, 2: Select Plane, 3: Game Field, 4: Ranking) #TODO
int score;      // ���� ����
int title_default_cur = 25;   // Ÿ��Ʋ ȭ�� Ŀ�� �ʱ� ��ġ.

int titleCursor[3] = { 25, 30, 35 };      // Ÿ��Ʋ Ŀ���� y��ǥ�� �迭
int planeCursor[4] = { 19, 39, 59, 79 };  // ����� ���� Ŀ���� x��ǥ�� �迭

enum ColorSet { // �ܼ� ���� ���� (0~15)
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
	int liveFlag;   // �⺻�� 1(alive), 0: death
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
	int initX;     // �ʱ� x ��ǥ��
	int initY;     // �ʱ� y ��ǥ��
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

char *arEnemy[] = { "oOoOo", "[-@-]", "^_*_^" };  // �� ���� �迭

int plane[4][3][3] = { // �÷��̾� ���� �迭
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

/* ���� */

BOOL IsKeyDown(int Key)
{
	return ((GetAsyncKeyState(Key) & 0x8000) != 0);
}

void Gotoxy(short x, short y)
// Ŀ���� (x, y)��ǥ�� �̵�. 
{
	COORD pos = { x, y }; // x, y��ǥ
	SetConsoleCursorPosition(stdhdl, pos); // set cursor
}

void SetConsoleSize()
// �ܼ� â ũ�⸦ ����
{
	system("mode con:lines=40 cols=100"); // lines=����, cols=����
}

void HideCursor(bool visibility)
// Ŀ���� �����. (visibility true: ����, false: �� ����)
{
	CONSOLE_CURSOR_INFO ConsoleCursor = { 0, };
	ConsoleCursor.dwSize = 1;
	ConsoleCursor.bVisible = visibility;
	SetConsoleCursorInfo(stdhdl, &ConsoleCursor);
}

void SetColor(int txt, int bg)
// txt: �ؽ�Ʈ ����, bg: ��� ����. ����ǥ�� ������ ColorSet ����.
{
	SetConsoleTextAttribute(stdhdl, txt + bg * 16);
}

void DrawTitle()
// ���� Ÿ��Ʋ ȭ�� ���� (�ӽ�)
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
// �޴� ���
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
// Ŀ�� ���� (type 1: ����ȭ��, 2: ����� ����ȭ��)
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
// �޴� ���ÿ� ȭ��ǥ Ŀ�� (type 1: ����ȭ��, 2: ����� ����ȭ��)
{
	if (type == 1)
	{
		Gotoxy(title_cursor, a);  // �ʱ� ��ġ: Game Start
		printf(">>");
	}
	else if (type == 2)
	{
		Gotoxy(a, plane_cursor);
		printf("��");
	}
	else
		return;
}

void GetTitleArrowKey()
// Ÿ��Ʋ ȭ�鿡�� ����Ű�� �Է¹޾� Ŀ���� �����δ�.
{
	int idx = 0;                  // ����: 0~2
	ArrowCursor(titleCursor[0], 1);  // �ʱ� Ŀ�� ��ġ
	while (area == 0)             // �ʱ� ȭ�鿡���� ���� 
	{
		int temp;
		temp = _getch();
		if (temp == 224)          // ����Ű�� �Է¹޾��� ��
		{
			temp = _getch();
			if (temp == 72)       // �� ����Ű
			{
				if (idx != 0)
				{
					DeleteCursor(titleCursor[idx--], 1);
					ArrowCursor(titleCursor[idx], 1);
					continue;
				}
			}
			else if (temp == 80)   // �Ʒ� ����Ű
			{
				if (idx != 2)
				{
					DeleteCursor(titleCursor[idx++], 1);
					ArrowCursor(titleCursor[idx], 1);
					continue;
				}
			}
		}
		else if (temp == 13)    // ����Ű�� �Է¹޾��� ��
		{
			if (idx == 0)         // #TODO ���� ����
			{
				area = 1;
				HowToPlay();
			}
			else if (idx == 1)   // Ranking
			{
				area = 2;
				// #TODO Ranking ���
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

	Gotoxy(35, 14); printf("���� �̵�         :  W");
	Gotoxy(35, 16); printf("�Ʒ��� �̵�       :  S");
	Gotoxy(35, 18); printf("���������� �̵�   :  D");
	Gotoxy(35, 20); printf("�������� �̵�     :  A");
	Gotoxy(35, 22); printf("�Ѿ� �߻�         : Space Bar");

	SetColor(12, 0);
	Gotoxy(30, 26); printf("����Ű�� �����ø� ���� ȭ������ �Ѿ�ϴ�...");
	SetColor(0, 0);

	if (area == 1)
	{
		while (1)
		{
			temp = _getch();
			if (temp == 13)        // ����Ű �Է¹޾��� ��
			{
				area = 2;
				SelectPlane();     // ����� ���� ȭ������ �̵�
			}
		}
	}
}

void PrintPlane(int arr[][3][3], int type, int x, int y)
{
	// ����� ����Ʈ ���ִ� �Լ�
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
	int idx = 0;                  // ����: 0~3
	ArrowCursor(planeCursor[0], 2);  // �ʱ� Ŀ�� ��ġ
	while (area == 2)
	{
		int temp;
		temp = _getch();
		if (temp == 224)          // ����Ű�� �Է¹޾��� ��
		{
			temp = _getch();
			if (temp == 75)       // ���� ����Ű
			{
				if (idx != 0)
				{
					DeleteCursor(planeCursor[idx--], 2);
					ArrowCursor(planeCursor[idx], 2);
					continue;
				}
			}
			else if (temp == 77)   // ������ ����Ű
			{
				if (idx != 3)
				{
					DeleteCursor(planeCursor[idx++], 2);
					ArrowCursor(planeCursor[idx], 2);
					continue;
				}
			}
		}
		else if (temp == 13)    // ����Ű�� �Է¹޾��� ��
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
	Gotoxy(20, 7); printf("����Ű�� �̿��� �÷����� ����⸦ �����ϼ���.");   // ��� �� �� TODO

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
	HideCursor(0);     // Ŀ�� �����
	SetConsoleSize();  // ȭ�� ũ�� ����
	DrawTitle();       // Ÿ��Ʋ ���
	PrintMenu();       // ���� �޴� ���
	ArrowCursor(25, 1);   // ���� �޴� Ŀ�� ���
	GetTitleArrowKey();     // Ŀ�� �̵� �Լ�

	return 0;
}