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

/* �̰����� ��� �Լ��� Ȯ���ϼ��� */

// �ܼ� ���� �Լ�

void Gotoxy(short x, short y);         // Ŀ�� �̵� �Լ�
void SetConsoleSize();                 // �ܼ�â ũ�� ���� �Լ�
void HideCursor(bool visibility);      // Ŀ�� ����� �Լ�
void SetColor(int txt, int bg);        // Ŀ�� ����, ��� ���� ���� �Լ�

// ���� �޴� ���� �Լ�

void DrawTitle();                      // Ÿ��Ʋ ȭ�� ��� �Լ�
void PrintMenu();                      // ���� �޴� ��� �Լ�
void DeleteCursor(int d);              // ���� ȭ�� '>>' Ŀ�� ����� �Լ�
void ArrowCursor(int a);               // ���� ȭ�� '>>' Ŀ�� ��� �Լ�
void GetArrowKey();                    // ���� ȭ�� Ŀ�� �Է¹޴� �Լ�

// ���� ���� �Լ�

void HowToPlay();                      // �÷��� ��� ��� �Լ�
void SelectPlaneUI();                  // �÷��̾� ���� ���� UI ��� �Լ�
void SelectPlane();                    // �÷��̾� ���� ���� �Լ�

// ��ŷ ���� �Լ�

void RankingUI();                      // ��ŷ ȭ�� UI ��� �Լ�

// ���� ����, ������, ����ü

int area = 0;   // ��� ����ġ (0: title, 1: How to Play, 2: Game Field, 3: Ranking) #TODO
int title_default_cur = 25;   // Ÿ��Ʋ ȭ�� Ŀ�� �ʱ� ��ġ.

int titleCursor[3] = { 25, 30, 35 };   // Ÿ��Ʋ Ŀ���� y��ǥ�� �迭
						  
enum eColorSet { // �ܼ� ���� ���� (0~15)
	Black = 0, Blue, Green, Cyan, Red, Magenta, Brown, Lightgray, Darkgray,
	Lightblue, Lightgreen, Lightcyan, Lightred, Lightmagenta, Yellow, White = 15
};

/* ���� */

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
// txt: �ؽ�Ʈ ����, bg: ��� ����. ����ǥ�� ������ eColorSet ����.
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

void DeleteCursor(int d)
// Ŀ�� ����
{
	Gotoxy(title_cursor, d);
	printf("  ");
}

void ArrowCursor(int a)
// �޴� ���ÿ� ȭ��ǥ Ŀ��
{
	Gotoxy(title_cursor, a);  // �ʱ� ��ġ: Game Start
	printf(">>");
}

void GetArrowKey()
// Ÿ��Ʋ ȭ�鿡�� ����Ű�� �Է¹޾� Ŀ���� �����δ�.
{
	int idx = 0;                  // ����: 0~2
	ArrowCursor(titleCursor[0]);  // �ʱ� Ŀ�� ��ġ
	while (area == 0)             // �ʱ� ȭ�鿡���� ���� 
	{
		int temp;
		temp = _getch();
		if (temp == 224)          // ����Ű�� �Է¹޾��� ��
		{
			temp = _getch();
			if (temp == 72)       // �� ����Ű
			{
				if (idx!=0)
				{
					DeleteCursor(titleCursor[idx--]);
					ArrowCursor(titleCursor[idx]);
					continue;
				}
			}
			else if (temp == 80)   // �Ʒ� ����Ű
			{
				if (idx!=2)
				{
					DeleteCursor(titleCursor[idx++]);
					ArrowCursor(titleCursor[idx]);
					continue;
				}
			}
		}
		else if (temp == 13)    // ����Ű�� �Է¹޾��� ��
		{
			if (idx==0)        // #TODO ���� ����
			{
				area = 1;       // �ʱ�ȭ�鿡�� ���
				HowToPlay();
			}
			else if (idx==1)   // Ranking
			{
				// #TODO Ranking ���
			}
			else if (idx==2)   // Exit
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

	while (1)
	{
		temp = _getch();
		if (temp == 13)        // ����Ű �Է¹޾��� ��
			SelectPlaneUI();   // ����� ���� ȭ������ �̵�
	}
}

int plane1[3][3] = { {1,0,1}, {0,1,0}, {1,0,1} }; //X
int plane2[3][3] = { {1,0,1}, {1,1,1}, {1,0,1} }; //H
int plane3[3][3] = { {1,1,1}, {0,1,0}, {0,1,0} }; //T
int plane4[3][3] = { {0,1,0}, {1,0,1}, {0,1,0} }; //O
//����� �� ��

void PrintPlane(const arr[][3], int x, int y) {
	Gotoxy(x, y);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (arr[i][j] = 1)
			{
				Gotoxy(x + j, y + i);
				SetColor(White, Black);
				if (x == 20)
					printf("X");
				else if (x == 30)
					printf("H");
				else if (x == 40)
					printf("T");
				else 
					printf("O");
			}
		}

	}
}
// ����� ����Ʈ ���ִ� �Լ�

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
	HideCursor(0);     // Ŀ�� �����
	SetConsoleSize();  // ȭ�� ũ�� ����
	DrawTitle();       // Ÿ��Ʋ ���
	PrintMenu();       // ���� �޴� ���
	ArrowCursor(25);   // ���� �޴� Ŀ�� ���
	GetArrowKey();     // Ŀ�� �̵� �Լ�

	return 0;
}