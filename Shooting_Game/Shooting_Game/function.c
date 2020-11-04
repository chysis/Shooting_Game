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

int area = 0;   // ��� ����ġ (0: title, 1: Game Field, 2: Ranking, 3: How to Play) #TODO
int title_default_cur = 25;   // Ÿ��Ʋ ȭ�� Ŀ�� �ʱ� ��ġ

							  // �ܼ� ���� ���� (0~15)
enum colorset {
	Black = 0, Blue, Green, Cyan, Red, Magenta, Brown, Lightgray, Darkgray,
	Lightblue, Lightgreen, Lightcyan, Lightred, Lightmagenta, Yellow, White = 15
};

void gotoxy(short x, short y)
// Ŀ���� (x, y)��ǥ�� �̵�. 
{
	COORD pos = { x, y }; // x, y��ǥ
	SetConsoleCursorPosition(stdhdl, pos); // set cursor
}

void SetConsolesize()
// �ܼ� â ũ�⸦ ����
{
	system("mode con:lines=50 cols=100"); // lines=����, cols=����
}

void HideCursor(bool visibility)
// Ŀ���� �����.(visibility true: ����, false: �� ����)
{
	CONSOLE_CURSOR_INFO ConsoleCursor = { 0, };
	ConsoleCursor.dwSize = 1;
	ConsoleCursor.bVisible = visibility;
	SetConsoleCursorInfo(stdhdl, &ConsoleCursor);
}

void Setcolor(int txt, int bg)
// txt: �ؽ�Ʈ ����, bg: ��� ����. ����ǥ�� ������ colorset ����.
{
	SetConsoleTextAttribute(stdhdl, txt + bg * 16);
}

void DrawTitle()
// ���� Ÿ��Ʋ ȭ�� ���� (�ӽ�)
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
// �޴� ���
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
// Ŀ�� ����
{
	gotoxy(title_cursor, d);
	printf("  ");
}

void ArrowCursor(int a)
// �޴� ���ÿ� ȭ��ǥ Ŀ��
{
	gotoxy(title_cursor, a);  // �ʱ� ��ġ: Game Start
	printf(">>");
}

int Showcur()
// �������� cur�� �ҷ�����
{
	return title_default_cur;
}

void Updowncur(int num)
// �������� cur�� ����
{
	title_default_cur += num;
}

void GetArrowKey()
// Ÿ��Ʋ ȭ�鿡�� ����Ű�� �Է¹޾� Ŀ���� �����δ�.
{
	while (area == 0)  // �ʱ� ȭ�鿡���� ���� 
	{
		int a = Showcur();  // ���� ������ Ŀ���� ����
		int temp;
		temp = _getch();
		if (temp == 224)  // ����Ű�� �Է¹޾��� ��
		{
			temp = _getch();
			if (temp == 72)  // �� ����Ű
			{
				if (Showcur() != 25)
				{
					Updowncur(-5);
					DeleteCursor(a);
					ArrowCursor(Showcur());
					continue;
				}
			}
			else if (temp == 80)  // �Ʒ� ����Ű
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
			else if (a == 35)  // How to Play?
			{
				// #TODO How to Play ���
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
	HideCursor(0);  // Ŀ�� �����
	SetConsolesize();  // ȭ�� ũ�� ����
	DrawTitle();
	PrintMenu();
	ArrowCursor(25);
	GetArrowKey();

	return 0;
}