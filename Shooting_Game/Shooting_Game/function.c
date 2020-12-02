#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <memory.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include "console.h"

char screen[HEIGHT][WIDTH];        //    [행][열]
char MainScreen[HEIGHT][WIDTH] = {
	"                                                                                                   \n",
	"             =========            ==            ====          ====      ============               \n",
	"           ==                   ==  ==          == ==        == ==      ==                         \n",
	"          ==                   ==    ==         ==  ==      ==  ==      ==                         \n",
	"         ==    =======        ==========        ==   ==    ==   ==      ============               \n",
	"          ==        ==       ==        ==       ==    ==  ==    ==      ==                         \n",
	"           ==       ==      ==          ==      ==     ====     ==      ==                         \n",
	"             =========     ==            ==     ==      ==      ==      ============               \n",
	"                                                                                                   \n",
	"                                               |                       Version 1.0                 \n",
	"                                              | |                                                  \n",
	"                                             | | |                                                 \n",
	"                                                                                                   \n",
	"                                                                                                   \n",
	"                                          Start Game                                               \n", // 14,42
	"                                          Ranking                                                  \n", // 15,42
	"                                          Exit Game                                                \n", // 16,42
	"                                                                                                   \n",
	"                                                                                                   \n",
	"                                                                                                   \n",
	"                                                                                                   \n",
	"                                                                                                   \n",
	" < How To Play >                                                                                   \n",
	" Space Bar : Fire                                                                                  \n",
	" Up    : ↑             < ITEM >                                                                    \n",
	" Down  = ↓             P = Power                                                                   \n",
	" Left  = ←             L = Life                                                                    \n",
	" Right = →             S = Speed                                                                   \n",
	"                                                                                                   \n",
	"                                                                                                   \n"
};

#define P_COUNT 3
#define E_COUNT 3
#define ENEMY_FLIGHT 10
#define ENEMY_LINE 3
#define ENEMY_NUM 30
#define B_SHOT 2
#define P_SHOT 0
#define E_SHOT 1
#define SHOT_MAX 40
#define PLAYER_LIFE 12        // 유저 피한칸 늘릴때마다 +2씩해주기
#define SCORE_COUNT 8
#define ITEM_COUNT 2
#define ITEM_MAX 3
#define POWER_ITEM 2
#define LIFE_ITEM 4
#define SPEED_ITEM 6
#define B_COUNT 7
#define BOSS_LIFE 36          // 보스 피한칸 늘릴때마다 +2 씩해주기

#define plane_cursor 14
#define MAX_INPUT 30
#define MAX_NAME 10           // 입력받을 수 있는 최대 글자

int BossLifeCount = BOSS_LIFE;
char BossLife[BOSS_LIFE + 1] = " Boss : ♥♥♥♥♥♥♥♥♥♥♥♥♥♥";
int PowerCount = 1;
int SpeedCount = 1;
int LifeCount = PLAYER_LIFE;
int stagecount = 0;
char bShot = '$';
char pShot = '|';
char pShot2 = '@';
char eShot = '*';
char startcursor[3] = ">>";
char planecursor[4] = "[v]";
//char pUnit[P_COUNT + 1] = ".=^=.";        // +1은 뒤에 NULL을 붙여주기위해서
char eUnit[E_COUNT + 1] = "=.=";
char eUnit2[E_COUNT + 1] = "oOo";
char PlayerLife[PLAYER_LIFE + 1] = " HP : ♥♥♥";
char Power = 'P';
char Life = 'L';
char Speed = 'S';
char Score[SCORE_COUNT + 1] = "Score : ";
char bUnit[B_COUNT + 1] = "!|=@=|!";
int Score2 = 0;
int planeCode;          // 플레이어가 선택한 비행기 코드

enum ColorSet { // 콘솔 색상 모음 (0~15)
	Black = 0, Blue, Green, Cyan, Red, Magenta, Brown, Lightgray, Darkgray,
	Lightblue, Lightgreen, Lightcyan, Lightred, Lightmagenta, Yellow, White = 15
};

//int planeCursor[4] = { 19, 39, 59, 79 };  // 비행기 선택 커서의 x좌표값 배열

char plane[4][3][3] = { // 플레이어 유닛 배열
	{
		{"X X"},
		{" X "},   // X
		{"X X"}
	},
	{
		{"H H"},
		{"HHH"},   // H
		{"H H"}
	},
	{
		{"TTT"},
		{" T "},   // T
		{" T "}
	},
	{
		{" O "},
		{"O O"},   // O
		{" O "}
	}
};

struct StartInfo {
	int x, y;
};

struct PlaneInfo {
	int x, y;
};

struct PlayerInfo {
	int x, y;
	int liveFlag;        // 플레이어가 살았는지를 나타낸다. 1로초기화시키고 죽으면 0이되어 게임을 끝낸다.
};

struct BossInfo {
	int x, y;
	int LiveFlag;
	int MoveFlag;
	int StartX;
};

struct EnemyInfo {
	int x, y;
	int liveFlag;
	int StartX;            //    기준점좌표값 x - StartX 절대값을 구해서.
	int StartY;
	int MoveFlag;        // 0 = x좌표를 -- 좌 무빙 , 1 = x좌표를 ++ 우 무빙
};

struct ShotInfo {
	int x, y;
	int Type;
	int UseFlag;
};

struct ItemInfo {
	int x, y;
	int Type;
	int UseFlag;
};

typedef struct {
	char name[MAX_NAME];
	int score;
}DATA;

struct StartInfo StartGame;
struct PlaneInfo ChoosePlane;
struct BossInfo boss;
struct ItemInfo item[ITEM_MAX];
struct ShotInfo shot[SHOT_MAX];
struct PlayerInfo player;                // 플레이어 비행기 구조체선언
struct EnemyInfo enemy[ENEMY_NUM];        // 적비행기 30대 구조체선언

int StartGameAction2();
void StartGameAction();
void DrawMain();
void DrawStartGame();
void StartGameInitialObject();
void DrawBossInfo();
int BossClearGame();
void BossInitalObject();
void DrawBoss();
void PowerUp();
void SpeedUp();
void LifeUp();
void CheckCrashItem();
void CreateItem(int ItemNumber, int x, int y);
void DrawItem();
void DrawScore();
void DrawPlayerInfo();
void SetStartPosition();            // 스테이지 시작시 초기화해주는 함수
int CheckClearGame();
int CheckEndGame();
void DrawShot();
void PlayerAction();
void EnemyAction();
void EnemyAction2();
void Draw();
void DrawPlayer();
void DrawEnemy();
void PlayerInitialObject();
void EnemyInitialObject();
void CreateShot(int Type, int x, int y);
void ShotAction();
void CheckCrash();
void ItemAction();
void BossAction();

void SelectPlaneInitialObject();
void DrawSelectPlane();
void SelectPlaneAction();
int SelectPlaneAction2();
void PrintPlane(char arr[][3][3], int type, int x, int y);
void DeleteCursor(int d);
void ArrowCursor(int a);
//int GetPlaneArrowKey();
void SelectPlane();
void SaveRecord();
void Ranking();

void main() {

	SetConsoleSize();
	srand((unsigned)time(NULL));
	Initial(FALSE);  // 커서 안 보이게
	StartGameInitialObject();
	while (stagecount == 0)
	{
		if (stagecount == 0)
		{
			StartGameAction();
			DrawMain();
			if (StartGameAction2() == 1)
			{
				stagecount = 5;
			}
			else if (StartGameAction2() == 2)
				Ranking();
			else if (StartGameAction2() == 3)
				break;
		}
		Sleep(100);
	}

	SelectPlaneInitialObject();
	while (stagecount == 5)
	{
		SelectPlane();
		DrawSelectPlane();
		SelectPlaneAction();
		if (SelectPlaneAction2() == 1)
		{
			planeCode = 0;
			stagecount = 1;
		}
		else if (SelectPlaneAction2() == 2)
		{
			planeCode = 1;
			stagecount = 1;
		}
		else if (SelectPlaneAction2() == 3)
		{
			planeCode = 2;
			stagecount = 1;
		}
		else if (SelectPlaneAction2() == 4)
		{
			planeCode = 3;
			stagecount = 1;
		}
	}

	SetStartPosition();
	while (stagecount == 1)
	{
		if (stagecount == 1)
		{
			EnemyAction();
			PlayerAction();
			ShotAction();
			CheckCrash();
			ItemAction();
			CheckCrashItem();
			Draw();
			if (CheckEndGame() == 1)
				SaveRecord();
			if (CheckClearGame() == 1)
				stagecount++;
		}
		Sleep(100);
	}

	SetStartPosition();
	while (stagecount == 2)
	{

		if (stagecount == 2)
		{
			EnemyAction2();
			PlayerAction();
			ShotAction();
			CheckCrash();
			ItemAction();
			Draw();
			if (CheckEndGame() == 1)
				SaveRecord();
			if (CheckClearGame() == 1)
				stagecount++;
		}

		Sleep(100);
	}

	Initial(FALSE);
	PlayerInitialObject();
	BossInitalObject();
	while (stagecount == 3)
	{
		if (stagecount == 3)
		{
			PlayerAction();
			BossAction();
			ShotAction();
			CheckCrash();
			ItemAction();
			Draw();
			if (CheckEndGame() == 1)
				SaveRecord();
			if (BossClearGame() == 1)
				SaveRecord();
		}
		Sleep(100);
	}
}

