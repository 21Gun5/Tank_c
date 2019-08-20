#include <stdio.h>
#include <time.h>
#include "Data.h"
#include "Func.h"
#include <conio.h>

int main()
{
	//初始化
	GameInit();
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
	for (int i = 0; i < ENEMY_TANK_AMOUNT; i++) { SetTankShape(&enemyTank[i]); }//设置形态

	//流程控制
	int indexForPlay = 0;		//菜单索引
	bool IsSelectedPlay = 0;	//是否选择结束
	while (true)
	{
		DrawMenu(menuPlay, _countof(menuPlay), indexForPlay);
		int play = SelectMenu(_countof(menuPlay), &indexForPlay);
		if (play == ENTER)
		{
			switch (indexForPlay)
			{
			case 开始游戏:
			{
				int indexForWhoMap = 0;//菜单索引
				bool IsSelectedWhoMap = 0; //选择的循环结束
				while (true)
				{
					DrawMenu(menuWhoMap, _countof(menuWhoMap), indexForWhoMap);
					int whoMap = SelectMenu(_countof(menuWhoMap), &indexForWhoMap);
					if (whoMap == ENTER)
					{
						switch (indexForWhoMap)
						{
						case 系统默认:
						{
							int indexForLevel = 0;
							bool IsSelectedLevel = 0;
							while (true)
							{
								DrawMenu(menuLevel, _countof(menuLevel), indexForLevel);
								int level = SelectMenu(_countof(menuLevel), &indexForLevel);
								if (level == ENTER)
								{
									switch (indexForLevel)
									{
									case 简单:
										g_levelEneTank = 300;
										g_levelEneBul = 90;
										IsSelectedLevel = 1;
										break;
									case 一般:
										g_levelEneTank = 200;
										g_levelEneBul = 70;
										IsSelectedLevel = 1;
										break;
									case 困难:
										g_levelEneTank = 100;
										g_levelEneBul = 50;
										IsSelectedLevel = 1;
										break;
									default:
										break;
									}
								}
								if (IsSelectedLevel == 1)
									break;
							}
							SetDefaultMap();//使用默认的
							IsSelectedPlay = 1;
							IsSelectedWhoMap = 1;
							g_isRunning = 1;//游戏运行
							break;
						}
						case 玩家提供:
						{
							int indexForWhenMap = 0;//菜单索引
							bool IsSelectedWhenMap = 0; //选择的循环结束
							while (true)
							{
								DrawMenu(menuWhenMap, _countof(menuWhenMap), indexForWhenMap);
								int whenMap = SelectMenu(_countof(menuWhenMap), &indexForWhenMap);
								if (whenMap == ENTER)
								{
									switch (indexForWhenMap)
									{
									case 新建地图:
										IsSelectedPlay = 1;
										IsSelectedWhoMap = 1;
										IsSelectedWhenMap = 1;
										g_isRunning = 1;//要进入游戏了
										CustomizeMap(&tank, enemyTank);//手动设置并直接使用
										break;
									case 已有地图:
									{
										IsSelectedPlay = 1;
										IsSelectedWhoMap = 1;
										IsSelectedWhenMap = 1;
										g_isRunning = 1;//要进入游戏了
										char* mapFile = ShowMapFile();
										LoadMapFile(mapFile);//导入已有地图
										break;
									}
									case 返回上页:
									{
										IsSelectedWhenMap = 1;
										break;
									}
									default:
										break;
									}
								}
								if (IsSelectedWhenMap == 1)
									break;
							}
							break;
						}
						case 返回上页:
						{
							IsSelectedWhoMap = 1;
							break;
						}
						default:
							break;
						}
					}
					if (IsSelectedWhoMap == 1)
						break;
				}
				break;
			}
			case 读取游戏:
			{
				IsSelectedPlay = 1;
				char* gameFile = ShowGameFile();
				LoadGame(&tank, enemyTank, gameFile);
				break;
			}
			case 退出游戏:
				return 0;
			default:
				break;
			}
		}
		if (IsSelectedPlay)//选择结束，进入游戏
			break;
	}

	//边界及障碍
	DrawBorder();
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
			DrawTank(&tank, 我方坦克);
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
				DrawBullet(&tank.bullet, &tank);
				IsMyBulMeetOther(&tank.bullet, enemyTank, &tank);
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
				ManipulateEneTank(&enemyTank[i], 敌方坦克, &tank, enemyTank);
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
					IsEneBulMeetOther(&enemyTank[i].bullet, enemyTank, &tank);
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

	//消耗多余字符
	char ch = _getch();
	ch = _getch();

	return 0;
}