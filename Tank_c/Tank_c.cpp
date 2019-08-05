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

		//坦克移动线程
		if (clock() - nTime1 >= 100)
		{
			nTime1 = clock();
			COORD oldCore = tank.core;
			COORD oldBody[5] = { tank.body[0],tank.body[1],tank.body[2],tank.body[3],tank.body[4] };
			MoveTank(&tank);
			CleanTankTail(oldCore, oldBody);
			DrawTank(&tank);
		}

		//子弹移动线程
		if (g_isBulExist != 0)//1和2均可，只要不为0
		{
			//子弹创建及赋值
			static BULLET bullet;//开火后才可创建对象
			if (g_isBulExist == 1)//只有1时，才可赋值
			{
				bullet = { {tank.body[0].X, tank.body[0].Y}, tank.dir };//参数由坦克提供
				g_isBulExist++;//马上++，此时为2，只有1才可赋值
			}

			//子弹移动
			if (clock() - nTime2 >= 50)
			{
				nTime2 = clock();
				COORD oldBulCore = bullet.core;//记录下旧地址
				MoveBullet(&bullet);//移动
				CleanBullet(oldBulCore);//清理旧
				DrawBullet(&bullet);//画
				IsBulMeetOther(&bullet);//是否遇其他对象
			}
		}
	}
	return 0;
}