void SelectPlane()
{
	system("cls");
	MoveCursor(30, 7);
	printf("비행기를 선택하세요.");
	PrintPlane(plane, 0, 18, 10);
	PrintPlane(plane, 1, 38, 10);
	PrintPlane(plane, 2, 58, 10);
	PrintPlane(plane, 3, 78, 10);
	Sleep(100);
}

/*int GetPlaneArrowKey()
{
	int idx = 0;                  // 범위: 0~3
	ArrowCursor(planeCursor[0]);  // 초기 커서 위치
	while (stagecount == 5)
	{
		if ((GetAsyncKeyState(VK_LEFT) & 0x8000))
		{
			if (idx=0)
			{
				DeleteCursor(planeCursor[0]);
				ArrowCursor(planeCursor[1]);
				idx = 1;
			}
			if (idx = 1)
			{
				DeleteCursor(planeCursor[1]);
				ArrowCursor(planeCursor[2]);
				idx = 2;
			}
			if (idx = 2)
			{
				DeleteCursor(planeCursor[2]);
				ArrowCursor(planeCursor[3]);
				idx = 3;
			}
		}
		if ((GetAsyncKeyState(VK_RIGHT) & 0x8000))
		{
			if (idx=3)
			{
				DeleteCursor(planeCursor[3]);
				ArrowCursor(planeCursor[2]);
				idx = 2;
			}
			if (idx = 2)
			{
				DeleteCursor(planeCursor[2]);
				ArrowCursor(planeCursor[1]);
				idx = 1;
			}
			if (idx = 1)
			{
				DeleteCursor(planeCursor[1]);
				ArrowCursor(planeCursor[0]);
				idx = 0;
			}
		}
		/*int temp;
		temp = _getch();
		if (temp == 224)          // 방향키를 입력받았을 때
		{
			temp = _getch();
			if (temp == 75)       // 왼쪽 방향키
			{
				if (idx != 0)
				{
					DeleteCursor(planeCursor[idx--]);
					ArrowCursor(planeCursor[idx]);
					continue;
				}
			}
			else if (temp == 77)   // 오른쪽 방향키
			{
				if (idx != 3)
				{
					DeleteCursor(planeCursor[idx++]);
					ArrowCursor(planeCursor[idx]);
					continue;
				}
			}
		}//
		if ((GetAsyncKeyState(VK_RETURN) & 0x8000))    // 엔터키를 입력받았을 때
		{
			planeCode = idx;
			stagecount = 1;
			return 1;
		}
	}
}*/

void DeleteCursor(int d)
// 커서 제거 (type 1: 메인화면, 2: 비행기 선택화면)
{
	//screen[plane_cursor][d] = "  ";
	MoveCursor(d, plane_cursor);
	printf("  ");
	return;
}

void ArrowCursor(int a)
// 메뉴 선택용 화살표 커서 (type 1: 메인화면, 2: 비행기 선택화면)
{
	//screen[plane_cursor][a] = "△";
	MoveCursor(a, plane_cursor);
	printf("△");
	return;
}

void PrintPlane(char arr[][3][3], int type, int x, int y)
{
	// 비행기 프린트 해주는 함수
	//MoveCursor(x, y);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			//screen[y][x] = arr[type][i][j];
			MoveCursor(x + j, y + i);
			printf("%c", arr[type][i][j]);
			//x++;
		}
		//y++;
	}
}

void BossInitalObject() {

	boss.x = 39;
	boss.y = 5;
	boss.StartX = boss.x;
	boss.LiveFlag = 1;        // 1로 설정해주어 죽으면 0으로 바뀌게끔 설정
	boss.MoveFlag = 1;
}

void DrawBoss() {

	int i;
	int x = boss.x - P_COUNT / 2;
	int y = boss.y;

	if (y < 0 || y >= HEIGHT)        // y값이 화면 밖으로나가면 오류로 실행종료
		return;

	if (boss.LiveFlag == 1)
	{
		for (i = 0; i < B_COUNT; i++)
		{
			if (x >= 0 && x <WIDTH - 1)
				screen[y][x] = bUnit[i];
			x++;
		}
	}
}

void BossAction() {

	if (boss.MoveFlag == 1)
		boss.x += 2;
	else
		boss.x -= 2;

	if (abs(boss.StartX - boss.x) > 15)
	{
		boss.MoveFlag = !boss.MoveFlag;
		boss.y++;
	}
	if (rand() % 100 < 30)
		CreateShot(B_SHOT, boss.x, boss.y);
}

