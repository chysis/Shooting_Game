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
int ShowCur();                         // ���� ȭ���� '>>' Ŀ�� ��ġ �ҷ�����
void UpdownCur(int num);               // ���� ȭ�� Ŀ�� �̵� �Լ�
void GetArrowKey(); 

// ���� ����

int area = 0;   // ��� ����ġ (0: title, 1: How to Play, 2: Game Field, 3: Ranking) #TODO
int title_default_cur = 25;   // Ÿ��Ʋ ȭ�� Ŀ�� �ʱ� ��ġ.
						  
enum eColorSet { // �ܼ� ���� ���� (0~15)
	Black = 0, Blue, Green, Cyan, Red, Magenta, Brown, Lightgray, Darkgray,
	Lightblue, Lightgreen, Lightcyan, Lightred, Lightmagenta, Yellow, White = 15
};

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

int ShowCur()
// ���� ȭ���� >> Ŀ�� �� �ҷ�����
{
	return title_default_cur;
}

void UpdownCur(int num)
// �������� cur�� ����
{
	title_default_cur += num;
}

void GetArrowKey()
// Ÿ��Ʋ ȭ�鿡�� ����Ű�� �Է¹޾� Ŀ���� �����δ�.
{
	while (area == 0)  // �ʱ� ȭ�鿡���� ���� 
	{
		int a = ShowCur();  // ���� ������ Ŀ���� ����
		int temp;
		temp = _getch();
		if (temp == 224)  // ����Ű�� �Է¹޾��� ��
		{
			temp = _getch();
			if (temp == 72)  // �� ����Ű
			{
				if (ShowCur() != 25)
				{
					UpdownCur(-5);
					DeleteCursor(a);
					ArrowCursor(ShowCur());
					continue;
				}
			}
			else if (temp == 80)  // �Ʒ� ����Ű
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
		else if (temp == 13) // ����Ű�� �Է¹޾��� ��
		{
			if (a == 25) // #TODO ���� ����
			{
				area = 1;  // �ʱ�ȭ�鿡�� ���
				system("cls");
			}
			else if (a == 30)  // Ranking
			{
				// #TODO Ranking ���
			}
			else if (a == 35)  // Exit
			{
				exit(1);
			}
		}
	}
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