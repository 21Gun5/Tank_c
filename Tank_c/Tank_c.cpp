#include <stdio.h>
#include <time.h>
#include "Data.h"
#include "Func.h"
#include <conio.h>

int main()
{
	int nTime1 = 0;
	int nTime2 = 0;

	TANK tank = { {MAP_X_WALL / 4, MAP_Y / 2}, {0},UP };
	SetTankShape(&tank);	//设置坦克形态
	
	

	GameInit();
	DrawWelcome();
	SelectAction();
	DrawMapBorder();

	
	BarrierInit();
	DrawBarr();


	//g_isRunning = 0;
	while (g_isRunning)
	{

		//坦克移动
		if (clock() - nTime1 >= 100)
		{
			nTime1 = clock();
			COORD oldCore = tank.core;
			COORD oldBody[5] = { tank.body[0],tank.body[1],tank.body[2],tank.body[3],tank.body[4] };
			MoveTank(&tank);
			CleanTankTail(oldCore, oldBody);
			DrawTank(&tank);
		}

	
		if (g_isFire == 1)
		{
			static BULLET bullet;
			if (g_isBulExist == 1)
			{
				bullet = { {tank.body[0].X, tank.body[0].Y}, tank.dir };
				g_isBulExist++;
			}
			//子弹移动
			if (clock() - nTime2 >= 50)
			{
				nTime2 = clock();
				if (g_isBulExist)
				{
					COORD oldBulCore = bullet.core;
					MoveBullet(&bullet);
					CleanBullet(oldBulCore);
					DrawBullet(&bullet);
					IsBulExist(&bullet);
				}
			}
		}
	}
	return 0;
}