void EnemyAction() {            // 적비행기 이동함수

	int i;                        // STAGE 5.
	for (i = 0; i<ENEMY_NUM; i++)
	{
		if (enemy[i].liveFlag)
		{
			if (i % 2 == 0)
			{
				if (enemy[i].MoveFlag == 1)
					enemy[i].y++;
				else if (enemy[i].MoveFlag == 2)
					enemy[i].x++;
				else if (enemy[i].MoveFlag == 3)
					enemy[i].y--;
				else if (enemy[i].MoveFlag == 4)
					enemy[i].x--;
			}

			if ((enemy[i].y - enemy[i].StartY == 1) && (enemy[i].x - enemy[i].StartX == 0))
				enemy[i].MoveFlag = 2;
			if ((enemy[i].y - enemy[i].StartY == 1) && (enemy[i].x - enemy[i].StartX == 9))
				enemy[i].MoveFlag = 3;
			if ((enemy[i].y - enemy[i].StartY == -1) && (enemy[i].x - enemy[i].StartX == 9))
				enemy[i].MoveFlag = 4;
			if ((enemy[i].y - enemy[i].StartY == -1) && (enemy[i].x - enemy[i].StartX == 0))
				enemy[i].MoveFlag = 1;
			if (rand() % 100 < 5)
				CreateShot(E_SHOT, enemy[i].x, enemy[i].y);
		}
	}
}

void EnemyAction2() {
	int i;                                        // STAGE 4.
	for (i = 0; i<ENEMY_NUM; i++)
	{
		if (enemy[i].liveFlag == 1)
		{
			if (i > 9 && i < 20)
			{
				if (enemy[i].MoveFlag == 1)
					enemy[i].x--;
				else
					enemy[i].x++;
			}
			else
			{
				if (enemy[i].MoveFlag == 1)
					enemy[i].x++;
				else
					enemy[i].x--;
			}
			if (abs(enemy[i].StartX - enemy[i].x) > 10)
			{
				enemy[i].MoveFlag = !enemy[i].MoveFlag;
				enemy[i].y++;

			}
			if (rand() % 100 < 5)
				CreateShot(E_SHOT, enemy[i].x, enemy[i].y);
		}
	}
}

void PlayerAction() {            // 플레이어 비행기 이동 함수

	if (player.liveFlag == 1)
	{
		if ((GetAsyncKeyState(VK_UP) & 0x8000))        //GetAsyncKeyState는 short값으로 입력이 되었으면 맨앞비트가 1 아니면 0을 리턴
		{                                                //0x8000은 16진수로 short값의 맨앞만 1인상태이므로
			if (player.y > HEIGHT - HEIGHT)            //비트연산자 &를 사용해주어 리턴된 비트만 맨앞자리가 1이면 0을 리턴
			{                                            //둘다 맨앞자리가 1이면 0이 아닌숫자를 리턴
				if (SpeedCount == 1)
					player.y--;
				else if (SpeedCount == 2)
					player.y--;
			}
		}
		if ((GetAsyncKeyState(VK_DOWN) & 0x8000))
		{
			if (player.y < HEIGHT - 2)
			{
				if (SpeedCount == 1)
					player.y++;
				else if (SpeedCount == 2)
					player.y++;
			}
		}
		if ((GetAsyncKeyState(VK_RIGHT) & 0x8000))
		{
			if (player.x < WIDTH - 4)
			{
				if (SpeedCount == 1)
					player.x += 2;
				else if (SpeedCount == 2)
					player.x += 4;
			}
		}
		if ((GetAsyncKeyState(VK_LEFT) & 0x8000))
		{
			if (player.x > WIDTH - 98)
			{
				if (SpeedCount == 1)
					player.x -= 2;
				else if (SpeedCount == 2)
					player.x -= 4;
			}
		}
		if ((GetAsyncKeyState(VK_SPACE) & 0x8000))
		{
			if (PowerCount == 1)
			{
				CreateShot(P_SHOT, player.x, player.y);
			}
			else if (PowerCount == 2)
			{
				CreateShot(P_SHOT, player.x, player.y);
				CreateShot(P_SHOT, player.x - 2, player.y);
				CreateShot(P_SHOT, player.x + 2, player.y);
			}
		}
	}
}

void StartGameAction() {
	if (stagecount == 0)
	{
		if ((GetAsyncKeyState(VK_UP) & 0x8000))
		{
			if (StartGame.y > 14)
			{
				StartGame.y--;
			}
		}
		if ((GetAsyncKeyState(VK_DOWN) & 0x8000))
		{
			if (StartGame.y <= 16)
			{
				StartGame.y++;
			}
		}
	}
}

void SelectPlaneAction() {
	if (stagecount == 5)
	{
		if ((GetAsyncKeyState(VK_LEFT) & 0x8000))
		{
			if (ChoosePlane.x > 18)
			{
				ChoosePlane.x -= 20;
			}
		}
		if ((GetAsyncKeyState(VK_RIGHT) & 0x8000))
		{
			if (ChoosePlane.x <= 78)
			{
				ChoosePlane.x += 20;
			}
		}
	}
}

int StartGameAction2() {
	if (stagecount == 0)
	{
		if ((GetAsyncKeyState(VK_RETURN) & 0x8000))
		{
			if (StartGame.y == 14)
			{
				return 1;
			}
			else if (StartGame.y == 15)
			{
				return 2;
			}
			else if (StartGame.y == 16)
			{
				return 3;
			}
		}
	}
}

int SelectPlaneAction2() {
	if (stagecount == 5)
	{
		if ((GetAsyncKeyState(VK_RETURN) & 0x8000))
		{
			if (ChoosePlane.x == 18)
			{
				return 1;
			}
			else if (ChoosePlane.x == 38)
			{
				return 2;
			}
			else if (ChoosePlane.x == 58)
			{
				return 3;
			}
			else if (ChoosePlane.x == 78)
			{
				return 4;
			}
		}
	}
}

void CreateShot(int Type, int x, int y) {

	int i;
	for (i = 0; i<SHOT_MAX; i++)
	{
		if (shot[i].UseFlag == 0)
		{
			shot[i].UseFlag = 1;
			shot[i].Type = Type;
			shot[i].x = x;
			shot[i].y = y;
			return;
		}
	}
}

void PlayerInitialObject() {       //플레이어 비행기 좌표설정 (1대이므로 지정해준다.)
	player.x = 49;
	player.y = 25;

	player.liveFlag = 1;           // 1로 설정해주어 죽으면 0으로 바뀌게끔 설정
}

void EnemyInitialObject() {        // 적 비행기 좌표설정    ( i 는 적비행기 객체, j 행 , k 열 ) 

	int i = 0;
	for (int j = 0; j < ENEMY_LINE; j++)
	{
		for (int k = 0; k < ENEMY_FLIGHT; k++)
		{
			enemy[i].x = 10 + 8 * k;
			enemy[i].y = 3 + 3 * j;
			enemy[i].liveFlag = 1;
			enemy[i].StartX = enemy[i].x;    //enemy[i]의 x좌표와 처음 동일시
			enemy[i].StartY = enemy[i].y;
			enemy[i].MoveFlag = 1;

			i++;
		}
	}
}

void Draw() {
	int i;
	for (i = 0; i < HEIGHT; i++)            // HEIGHT = 31
	{
		memset(screen[i], ' ', WIDTH);        //화면의  i번째 행들을 ' ' 공백으로 width의 길이만큼 넣어주고
		screen[i][WIDTH - 1] = NULL;            // 마지막 width칸에 NULL을 넣어준다.
	}

	DrawPlayer();        // 유저 비행기 출력
	DrawEnemy();         // 적비행기 출력
	DrawShot();
	DrawPlayerInfo();
	DrawScore();
	DrawItem();
	DrawBoss();
	DrawStartGame();
	if (stagecount == 3)
		DrawBossInfo();

	for (i = 0; i < HEIGHT; i++)
	{
		MoveCursor(0, i);        //  커서의 시작 지점 즉 y축을 바꾸어주며 한줄단위로 출력
		printf(screen[i]);       //  화면의 i번째행 을 출력   ==  printf("%s",screen[i]);

		MoveCursor(70, 23);
		printf("%d", Score2);
	}
}

void DrawMain() {
	int i;
	for (i = 0; i<HEIGHT; i++)
	{
		strcpy(MainScreen[14], "                                          Start Game                                               \n");
		strcpy(MainScreen[15], "                                          Ranking                                                  \n");
		strcpy(MainScreen[16], "                                          Exit Game                                                \n");

		MainScreen[i][WIDTH - 1] = NULL;
	}

	DrawStartGame();

	for (i = 0; i<HEIGHT; i++)
	{
		MoveCursor(0, i);
		printf(MainScreen[i]);
	}
}

void StartGameInitialObject() {
	StartGame.x = 41;
	StartGame.y = 14;
	int x = StartGame.x;
	int y = StartGame.y;
}

void SelectPlaneInitialObject() {
	ChoosePlane.x = 18;
	ChoosePlane.y = 14;
	int x = ChoosePlane.x;
	int y = ChoosePlane.y;
}

