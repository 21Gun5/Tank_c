#include <stdio.h>
#include <time.h>
#include "Data.h"
#include "Func.h"

int main()
{
	TANK tank = { {MAP_X_WALL / 4, MAP_Y / 2}, {0},UP };
	SetTankShape(&tank);	//…Ë÷√ÃπøÀ–ŒÃ¨
	GameInit();
	DrawWelcome();
	SelectAction();
	DrawMapBorder();

	while (g_isRunning)
	{
		COORD oldCore = tank.core;
		COORD oldBody[5] = { tank.body[0],tank.body[1],tank.body[2],tank.body[3],tank.body[4] };
		MoveTank(&tank);
		CleanTankTail(oldCore, oldBody);
		DrawTank(&tank);
		Sleep(20);
	}
	return 0;
}