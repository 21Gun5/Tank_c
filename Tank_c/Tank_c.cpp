#include <stdio.h>
#include <time.h>
#include "Data.h"
#include "Func.h"
#include <conio.h>

int main()
{
	int time4Tank = 0;
	int time4Bullet = 0;

	TANK tank = { {MAP_X_WALL / 4, MAP_Y / 2}, {0},UP };
	SetTankShape(&tank);//设置坦克形态
	BULLET bullet = {{0},UP};

	GameInit();
	DrawWelcome();
	SelectAction();
	DrawMapBorder();
	BarrierInit();
	DrawBarr();

	while (g_isRunning)
	{
		//坦克移动线程
		if (clock() - time4Tank >= 100)
		{
			time4Tank = clock();
			COORD oldCore = tank.core;
			COORD oldBody[5] = { tank.body[0],tank.body[1],tank.body[2],tank.body[3],tank.body[4] };
			ManipulateTank(&tank);
			CleanTankTail(oldCore, oldBody);
			DrawTank(&tank);
		}

		//子弹移动线程
		if (g_isBulExist != 0)//1和2均可，只要不为0
		{
			//子弹赋值
			if (g_isBulExist == 1)//==1未赋值状态
			{
				bullet = { {tank.body[0].X, tank.body[0].Y}, tank.dir };
				g_isBulExist++;//==2已赋值状态
			}

			//子弹移动
			if (clock() - time4Bullet >= 50)
			{
				time4Bullet = clock();
				COORD oldBulCore = bullet.core;
				MoveBullet(&bullet);
				CleanBullet(oldBulCore);
				DrawBullet(&bullet);
				IsBulMeetOther(&bullet);
			}
		}
	}
	return 0;
}