void DrawEnemy() {        // 적 비행기 좌표 지정

	int i, j;
	int x, y;

	for (i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].liveFlag == 1)
		{
			x = enemy[i].x - E_COUNT / 2;
			y = enemy[i].y;

			if (y < 0 || y >= HEIGHT)
				continue;

			if (stagecount == 1)
			{
				for (j = 0; j < E_COUNT; j++)
				{
					if (x >= 0 && x < WIDTH - 1)
						screen[y][x] = eUnit[j];
					x++;
				}
			}
			else if (stagecount == 2)
			{
				for (j = 0; j < E_COUNT; j++)
				{
					if (x >= 0 && x < WIDTH - 1)
						screen[y][x] = eUnit2[j];
					x++;
				}
			}
		}
	}
}

void DrawPlayer() {        // 플레이어 비행기 출력
	int i;
	int x = player.x - P_COUNT / 2;
	int y = player.y;

	if (y < 0 || y >= HEIGHT-3)        // y값이 화면 밖으로나가면 오류로 실행종료
		return;

	if (player.liveFlag == 1)
	{
		for (i = 0; i < P_COUNT; i++)
		{
			for (int j = 0; j < P_COUNT; j++)
			{
				if (x >= 0 && x < WIDTH - 3)
				{
					//MoveCursor(x + j, y + i);
					//printf(plane[planeCode][i][j]);
					screen[y+i][x+j] = plane[planeCode][i][j];
					//x++;
				}
				//y++;
			}
		}
	}
}

void DrawStartGame() {
	int i;
	int x = StartGame.x - 2;
	int y = StartGame.y;

	for (i = 0; i<2; i++)
	{
		if (x >= 39 && x < 41 && y >= 14 && y <= 16)
			MainScreen[y][x] = startcursor[i];
		x++;
	}
}

void DrawSelectPlane() {
	int i = 0;
	int x = ChoosePlane.x;
	int y = ChoosePlane.y;

	MoveCursor(x, y);
	for (i = 0; i < 3; i++)
	{
		printf("%c", planecursor[i]);
	}

}

void DrawPlayerInfo() {
	int i;
	int x = 0;
	int y = 23;

	for (i = 0; i<LifeCount; i++)
	{
		screen[y][x] = PlayerLife[i];
		x++;
	}
}

void DrawBossInfo() {
	int i;
	int x = 0;
	int y = 0;

	for (i = 0; i<BossLifeCount; i++)
	{
		screen[y][x] = BossLife[i];

		x++;
	}
}

void DrawScore() {
	int i;
	int x = 60;
	int y = 23;

	for (i = 0; i<SCORE_COUNT; i++)
	{
		screen[y][x] = Score[i];
		x++;
	}
}

void DrawShot() {
	int i;
	for (i = 0; i < SHOT_MAX; i++)
	{
		if (shot[i].UseFlag == 1)
		{
			int x = shot[i].x;
			int y = shot[i].y;
			if ((x >= 0 && x<WIDTH - 1) && (y > 0 && y < 30))
			{
				if (shot[i].Type == E_SHOT)
					screen[y][x] = eShot;
				else if (shot[i].Type == P_SHOT)
				{
					if (PowerCount == 1)
						screen[y][x] = pShot;
					else if (PowerCount == 2)
					{
						screen[y][x] = pShot2;
					}
				}
				else if (shot[i].Type == B_SHOT)
				{
					SetColor(Yellow, Black);
					screen[y][x] = bShot;
					screen[y][x + 2] = bShot;
					screen[y][x - 2] = bShot;
					SetColor(White, Black);
				}
			}
		}
	}
}

void ShotAction() {
	int i;
	for (i = 0; i<SHOT_MAX; i++)
	{
		if (shot[i].UseFlag == 1)
		{
			if (shot[i].Type == E_SHOT)  // 적 총알
			{
				shot[i].y++;  // y값 증가 -> 화면 밑으로 내려감
				if (shot[i].y >= HEIGHT)
					shot[i].UseFlag = 0;
			}
			else if (shot[i].Type == P_SHOT)  // 플레이어 총알
			{
				shot[i].y--;   // y값 감소 -> 화면 위로 올라감
				if (shot[i].y <= 0)
					shot[i].UseFlag = 0;
			}
			else if (shot[i].Type == B_SHOT)  // 보스 총알
			{
				shot[i].y++;  // y값 증가 -> 화면 밑으로 내려감
				if (shot[i].y <= 0)
					shot[i].UseFlag = 0;
			}
		}
	}
}

