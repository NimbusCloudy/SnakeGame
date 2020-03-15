#include <iostream>
#include <conio.h>
#include <math.h>
#include <Windows.h>
#include <time.h>
using namespace std;

enum Direction { UP = 72, DOWN = 80, RIGHT = 77, LEFT = 75};

void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}

struct Pos
{
	int r;
	int c;
};

struct Snake
{
	int Size;
	Pos *Ps;
	char SS;
	Direction D;
	int Score = 0;
};


struct Food
{
	char FS;
	Pos P;
	int Point;
};

bool IsValidPosition(Snake S, Pos P)
{
	for (int si =0 ; si < S.Size ; si++)
	{
		if (P.r == S.Ps[si].r && P.c == S.Ps[si].c)
			return false;
	}
	return true;
}
Pos FoodGenerate(Snake S,int Rows , int Cols)
{
	Pos P;
	do
	{
	P.r = rand() % Rows;
	P.c  =rand() % Cols;
	} while (IsValidPosition(S, P ) != true);
	return P;
}

Pos FoodGenerate2(Snake S, int Rows, int Cols)
{
	Pos P;
	do
	{
		P.r = rand() % Rows;
		P.c = rand() % Cols;
	} while (IsValidPosition(S, P) != true );
	return P;
}

void DisplaySnake(const Snake & S)
{
	for (int si =0 ; si < S.Size ; si++)
	{
		int ri = S.Ps[si].r;
		int ci = S.Ps[si].c;
		gotoRowCol(ri, ci);
		cout << S.SS;
	}
}

void EraseSnake(const Snake& S)
{
	for (int si = 0; si < S.Size; si++)
	{
		int ri = S.Ps[si].r;
		int ci = S.Ps[si].c;
		gotoRowCol(ri, ci);
		cout << ' ';
	}
}

void DisplayFood(const Food& F)
{
		int ri = F.P.r;
		int ci = F.P.c;
		gotoRowCol(ri, ci);
		cout << F.FS;
}

void MoveSnake(Snake & S, int Rows , int Cols)
{
	for (int si = S.Size -1 ; si > 0 ; si--)
	{
		S.Ps[si] = S.Ps[si - 1];
	}
	switch (S.D)
	{
	case UP:
		S.Ps[0].r--;
		break;
	case DOWN:
		S.Ps[0].r++;
		break;
	case RIGHT:
		S.Ps[0].c++;
		break;
	case LEFT:
		S.Ps[0].c--;
		break;
	}
	if (S.Ps[0].r==-1)
		S.Ps[0].r = Rows - 1;
	if (S.Ps[0].r == Rows)
		S.Ps[0].r = 0;
	if (S.Ps[0].c == -1)
		S.Ps[0].c = Cols - 1;
	if (S.Ps[0].c == Cols)
		S.Ps[0].c = 0;
}

void Init(int Cols, int Rows, Snake& S, Food& F)
{
	S.Size = 1; S.Ps = new Pos[1];
	S.SS = -37;
	S.Ps[0].r = Rows / 2;
	S.Ps[0].c = Cols / 2;
	S.Score = 0;
	F.FS = '$';
	F.P = FoodGenerate(S, Rows, Cols);
}

void StartDisplay(int Rows , int Cols)
{
	gotoRowCol(Rows / 2, Cols / 2 - 10 );
	cout << "   S";
	Sleep(500);
	cout << "   N";
	Sleep(500);
	cout << "   A";
	Sleep(500);
	cout << "   K";
	Sleep(500);
	cout << "   E" << endl;
	Sleep(1000);
}

void GameOvertDisplay(int Rows, int Cols)
{
	gotoRowCol(Rows / 2, 10 );
	cout << "G";
	Sleep(500);
	cout << "   A";
	Sleep(500);
	cout << "   M";
	Sleep(500);
	cout << "   E";
	Sleep(500);
	cout << "        O";
	cout << "   V";
	Sleep(500);
	cout << "   E";
	Sleep(500);
	cout << "   R";
	Sleep(500);
}

void DisplayScore(Snake S,int Rows, int Cols)
{
	gotoRowCol(Rows / 2 + 5, Cols / 2 - 10 );
	cout << "S";
	Sleep(500);
	cout << "   C";
	Sleep(500);
	cout << "   O";
	Sleep(500);
	cout << "   R";
	Sleep(500);
	cout << "   E : " << S.Score;
	Sleep(500);
}

int main()
{
	srand(time(0));
	int Rows = 80, Cols = 80;
	Snake  S;
	Food F;
	StartDisplay(Rows, Cols);
	system("CLS");
	Init(Cols , Rows , S , F);
	F.P = FoodGenerate(S, Rows, Cols);
	DisplayFood(F);
	while (true)
	{
		if (_kbhit() == true)
		{
			char ch = _getch();
			if (ch == -32)
			{
				ch = _getch();
				switch (ch)
				{
				case UP:
					if(S.D != DOWN )
					S.D = UP;
					break;
				case DOWN:
					if (S.D != UP)
					S.D = DOWN;
					break;
				case RIGHT:
					if (S.D != LEFT)
					S.D = RIGHT;
					break;
				case LEFT:
					if (S.D != RIGHT)
					S.D = LEFT;
					break;
				}
			}

		}
		if (S.Ps[0].r == F.P.r && S.Ps[0].c == F.P.c)
		{
			S.Size++;
			S.Score += 10;
			F.P = FoodGenerate(S, Rows, Cols);
			DisplayFood(F);
		}
		Sleep(50);
		EraseSnake(S);
		MoveSnake(S, Rows, Cols);
		DisplaySnake(S);
		for (int i = 1; i < S.Size; i++)
		{
			if (S.Ps[0].r == S.Ps[i].r && S.Ps[0].c == S.Ps[i].c)
			{
				system("CLS");
				gotoRowCol(Rows/2 , Cols / 2);
				GameOvertDisplay( Rows, Cols);
				DisplayScore(S, Rows, Cols);
				exit(1);
			}
		}
		Sleep(50);
	}
	return _getch();;
}
