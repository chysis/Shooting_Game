#pragma once
#define WIDTH 101  //81   // ���� 80�� +1�� NULL�� �־��ֱ� ����.
#define HEIGHT 30  //24    // ���� 24��
#define stdhdl GetStdHandle(STD_OUTPUT_HANDLE)

void Initial();  //Ŀ���� �Ⱥ��̰� ���ִ� �Լ�

void MoveCursor(int x, int y);     // Ŀ���� �����̴� �Լ�

void ClearScreen();        // ȭ�鿡 ������ �����Ͽ� ����ִ� ȿ���� ���� ��

void SetConsoleSize();     // �ܼ�â ũ�� ����

void SetColor(int txt, int bg);   // �ؽ�Ʈ, ��� ���� ���� �Լ�
