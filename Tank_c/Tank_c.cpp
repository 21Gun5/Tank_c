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
		{0},UP,3,true,我方坦克,
		{{0},UP,不存在}
	};
	SetTankShape(&tank);//设置形态

	//敌方坦克
	TANK enemyTank[ENEMY_TANK_AMOUNT] = {
		{{2, 2}, {0},DOWN ,2,true,敌方坦克,{{0},UP,不存在}},
		{{MAP_X_WALL / 4, 2}, {0},UP,1,true,敌方坦克,{{0},UP,不存在} },
		{{MAP_X_WALL / 2 - 2, 2}, {0},DOWN,2,true,敌方坦克,{{0},UP,不存在} },

		{{2,  MAP_Y / 2}, {0},UP ,1,true,敌方坦克,{{0},UP,不存在}},
		{{MAP_X_WALL / 2 - 2,  MAP_Y / 2}, {0},UP,1,true,敌方坦克,{{0},UP,不存在} },

		{{2,  MAP_Y - 3}, {0},UP ,2,true,敌方坦克,{{0},UP,不存在}},
		//{{MAP_X_WALL / 4, MAP_Y - 3} ,{0},UP ,2,true,敌方坦克,{{0},UP,不存在} },
		{{MAP_X_WALL / 2 - 2,  MAP_Y - 3}, {0},UP,2,true,敌方坦克,{{0},UP,不存在} }
	};
	for (int i = 0; i < ENEMY_TANK_AMOUNT; i++) {SetTankShape(&enemyTank[i]);}//设置形态

	//初始化及欢迎界面
	GameInit();
	DrawWelcome();

	//整体流程
	int action = SelectAction();
	if (action == 开始游戏)
	{
		//选谁的地图
		int whoMap = SelectWhoMap();
		if (whoMap == 系统默认)
		{
			SelectLevel();
			BarrierInit();//使用默认的
		}
		else if (whoMap == 自定义)
		{
			//选何时的地图
			int whenMap = SelectWhenMap();
			if (whenMap == 新建地图)
			{
				SetBarrier(&tank,enemyTank);//手动设置并直接使用
			}
			else if (whenMap == 已有地图)
			{
				char* str = ShowMaps();
				LoadMap(str);//导入已有地图
			}
		}
	}
	else if (action == 读取游戏)
	{
		return 0;
	}
	else if (action == 退出游戏)
	{
		return 0;
	}

	//边界及障碍
	DrawMapBorder();
	DrawGameHelp();
	DrawBarr();

	//主循环
	while (g_isRunning)
	{
		//信息实时显示
		DrawGameInfo(&tank, enemyTank);

		//我方坦克线程
		if (clock() - time4Tank >= 100)
		{
			time4Tank = clock();
			COORD oldCore = tank.core;
			COORD oldBody[5] = { tank.body[0],tank.body[1],tank.body[2],tank.body[3],tank.body[4] };
			ManipulateMyTank(&tank, 我方坦克, enemyTank);
			CleanTankTail(oldCore, oldBody);
			DrawTank(&tank,我方坦克);
		}

		//我方子弹线程
		if (clock() - time4Bullet >= 50)
		{
			if (tank.bullet.state != 不存在)
			{
				//子弹赋值
				if (tank.bullet.state == 未赋值)//==1未赋值状态
				{
					tank.bullet = { {tank.body[0].X, tank.body[0].Y}, tank.dir };
					tank.bullet.state = 已赋值;
					//g_isBulExist++;//==2已赋值状态
				}
				//子弹移动
				time4Bullet = clock();
				COORD oldBulCore = tank.bullet.core;
				MoveBullet(&tank.bullet);
				CleanBullet(oldBulCore);
				DrawBullet(&tank.bullet,&tank);
				IsMyBulMeetOther(&tank.bullet, enemyTank,&tank);
				//IsBulMeetOther(&tank.bullet, enemyTank,&tank);
			}
		}

		//敌方坦克线程
		if (clock() - time4EnemyTank >= g_levelEneTank)
		{
			for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
			{
				time4EnemyTank = clock();
				COORD oldCore = enemyTank[i].core;
				COORD oldBody[5] = { enemyTank[i].body[0],enemyTank[i].body[1],enemyTank[i].body[2],enemyTank[i].body[3],enemyTank[i].body[4] };
				ManipulateEneTank(&enemyTank[i], 敌方坦克,&tank,enemyTank);
				CleanTankTail(oldCore, oldBody);
				DrawTank(&enemyTank[i], 敌方坦克);
			}
		}

		//敌方子弹线程
		if (clock() - time4EnemyBullet >= g_levelEneBul)
		{
			for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
			{
				if (enemyTank[i].bullet.state != 不存在)
				{
					//子弹赋值
					if (enemyTank[i].bullet.state == 未赋值)//==1未赋值状态
					{
						enemyTank[i].bullet = { {enemyTank[i].body[0].X, enemyTank[i].body[0].Y}, enemyTank[i].dir };
						enemyTank[i].bullet.state = 已赋值;
					}
					//子弹移动
					time4EnemyBullet = clock();
					COORD oldBulCore = enemyTank[i].bullet.core;
					MoveBullet(&enemyTank[i].bullet);
					CleanBullet(oldBulCore);
					DrawBullet(&enemyTank[i].bullet, &enemyTank[i]);
					IsEneBulMeetOther(&enemyTank[i].bullet, enemyTank,&tank);
				}
			}
		}

		//判断游戏结束
		if (tank.blood == 0 || GetLiveEnemyAmount(enemyTank) == 0)
		{
			GameOver(enemyTank);
			break;
		}
	}

	// 消耗多余字符
	char ch = _getch();
	ch = _getch();
	return 0;
}