void CheckCrash() {

	int i, j;
	int itemdrop;
	for (i = 0; i<SHOT_MAX; i++)
	{
		for (j = 0; j<ENEMY_NUM; j++)
		{
			if (shot[i].UseFlag == 1)
			{
				if (shot[i].Type == P_SHOT)
				{
					if (enemy[j].liveFlag == 1)
					{
						if (PowerCount == 1)
						{
							if (((shot[i].y == enemy[j].y)) && ((shot[i].x >(enemy[j].x - 2)) && (shot[i].x < (enemy[j].x + 2))))  // 충돌 판정
							{
								enemy[j].liveFlag = 0;
								if (enemy[j].liveFlag == 0)  // 적이 죽으면
								{
									itemdrop = rand() % 100 + 1;  // 1~100
									if (itemdrop < 6)  // 5%의 확률로 아이템 생성
										CreateItem(itemdrop, enemy[j].x, enemy[j].y);
								}
								shot[i].UseFlag = 0;
								Score2 += 50;  // 50점 추가
								break;
							}
						}
						if (PowerCount == 2)
						{
							if (((shot[i].y == enemy[j].y)) && ((shot[i].x >(enemy[j].x - 4)) && (shot[i].x < (enemy[j].x + 4))))
							{
								enemy[j].liveFlag = 0;
								if (enemy[j].liveFlag == 0)
								{
									itemdrop = rand() % 100 + 1;
									if (itemdrop < 6)
										CreateItem(itemdrop, enemy[j].x, enemy[j].y);
								}
								shot[i].UseFlag = 0;
								Score2 += 50;
								break;
							}
						}
					}
				}
			}
		}
		if (shot[i].UseFlag == 1)
		{
			if (shot[i].Type == P_SHOT)
			{
				if (boss.LiveFlag == 1)
				{
					if (((shot[i].y == boss.y)) && ((shot[i].x > (boss.x - 4)) && (shot[i].x < (boss.x + 4))))
					{
						shot[i].UseFlag = 0;
						BossLifeCount -= 2;
						if (BossLifeCount == 8)
						{
							boss.LiveFlag = 0;
							shot[i].UseFlag = 0;
							Score2 += 1000;  // 보스 1000점 추가
							break;
						}
					}
				}
			}
		}
		if (shot[i].UseFlag == 1)
		{
			if (shot[i].Type == E_SHOT)  // 적 총알
			{
				if (player.liveFlag == 1)
				{
					if ((shot[i].y == player.y) && ((shot[i].x >= (player.x - P_COUNT / 2)) && (shot[i].x <= (player.x + P_COUNT / 2))))
					{
						shot[i].UseFlag = 0;
						LifeCount -= 2;  // 하트 문자 하나가 2바이트이므로 2 감소
						if (LifeCount == 6)  // 12에서 시작, 3개 감소 시 게임 오버 판정
						{
							player.liveFlag = 0;
							shot[i].UseFlag = 0;
							break;
						}
					}
				}
			}
		}
		if (shot[i].UseFlag == 1)
		{
			if (shot[i].Type == B_SHOT)  // 보스 총알
			{
				if (player.liveFlag == 1)
				{
					if ((shot[i].y == player.y) && ((shot[i].x >= (player.x - 4)) && (shot[i].x <= (player.x + +4))))
					{
						shot[i].UseFlag = 0;
						LifeCount -= 2;
						if (LifeCount == 6)
						{
							player.liveFlag = 0;
							shot[i].UseFlag = 0;
							break;
						}
					}
				}
			}
		}
	}
}

void CheckCrashItem() {
	int i;
	for (i = 0; i<ITEM_MAX; i++)
	{
		if (player.liveFlag == 1)
		{
			if ((item[i].y <= (player.y)) && (item[i].y >= (player.y)) && ((item[i].x >= (player.x - 2) && (item[i].x <= (player.x + 2)))))
			{
				item[i].UseFlag = 0;
				if (item[i].Type <= POWER_ITEM)
				{
					PowerUp();
					item[i].UseFlag = 0;
				}
				else if (item[i].Type <= LIFE_ITEM && item[i].Type > POWER_ITEM)
				{
					LifeUp();
					item[i].UseFlag = 0;
				}
				else if (item[i].Type <= SPEED_ITEM && item[i].Type > LIFE_ITEM)
				{
					SpeedUp();
					item[i].UseFlag = 0;
				}
				break;
			}
		}
	}
}

void PowerUp() {
	if (PowerCount == 1)
		PowerCount += 1;
	else if (PowerCount == 2)
		PowerCount += 0;
}

void SpeedUp() {
	if (SpeedCount == 1)
		SpeedCount += 1;
	else if (SpeedCount == 2)
		SpeedCount += 0;
}
void LifeUp() {

	if (LifeCount != 12)
		LifeCount += 2;
	else
		LifeCount += 0;

}

int CheckEndGame() {

	if (player.liveFlag == 0)
	{
		MoveCursor(36, 12);
		printf(" Game Over ");
		Sleep(1000);
		MoveCursor(0, 24);
		return 1;
	}
}

