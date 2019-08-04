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
	//BULLET bullet;
	//BULLET bullet{ {MAP_X_WALL / 4, MAP_Y / 2}, UP };
	int flag = 0;

	GameInit();
	DrawWelcome();
	SelectAction();
	DrawMapBorder();

	while (g_isRunning)
	{

		////坦克移动
		if (clock() - nTime1 >= 100)
		{
			nTime1 = clock();
			COORD oldCore = tank.core;
			COORD oldBody[5] = { tank.body[0],tank.body[1],tank.body[2],tank.body[3],tank.body[4] };
			//MoveTank(&tank,&bullet);
			MoveTank(&tank);
			CleanTankTail(oldCore, oldBody);
			DrawTank(&tank);
			
		}

		



		
	
		if (g_isFire == 1)
		{
			static BULLET bullet{ {tank.body[0].X, tank.body[0].Y}, tank.dir };

			//子弹移动
			if (clock() - nTime2 >= 300)
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