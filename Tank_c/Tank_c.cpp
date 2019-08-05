#include <stdio.h>
#include <time.h>
#include "Data.h"
#include "Func.h"
#include <conio.h>

int main()
{
	//模拟多线程
	int time4Tank = 0;
	int time4EnemyTank = 0;
	int time4Bullet = 0;

	//我方坦克
	TANK tank = { {MAP_X_WALL / 4, MAP_Y / 2}, {0},UP };
	g_MAP[tank.core.X][tank.core.Y] = 我方坦克;
	SetTankShape(&tank,我方坦克);//设置我方坦克形态

	//敌方坦克
	TANK enemyTank[3] = {
		{{2, 2}, {0},DOWN },
		{{MAP_X_WALL / 4, 2}, {0},DOWN },
		{{MAP_X_WALL/2 -2, 2}, {0},DOWN }
	};
	g_MAP[2][2] = 敌方坦克;
	g_MAP[MAP_X_WALL / 4][2] = 敌方坦克;
	g_MAP[MAP_X_WALL / 2 - 2][2] = 敌方坦克;
	for (int i = 0; i < 3; i++) {SetTankShape(&enemyTank[i],敌方坦克);}//设置敌方坦克形态

	
	//子弹创建
	BULLET bullet = {{0},UP};

	//基本流程
	GameInit();
	DrawWelcome();
	SelectAction();
	DrawMapBorder();
	BarrierInit();
	DrawBarr();

	//主循环
	while (g_isRunning)
	{
		//我方坦克移动线程
		if (clock() - time4Tank >= 100)
		{
			time4Tank = clock();
			COORD oldCore = tank.core;
			COORD oldBody[5] = { tank.body[0],tank.body[1],tank.body[2],tank.body[3],tank.body[4] };
			ManipulateTank(&tank,我方坦克,enemyTank);
			CleanTankTail(oldCore, oldBody);
			DrawTank(&tank,我方坦克);
		}

		//我方子弹移动线程
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

		//敌方坦克移动线程
		if (clock() - time4EnemyTank >= 5)
		{
			for (int i = 0; i < 3; i++)
			{
				time4EnemyTank = clock();
				COORD oldCore = enemyTank[i].core;
				COORD oldBody[5] = { enemyTank[i].body[0],enemyTank[i].body[1],enemyTank[i].body[2],enemyTank[i].body[3],enemyTank[i].body[4] };
				ManipulateTank2(&enemyTank[i], 敌方坦克,&tank,enemyTank);
				CleanTankTail(oldCore, oldBody);
				DrawTank(&enemyTank[i], 敌方坦克);
			}
		}

	}
	return 0;
}