int CheckClearGame() {
	int i;
	for (i = 0; i<ENEMY_NUM; i++)
	{
		if (enemy[i].liveFlag == 1)
		{
			return 0;
		}
	}
	MoveCursor(36, 12);
	printf(" Stage Clear ");
	MoveCursor(0, 24);
	Sleep(1000);
	return 1;
}
int BossClearGame() {
	if (boss.LiveFlag == 1)
		return 0;

	MoveCursor(34, 12);
	printf(" All Stage Clear ");
	MoveCursor(0, 24);
	Sleep(1000);
	return 1;
}

void SetStartPosition() {

	Initial(FALSE);               // 커서 안보이게
	PlayerInitialObject();        // 플레이어 비행기 시작 좌표 설정
	EnemyInitialObject();         // 적 비행기 좌표설정
}

void CreateItem(int ItemNumber, int x, int y) {

	int i = 0;
	for (i = 0; i<ITEM_MAX; i++)
	{
		if (item[i].UseFlag == 0)
		{
			item[i].UseFlag = 1;
			item[i].x = x - E_COUNT / 2;
			item[i].y = y;
			item[i].Type = ItemNumber;
			return;
		}
	}
}

void DrawItem() {

	int i, j;
	int x, y;
	for (i = 0; i<ITEM_MAX; i++)
	{
		if (item[i].UseFlag == 1)
		{

			x = item[i].x;
			y = item[i].y;

			if ((x >= 0 && x<WIDTH - 1) && (y > 0 && y < 30))
			{
				for (j = 0; j<ITEM_COUNT; j++)
				{
					if (item[i].Type <= POWER_ITEM)
					{
						screen[y][x] = Power;

					}
					else if (item[i].Type <= LIFE_ITEM && item[i].Type > POWER_ITEM)
					{
						screen[y][x] = Life;

					}
					else if (item[i].Type <= SPEED_ITEM && item[i].Type > LIFE_ITEM)
					{
						screen[y][x] = Speed;

					}
				}
			}
		}
	}
}

void ItemAction() {
	int i;
	for (i = 0; i<ITEM_MAX; i++)
	{
		if (item[i].UseFlag == 1)
		{
			if (item[i].Type <= POWER_ITEM)
			{
				item[i].y++;
				if (item[i].y >= HEIGHT)
					item[i].UseFlag = 0;
			}
			else if (item[i].Type <= LIFE_ITEM && item[i].Type > POWER_ITEM)
			{
				item[i].y++;
				if (item[i].y <= 0)
					item[i].UseFlag = 0;
			}
			else if (item[i].Type <= SPEED_ITEM && item[i].Type > LIFE_ITEM)
			{
				item[i].y++;
				if (item[i].y <= 0)
					item[i].UseFlag = 0;
			}
		}
	}
}

void SaveRecord() {

	DATA data;

	ClearScreen();
	MoveCursor(40, 12);
	printf("Score: %d", Score2);
	MoveCursor(40, 13);
	printf("이름을 입력하세요:");
	Initial(TRUE);
	scanf("%s", data.name);
	
	data.score = Score2;

	FILE *fp = fopen("Rank.txt", "a");

	fwrite(&data, sizeof(data), 1, fp);

	fclose(fp);
	Sleep(1000);

	SetColor(Lightred, Black);
	MoveCursor(40, 15);
	printf("기록이 저장되었습니다.");
	SetColor(White, Black);
	Sleep(1000);
	MoveCursor(40, 20);
	exit(-1);
}

void Ranking() {
	int i = 0, j = 0;
	DATA input[MAX_INPUT] = { 0 };
	DATA temp;
	int tem;

	FILE *fp = fopen("Rank.txt", "r");

	if (fp == NULL)
	{
		printf("파일 열기 실패!\n");
		exit(0);
	}

	for (int i = 0; i < MAX_INPUT; i++)
	{
		fscanf(fp, "%s %d", input[i].name, &input[i].score);
	}

	fclose(fp);

	for (i = MAX_INPUT - 1; i > 0; i--)
	{
		for (j = 0; j < i; j++)
		{
			if (input[j].score < input[j + 1].score)
			{
				temp = input[j + 1];
				input[j + 1] = input[j];
				input[j] = temp;
			}
		}
	}

	printf("\n\t\t\t      -----------------\n");
	printf("\t\t\t     |     Ranking     |\n");
	printf("\t\t\t      -----------------\n\n");
	printf("\tRank\t\t\t  Nickname\t\t\t  Score\n\n");

	for (i = 0; i < 5; i++)
	{
		printf("\t %d\t\t\t%10s\t\t\t%7d\n\n", i + 1, input[i].name, input[i].score);
	}
	printf("\n");
	system("pause");
	return;
}