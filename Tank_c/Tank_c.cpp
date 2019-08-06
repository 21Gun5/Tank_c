#include <stdio.h>
#include <time.h>
#include "Data.h"
#include "Func.h"
#include <conio.h>

int main()
{
	//模拟多线程
	int time4Tank = 0;
	int time4Bullet = 0;
	int time4EnemyTank = 0;
	int time4EnemyBullet = 0;
	
	//我方坦克
	TANK tank = {
		{MAP_X_WALL / 4, MAP_Y / 2},
		{0},UP,true,
		//{{0},UP,不存在}
	};
	SetTankShape(&tank);//设置我方坦克形态

	//敌方坦克
	TANK enemyTank[ENEMY_TANK_AMOUNT] = {
		{{2, 2}, {0},DOWN ,true},
		//{{MAP_X_WALL / 4, 2}, {0},UP,true },
		//{{MAP_X_WALL / 2 - 2, 2}, {0},DOWN,true },

		//{{2,  MAP_Y / 2}, {0},UP ,true},
		//{{MAP_X_WALL / 2 - 2,  MAP_Y / 2}, {0},UP,true },

		//{{2,  MAP_Y - 3}, {0},UP ,true},
		//{{MAP_X_WALL / 4, MAP_Y - 3} ,{0},UP ,true },
		//{{MAP_X_WALL / 2 - 2,  MAP_Y - 3}, {0},UP,true }
	};
	for (int i = 0; i < ENEMY_TANK_AMOUNT; i++) {SetTankShape(&enemyTank[i]);}//设置敌方坦克形态

	//子弹创建
	BULLET bullet = {{0},UP,0};//结构体字段
	//BULLET bullet = { {0},UP};//全局变量控制

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
			ManipulateTank(&tank,我方坦克,enemyTank,&bullet);
			//ManipulateTank(&tank, 我方坦克, enemyTank);
			CleanTankTail(oldCore, oldBody);
			DrawTank(&tank,我方坦克);
		}

		//我方子弹移动线程

		//if(g_isBulExist != 不存在)
		if (bullet.state != 不存在)
		{
			//子弹赋值
			//if(g_isBulExist ==未赋值)
			if (bullet.state == 未赋值)//==1未赋值状态
			{
				bullet = { {tank.body[0].X, tank.body[0].Y}, tank.dir };
				bullet.state = 已赋值;
				//bullet.state++;
				//g_isBulExist = 已赋值;//==2已赋值状态
			}
			//子弹移动
			if (clock() - time4Bullet >= 50)
			{
				time4Bullet = clock();
				COORD oldBulCore = bullet.core;
				MoveBullet(&bullet);
				CleanBullet(oldBulCore);
				DrawBullet(&bullet);
				IsBulMeetOther(&bullet,enemyTank);
			}
		}

		//if (tank.bullet.state != 不存在)
		//{
		//	//子弹赋值
		//	if (tank.bullet.state == 未赋值)//==1未赋值状态
		//	{
		//		tank.bullet = { {tank.body[0].X, tank.body[0].Y}, tank.dir };
		//		tank.bullet.state = 已赋值;
		//		//g_isBulExist++;//==2已赋值状态
		//	}
		//	//子弹移动
		//	if (clock() - time4Bullet >= 50)
		//	{
		//		time4Bullet = clock();
		//		COORD oldBulCore = tank.bullet.core;
		//		MoveBullet(&tank.bullet);
		//		CleanBullet(oldBulCore);
		//		DrawBullet(&tank.bullet);
		//		IsBulMeetOther(&tank.bullet, enemyTank);
		//		//IsBulMeetOther(&tank.bullet, enemyTank,&tank);
		//	}
		//}

		//敌方坦克移动线程

		/*if (clock() - time4EnemyTank >= 500)
		{
			for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
			{
				time4EnemyTank = clock();
				COORD oldCore = enemyTank[i].core;
				COORD oldBody[5] = { enemyTank[i].body[0],enemyTank[i].body[1],enemyTank[i].body[2],enemyTank[i].body[3],enemyTank[i].body[4] };
				ManipulateTank2(&enemyTank[i], 敌方坦克,&tank,enemyTank);
				CleanTankTail(oldCore, oldBody);
				DrawTank(&enemyTank[i], 敌方坦克);
			}
		}*/

		//敌方子弹移动线程

		//for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
		//{
		//	if (enemyTank[i].bullet.state != 不存在)
		//	{
		//		//子弹赋值
		//		if (enemyTank[i].bullet.state == 未赋值)//==1未赋值状态
		//		{
		//			enemyTank[i].bullet = { {enemyTank[i].body[0].X, enemyTank[i].body[0].Y}, enemyTank[i].dir };
		//			enemyTank[i].bullet.state = 已赋值;
		//		}
		//		//子弹移动
		//		if (clock() - time4EnemyBullet >= 50)
		//		{
		//			time4EnemyBullet = clock();
		//			COORD oldBulCore = enemyTank[i].bullet.core;
		//			MoveBullet(&enemyTank[i].bullet);
		//			CleanBullet(oldBulCore);
		//			DrawBullet(&enemyTank[i].bullet);
		//			IsBulMeetOther2(&enemyTank[i].bullet, enemyTank);
		//		}
		//	}
		//}
	}
	return 0;
}