#include "Func.h"
#include "Data.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#pragma comment(lib,"winmm.lib")

//系统功能
void GotoxyAndPrint(unsigned x, unsigned y, const char* str)
{
	COORD cur;
	cur.X = x * 2;
	cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
	printf(str);
}
void SetCursorState(bool b)
{

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);	//获取控制台光标信息
	CursorInfo.bVisible = b;					//显示/隐藏控制台光标
	SetConsoleCursorInfo(handle, &CursorInfo);	//设置控制台光标状态
}
void setColor(unsigned short ForeColor, unsigned short BackGroundColor)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);					//获取当前窗口句柄
	SetConsoleTextAttribute(handle, ForeColor + BackGroundColor * 0x10);//设置颜色
}
void PlayBGM()
{
	// 打开音频文件（死亡时关闭
	mciSendString("open conf/BGM.mp3 alias bgm", NULL, 0, NULL);//别名不可大写
	mciSendString("play bgm repeat", NULL, 0, NULL);			// 循环播放,适用于.mp3格式
}

//游戏相关
void GameInit()
{
	//设置地图
	for (int x = 0; x < MAP_X/2; x++)
	{
		for (int y = 0; y < MAP_Y; y++)
		{
			//地图边界
			if (x == 0 || x == MAP_X/2-1 || x == MAP_X_WALL/2 ||	//三竖边
				y == 0 || y == MAP_Y - 1 ||							//两横边
				(x > MAP_X_WALL/2 && y == MAP_Y / 2))				//帮助信息与游戏信息分割线
			{
				g_MAP[x][y] = 地图边界;
			}
		}
	}

	//隐藏光标
	SetCursorState(false);

	//设置标题
	system("title Tank");

	//初始化随机数种子
	srand((unsigned int)time(0));

	//播放背景音乐
	//PlayBGM();//先不放，烦人
}
void GameOver(PTANK penemytank)
{
	//存活敌坦数量
	int eneTankCount = GetLiveEnemyAmount(penemytank);

	//关闭音乐文件
	mciSendString("close bgm", NULL, 0, NULL);	//close关闭而非stop停止

	//提示信息
	setColor(12, 0);
	GotoxyAndPrint(MAP_X_WALL / 4 - 2, MAP_Y / 2 - 7, "GAME OVER! ");

	if (eneTankCount == 0)
		GotoxyAndPrint(MAP_X_WALL / 4 - 2, MAP_Y / 2 - 5, "A Winner!");
	else
		GotoxyAndPrint(MAP_X_WALL / 4 - 2, MAP_Y / 2 - 5, "A Loser!");
	GotoxyAndPrint(MAP_X_WALL / 4 -2, MAP_Y / 2 - 3,"");

	printf("Scores: %d", ENEMY_TANK_AMOUNT-eneTankCount);
	setColor(7, 0);

}

void DrawMapBorder()
{
	system("cls");						//换页则清屏
	//setColor(8, 0);
	for (int x = 0; x < MAP_X; x++)
	{
		for (int y = 0; y < MAP_Y; y++)
		{
			if (g_MAP[x][y] == 地图边界)
			{
				GotoxyAndPrint(x, y, "■");
			}
		}
	}
	//setColor(7, 0);
}
void DrawWelcome()
{
	//打印图案
	GotoxyAndPrint(MAP_X / 4 - 9, MAP_Y / 2 - 15, "MMP\"\"MM\"\"YMM                      `7MM      ");
	GotoxyAndPrint(MAP_X / 4 - 9, MAP_Y / 2 - 14, "P'   MM   `7                        MM      ");
	GotoxyAndPrint(MAP_X / 4 - 9, MAP_Y / 2 - 13, "     MM       ,6\"Yb.  `7MMpMMMb.    MM, MP'");
	GotoxyAndPrint(MAP_X / 4 - 9, MAP_Y / 2 - 12, "     MM      8)   MM    MM    MM    MM ;Y   ");
	GotoxyAndPrint(MAP_X / 4 - 9, MAP_Y / 2 - 11, "     MM       ,pm9MM    MM    MM    MM;Mm   ");
	GotoxyAndPrint(MAP_X / 4 - 9, MAP_Y / 2 - 10, "     MM      8M   MM    MM    MM    MM `Mb. ");
	GotoxyAndPrint(MAP_X / 4 - 9, MAP_Y / 2 - 9,  "   .JMML.    `Moo9^Yo..JMML  JMML..JMML. YA.");
	//提示信息
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 6, "1. 新游戏\n");
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 4, "2. 读取游戏\n");
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 2, "3. 退出游戏\n");
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 0, "请输入选择-> ");
}
void DrawGameInfo(PTANK ptank, PTANK penemytank)
{
	//存活敌坦数量
	int eneTankCount = GetLiveEnemyAmount(penemytank);

	//运行or暂停状态
	setColor(12, 0);
	GotoxyAndPrint(MAP_X / 2 - 14, 1, "RUNNING");
	GotoxyAndPrint(MAP_X / 2 - 14, 2, "Q: 暂停游戏");
	setColor(7, 0);

	//游戏信息打印
	setColor(12, 0);
	GotoxyAndPrint(MAP_X / 2 - 11, 5, "");
	printf("当前生命: %d", ptank->blood);
	GotoxyAndPrint(MAP_X / 2 - 11, 7, "");
	printf("当前分数: %d", ENEMY_TANK_AMOUNT - eneTankCount);
	GotoxyAndPrint(MAP_X / 2 - 11, 9, "");
	printf("敌坦个数: %d", eneTankCount);
	setColor(7, 0);
}
void DrawGameHelp()
{
	setColor(12, 0);
	GotoxyAndPrint(MAP_X / 2 - 10, 18, "操作说明");
	GotoxyAndPrint(MAP_X / 2 - 11, 20, "W: 上  S: 下");
	GotoxyAndPrint(MAP_X / 2 - 11, 22, "A: 左  D: 右");
	GotoxyAndPrint(MAP_X / 2 - 11, 24, "Q:  暂停游戏");
	GotoxyAndPrint(MAP_X / 2 - 11, 26, "空格: 开火");
	
	setColor(7, 0);
}

int SelectAction()
{
	SetCursorState(true);			//用户输入时，光标可见
	//int input = _getch() - '0';		//无回显接收；-'0'保证在0-9，而非ASCII
	int input = 1;
	SetCursorState(false);			//输入完隐藏光标

	switch (input)
	{
	case 开始游戏:
	{
		g_isRunning = true;
		break;
	}
	case 读取游戏:
	{
		GotoxyAndPrint(MAP_X / 2 - 25, MAP_Y / 2 + 3, "敬请期待！\n");
		break;
	}
	case 退出游戏:
	{
		GotoxyAndPrint(MAP_X / 2 - 25, MAP_Y / 2 + 3, "Bye! \n");
		break;
	}
	default:
		GotoxyAndPrint(MAP_X / 2 - 25, MAP_Y / 2 + 3, "输入错误\n");
		break;
	}
	return input;
}
int SelectWhoMap()
{
	//提示信息
	system("cls");				//换页则清屏
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 6, "请选择地图：");
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 4, "1. 系统默认");
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 2, "2. 玩家提供");
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2,"请输入选择-> ");

	SetCursorState(true);		//用户输入前显示光标
	//int input = _getch() - '0';	//控制其0-9，而非ASCII
	int input = 1;
	SetCursorState(false);		//输入结束后隐藏光标

	return input;
}
int SelectWhenMap()
{
	//提示信息
	system("cls");
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 6, "地图选择");
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 4, "1. 新建地图");
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 2, "2. 已有地图");
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2,"请输入选择-> ");

	SetCursorState(true);//显示光标
	int input = _getch() - '0';
	//int input = 1;
	SetCursorState(false);//隐藏光标

	return input;
}

//坦克相关
void CleanTankTail(COORD oldCore, PCOORD oldBody)
{
	GotoxyAndPrint(oldCore.X, oldCore.Y, "  ");//中心点
	for (int i = 0; i < 5; i++)//其他点
	{
		GotoxyAndPrint(oldBody[i].X, oldBody[i].Y, "  ");
	}
}
void DrawTank(PTANK ptank, int who)
{
	if (ptank->isAlive == false) return;
	if (who == 我方坦克)
	{
		setColor(10, 0);
		GotoxyAndPrint(ptank->core.X, ptank->core.Y, "■");//中心点
		for (int i = 0; i < 5; i++)//其他点
		{
			GotoxyAndPrint(ptank->body[i].X, ptank->body[i].Y, "■");
		}
		setColor(7, 0);
	}
	else if (who == 敌方坦克)
	{
		setColor(11, 0);//亮蓝色
		if (ptank->blood == 2)
		{
			GotoxyAndPrint(ptank->core.X, ptank->core.Y, "■");//中心点
			for (int i = 0; i < 5; i++)//其他点
				GotoxyAndPrint(ptank->body[i].X, ptank->body[i].Y, "■");//中心点
		}
		else if (ptank->blood == 1)
		{
			GotoxyAndPrint(ptank->core.X, ptank->core.Y, "□");//中心点
			for (int i = 0; i < 5; i++)//其他点
				GotoxyAndPrint(ptank->body[i].X, ptank->body[i].Y, "□");
		}
		setColor(7, 0);
	}
}
void SetTankShape(PTANK ptank)
{
	if (ptank->isAlive == false) return;
	if (ptank->dir == UP)
	{
		ptank->body[0] = { ptank->core.X, ptank->core.Y - 1 };
		ptank->body[1] = { ptank->core.X - 1, ptank->core.Y };
		ptank->body[2] = { ptank->core.X + 1, ptank->core.Y };
		ptank->body[3] = { ptank->core.X - 1, ptank->core.Y + 1 };
		ptank->body[4] = { ptank->core.X + 1, ptank->core.Y + 1 };
	}
	else if (ptank->dir == DOWN)
	{
		ptank->body[0] = { ptank->core.X, ptank->core.Y + 1 };
		ptank->body[1] = { ptank->core.X - 1, ptank->core.Y };
		ptank->body[2] = { ptank->core.X + 1, ptank->core.Y };
		ptank->body[3] = { ptank->core.X - 1, ptank->core.Y - 1 };
		ptank->body[4] = { ptank->core.X + 1, ptank->core.Y - 1 };
	}
	else if (ptank->dir == LEFT)
	{
		ptank->body[0] = { ptank->core.X - 1, ptank->core.Y };
		ptank->body[1] = { ptank->core.X , ptank->core.Y + 1 };
		ptank->body[2] = { ptank->core.X , ptank->core.Y - 1 };
		ptank->body[3] = { ptank->core.X + 1, ptank->core.Y + 1 };
		ptank->body[4] = { ptank->core.X + 1, ptank->core.Y - 1 };
	}
	else if (ptank->dir == RIGHT)
	{
		ptank->body[0] = { ptank->core.X + 1, ptank->core.Y };
		ptank->body[1] = { ptank->core.X , ptank->core.Y + 1 };
		ptank->body[2] = { ptank->core.X , ptank->core.Y - 1 };
		ptank->body[3] = { ptank->core.X - 1, ptank->core.Y + 1 };
		ptank->body[4] = { ptank->core.X - 1, ptank->core.Y - 1 };
	}
}
int GetLiveEnemyAmount(PTANK penemytank)
{
	int count = 0;
	for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
	{
		if (penemytank[i].isAlive)
			count++;
	}
	return count;
}

void ManipulateTank(PTANK ptank, int who, PTANK penemytank)
{
	if (ptank->isAlive == false) return;
	if (who == 我方坦克)
	{
		char ch = 0;
		if (_kbhit())				//非阻塞函数 
		{
			ch = _getch();			//无回显接受输入
			switch (ch)
			{
			case 'w':
				if (!IsTankMeetOther(ptank, UP, penemytank))
					ptank->core.Y--;
				ptank->dir = UP;
				break;
			case 's':
				if (!IsTankMeetOther(ptank, DOWN, penemytank))
					ptank->core.Y++;
				ptank->dir = DOWN;
				break;
			case 'a':
				if (!IsTankMeetOther(ptank, LEFT, penemytank))
					ptank->core.X--;
				ptank->dir = LEFT;
				break;
			case 'd':
				if (!IsTankMeetOther(ptank, RIGHT, penemytank))
					ptank->core.X++;
				ptank->dir = RIGHT;
				break;
			case ' ':
				if(ptank->bullet.state != 已赋值)
					ptank->bullet.state = 未赋值;//已赋值即在跑时，再开火，不可赋值为1，应该消失为0时，按键才生效
				break;
			case 'q':
			{
				//暂停及恢复
				mciSendString("pause bgm", NULL, 0, NULL);	//暂停bgm
				setColor(12, 0);
				GotoxyAndPrint(MAP_X / 2 - 14, 1, "       ");//先把较长的running清空
				GotoxyAndPrint(MAP_X / 2 - 14, 1, "PAUSE");
				GotoxyAndPrint(MAP_X / 2 - 14, 2, "1. 回到游戏");
				GotoxyAndPrint(MAP_X / 2 - 14, 3, "2. 退出游戏");
				char tmp;
				do
				{
					tmp = _getch();	//利用阻塞函数暂停游戏
				} while (!(tmp == '1' || tmp == '2'));//只有输入12才可

				switch (tmp)
				{
				case '1'://恢复游戏
				{
					mciSendString("resume bgm", NULL, 0, NULL);//恢复bgm
					GotoxyAndPrint(MAP_X / 2 - 14, 1, "RUNNING");
					GotoxyAndPrint(MAP_X / 2 - 14, 2, "Q: 暂停游戏");
					GotoxyAndPrint(MAP_X / 2 - 14, 3, "           ");
					GotoxyAndPrint(MAP_X / 2 - 14, 4, "           ");
					break;
				}
				case '2'://退出游戏
				{
					GameOver(penemytank);
					g_isRunning = false;
					break;
				}
				default:
					break;
				}
				break;
			}
			default:
				break;
			}
		}
	}

	SetTankShape(ptank);//每次移动后都要重新设置形态
}
void ManipulateTank2(PTANK ptank, int who, PTANK pmytank, PTANK penemytank)
{
	if (ptank->isAlive == false) return;
	if (who == 敌方坦克)
	{
		switch (rand() % 5)
		{
		case UP:
			if (!IsTankMeetOther2(ptank, UP, pmytank, penemytank))
				ptank->core.Y--;
			ptank->dir = UP;
			break;
		case DOWN:
			if (!IsTankMeetOther2(ptank, DOWN, pmytank, penemytank))
				ptank->core.Y++;
			ptank->dir = DOWN;
			break;
		case LEFT:
			if (!IsTankMeetOther2(ptank, LEFT, pmytank, penemytank))
				ptank->core.X--;
			ptank->dir = LEFT;
			break;
		case RIGHT:
			if (!IsTankMeetOther2(ptank, RIGHT, pmytank, penemytank))
				ptank->core.X++;
			ptank->dir = RIGHT;
			break;
		case 4:
			if (ptank->bullet.state != 已赋值)
				ptank->bullet.state = 未赋值;
			break;
		default:
			break;
		}
	}
	SetTankShape(ptank);//每次移动后都要重新设置形态
}
bool IsTankMeetOther(PTANK ptank,int dir, PTANK penemytank)
{
	switch (dir)
	{
	case UP:
		//是否撞边界
		if (ptank->core.Y <= 2)
		{
			return true;
		}
		//是否撞障碍物
		if ((g_MAP[ptank->core.X][ptank->core.Y - 2] == 土块障碍物||
			g_MAP[ptank->core.X-1][ptank->core.Y - 2] == 土块障碍物 ||
			g_MAP[ptank->core.X+1][ptank->core.Y - 2] == 土块障碍物) ||
			(g_MAP[ptank->core.X][ptank->core.Y - 2] == 石块障碍物 ||
			g_MAP[ptank->core.X - 1][ptank->core.Y - 2] == 石块障碍物 ||
			g_MAP[ptank->core.X + 1][ptank->core.Y - 2] == 石块障碍物))
		{
			return true;
		}
		//是否撞敌方坦克
		for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
		{
			if(penemytank[i].isAlive ==false) continue;
			if (
				((ptank->core.X == penemytank[i].core.X) && (ptank->core.Y - penemytank[i].core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X - 1) && (ptank->core.Y - penemytank[i].core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X - 2) && (ptank->core.Y - penemytank[i].core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X + 1) && (ptank->core.Y - penemytank[i].core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X + 2) && (ptank->core.Y - penemytank[i].core.Y == 3))
				)//要==3,而非<=，只有在挨着的时候可被当，如果小于，虽没挨着敌坦1，但距离却小于2，被2干扰
			{
				return true;
			}
		}
		
		break;
	case DOWN:
		//是否撞边界
		if (ptank->core.Y >= MAP_Y - 3)
		{
			return true;
		}
		//是否撞障碍物
		if ((g_MAP[ptank->core.X][ptank->core.Y + 2] == 土块障碍物 ||
			g_MAP[ptank->core.X - 1][ptank->core.Y + 2] == 土块障碍物 ||
			g_MAP[ptank->core.X + 1][ptank->core.Y + 2] == 土块障碍物) ||
			(g_MAP[ptank->core.X][ptank->core.Y + 2] == 石块障碍物 ||
			g_MAP[ptank->core.X - 1][ptank->core.Y + 2] == 石块障碍物 ||
			g_MAP[ptank->core.X + 1][ptank->core.Y + 2] == 石块障碍物))
		{
			return true;
		}
		//是否撞敌方坦克
		for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
		{
			if (penemytank[i].isAlive == false) continue;
			if (
				((ptank->core.X == penemytank[i].core.X) && (penemytank[i].core.Y - ptank->core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X - 1) && (penemytank[i].core.Y - ptank->core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X - 2) && (penemytank[i].core.Y - ptank->core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X + 1) && (penemytank[i].core.Y - ptank->core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X + 2) && (penemytank[i].core.Y - ptank->core.Y == 3))
				)
			{
				return true;
			}
		}
		break;
	case LEFT:
		//是否撞边界
		if (ptank->core.X <= 2)
		{
			return true;
		}
		//是否撞障碍物
		if ((g_MAP[ptank->core.X-2][ptank->core.Y] == 土块障碍物 ||
			g_MAP[ptank->core.X - 2][ptank->core.Y -1] == 土块障碍物 ||
			g_MAP[ptank->core.X -2][ptank->core.Y + 1] == 土块障碍物)||
			(g_MAP[ptank->core.X - 2][ptank->core.Y] == 石块障碍物 ||
			g_MAP[ptank->core.X - 2][ptank->core.Y - 1] == 石块障碍物 ||
			g_MAP[ptank->core.X - 2][ptank->core.Y + 1] == 石块障碍物))
		{
			return true;
		}
		//是否撞敌方坦克
		for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
		{
			if (penemytank[i].isAlive == false) continue;
			if (
				((ptank->core.Y == penemytank[i].core.Y) && (ptank->core.X - penemytank[i].core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y - 1) && (ptank->core.X - penemytank[i].core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y - 2) && (ptank->core.X - penemytank[i].core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y + 1) && (ptank->core.X - penemytank[i].core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y + 2) && (ptank->core.X - penemytank[i].core.X == 3))
				)
			{
				return true;
			}
		}
		break;
	case RIGHT:
		//是否撞边界
		if (ptank->core.X >= MAP_X_WALL / 2 - 2)
		{
			return true;
		}
		//是否撞障碍物
		if ((g_MAP[ptank->core.X + 2][ptank->core.Y] == 土块障碍物 ||
			g_MAP[ptank->core.X + 2][ptank->core.Y - 1] == 土块障碍物 ||
			g_MAP[ptank->core.X + 2][ptank->core.Y + 1] == 土块障碍物) ||
			(g_MAP[ptank->core.X + 2][ptank->core.Y] == 石块障碍物 ||
			g_MAP[ptank->core.X + 2][ptank->core.Y - 1] == 石块障碍物 ||
			g_MAP[ptank->core.X + 2][ptank->core.Y + 1] == 石块障碍物))
		{
			return true;
		}
		//是否撞敌方坦克
		for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
		{
			if (penemytank[i].isAlive == false) continue;
			if (
				((ptank->core.Y == penemytank[i].core.Y) && (penemytank[i].core.X - ptank->core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y - 1) && (penemytank[i].core.X - ptank->core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y - 2) && (penemytank[i].core.X - ptank->core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y + 1) && (penemytank[i].core.X - ptank->core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y + 2) && (penemytank[i].core.X - ptank->core.X == 3))
				)
			{
				return true;
			}
		}
		break;
	default:
		break;
	}
	return false;
}
bool IsTankMeetOther2(PTANK ptank, int dir,  PTANK pmytank, PTANK penemytank)
{
	switch (dir)
	{
	case UP:
		//是否撞边界
		if (ptank->core.Y <= 2)
		{
			return true;
		}
		//是否撞障碍物
		if ((g_MAP[ptank->core.X][ptank->core.Y - 2] == 土块障碍物 ||
			g_MAP[ptank->core.X - 1][ptank->core.Y - 2] == 土块障碍物 ||
			g_MAP[ptank->core.X + 1][ptank->core.Y - 2] == 土块障碍物) ||
			(g_MAP[ptank->core.X][ptank->core.Y - 2] == 石块障碍物 ||
			g_MAP[ptank->core.X - 1][ptank->core.Y - 2] == 石块障碍物 ||
			g_MAP[ptank->core.X + 1][ptank->core.Y - 2] == 石块障碍物))
		{
			return true;
		}
		//是否撞我方坦克
		if (
			((ptank->core.X == pmytank->core.X - 0) && (ptank->core.Y - pmytank->core.Y == 3)) ||
			((ptank->core.X == pmytank->core.X - 1) && (ptank->core.Y - pmytank->core.Y == 3)) ||
			((ptank->core.X == pmytank->core.X - 2) && (ptank->core.Y - pmytank->core.Y == 3)) ||
			((ptank->core.X == pmytank->core.X + 1) && (ptank->core.Y - pmytank->core.Y == 3)) ||
			((ptank->core.X == pmytank->core.X + 2) && (ptank->core.Y - pmytank->core.Y == 3))
			)//要==3,而非<=，只有在挨着的时候可被当，如果小于，虽没挨着敌坦1，但距离却小于2，被2干扰
		{
			return true;
		}
		//是否撞其他敌方坦克
		for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
		{
			if (ptank->core.X == penemytank[i].core.X && ptank->core.Y == penemytank[i].core.Y)//排除自己
				continue;
			if (
				((ptank->core.X == penemytank[i].core.X) && (ptank->core.Y - penemytank[i].core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X - 1) && (ptank->core.Y - penemytank[i].core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X - 2) && (ptank->core.Y - penemytank[i].core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X + 1) && (ptank->core.Y - penemytank[i].core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X + 2) && (ptank->core.Y - penemytank[i].core.Y == 3))
				)//要==3,而非<=，只有在挨着的时候可被当，如果小于，虽没挨着敌坦1，但距离却小于2，被2干扰
			{
				return true;
			}
		}

		break;
	case DOWN:
		//是否撞边界
		if (ptank->core.Y >= MAP_Y - 3)
		{
			return true;
		}
		//是否撞障碍物
		if ((g_MAP[ptank->core.X][ptank->core.Y + 2] == 土块障碍物 ||
			g_MAP[ptank->core.X - 1][ptank->core.Y + 2] == 土块障碍物 ||
			g_MAP[ptank->core.X + 1][ptank->core.Y + 2] == 土块障碍物) ||
			(g_MAP[ptank->core.X][ptank->core.Y + 2] == 石块障碍物 ||
			g_MAP[ptank->core.X - 1][ptank->core.Y + 2] == 石块障碍物 ||
			g_MAP[ptank->core.X + 1][ptank->core.Y + 2] == 石块障碍物))
		{
			return true;
		}
		//是否撞我方坦克
		if (
			((ptank->core.X == pmytank->core.X - 0) && (pmytank->core.Y - ptank->core.Y == 3)) ||
			((ptank->core.X == pmytank->core.X - 1) && (pmytank->core.Y - ptank->core.Y == 3)) ||
			((ptank->core.X == pmytank->core.X - 2) && (pmytank->core.Y - ptank->core.Y == 3)) ||
			((ptank->core.X == pmytank->core.X + 1) && (pmytank->core.Y - ptank->core.Y == 3)) ||
			((ptank->core.X == pmytank->core.X + 2) && (pmytank->core.Y - ptank->core.Y == 3))
			)
		{
			return true;
		}
		//是否撞其他敌方坦克
		for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
		{
			if (ptank->core.X == penemytank[i].core.X && ptank->core.Y == penemytank[i].core.Y)//排除自己
				continue;
			if (
				((ptank->core.X == penemytank[i].core.X) && (penemytank[i].core.Y - ptank->core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X - 1) && (penemytank[i].core.Y - ptank->core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X - 2) && (penemytank[i].core.Y - ptank->core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X + 1) && (penemytank[i].core.Y - ptank->core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X + 2) && (penemytank[i].core.Y - ptank->core.Y == 3))
				)
			{
				return true;
			}
		}
		break;
	case LEFT:
		//是否撞边界
		if (ptank->core.X <= 2)
		{
			return true;
		}
		//是否撞障碍物
		if ((g_MAP[ptank->core.X - 2][ptank->core.Y] == 土块障碍物 ||
			g_MAP[ptank->core.X - 2][ptank->core.Y - 1] == 土块障碍物 ||
			g_MAP[ptank->core.X - 2][ptank->core.Y + 1] == 土块障碍物) ||
			(g_MAP[ptank->core.X - 2][ptank->core.Y] == 石块障碍物 ||
			g_MAP[ptank->core.X - 2][ptank->core.Y - 1] == 石块障碍物 ||
			g_MAP[ptank->core.X - 2][ptank->core.Y + 1] == 石块障碍物))
		{
			return true;
		}
		//是否撞我方坦克
		if (
			((ptank->core.Y == pmytank->core.Y - 0) && (ptank->core.X - pmytank->core.X == 3)) ||
			((ptank->core.Y == pmytank->core.Y - 1) && (ptank->core.X - pmytank->core.X == 3)) ||
			((ptank->core.Y == pmytank->core.Y - 2) && (ptank->core.X - pmytank->core.X == 3)) ||
			((ptank->core.Y == pmytank->core.Y + 1) && (ptank->core.X - pmytank->core.X == 3)) ||
			((ptank->core.Y == pmytank->core.Y + 2) && (ptank->core.X - pmytank->core.X == 3))
			)
		{
			return true;
		}
		//是否撞其他敌方坦克
		for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
		{
			if (ptank->core.X == penemytank[i].core.X && ptank->core.Y == penemytank[i].core.Y)//排除自己
				continue;
			if (
				((ptank->core.Y == penemytank[i].core.Y) && (ptank->core.X - penemytank[i].core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y - 1) && (ptank->core.X - penemytank[i].core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y - 2) && (ptank->core.X - penemytank[i].core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y + 1) && (ptank->core.X - penemytank[i].core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y + 2) && (ptank->core.X - penemytank[i].core.X == 3))
				)
			{
				return true;
			}
		}
		break;
	case RIGHT:
		//是否撞边界
		if (ptank->core.X >= MAP_X_WALL / 2 - 2)
		{
			return true;
		}
		//是否撞障碍物
		if ((g_MAP[ptank->core.X + 2][ptank->core.Y] == 土块障碍物 ||
			g_MAP[ptank->core.X + 2][ptank->core.Y - 1] == 土块障碍物 ||
			g_MAP[ptank->core.X + 2][ptank->core.Y + 1] == 土块障碍物) ||
			(g_MAP[ptank->core.X + 2][ptank->core.Y] == 石块障碍物 ||
			g_MAP[ptank->core.X + 2][ptank->core.Y - 1] == 石块障碍物 ||
			g_MAP[ptank->core.X + 2][ptank->core.Y + 1] == 石块障碍物))
		{
			return true;
		}
		//是否撞我方坦克
		if (
			((ptank->core.Y == pmytank->core.Y - 0) && (pmytank->core.X - ptank->core.X == 3)) ||
			((ptank->core.Y == pmytank->core.Y - 1) && (pmytank->core.X - ptank->core.X == 3)) ||
			((ptank->core.Y == pmytank->core.Y - 2) && (pmytank->core.X - ptank->core.X == 3)) ||
			((ptank->core.Y == pmytank->core.Y + 1) && (pmytank->core.X - ptank->core.X == 3)) ||
			((ptank->core.Y == pmytank->core.Y + 2) && (pmytank->core.X - ptank->core.X == 3))
			)
		{
			return true;
		}
		//是否撞其他敌方坦克
		for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
		{
			if (ptank->core.X == penemytank[i].core.X && ptank->core.Y == penemytank[i].core.Y)//排除自己
				continue;
			if (
				((ptank->core.Y == penemytank[i].core.Y) && (penemytank[i].core.X - ptank->core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y - 1) && (penemytank[i].core.X - ptank->core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y - 2) && (penemytank[i].core.X - ptank->core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y + 1) && (penemytank[i].core.X - ptank->core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y + 2) && (penemytank[i].core.X - ptank->core.X == 3))
				)
			{
				return true;
			}
		}
		break;
	default:
		break;
	}
	return false;
}


//子弹相关
void MoveBullet(PBULLET pbullet)
{
	switch (pbullet->dir)
	{
	case UP:
		pbullet->core.Y--;
		break;
	case DOWN:
		pbullet->core.Y++;
		break;
	case LEFT:
		pbullet->core.X--;
		break;
	case RIGHT:
		pbullet->core.X++;
		break;
	default:
		break;
	}


}
void CleanBullet(COORD oldBulCore)
{
	GotoxyAndPrint(oldBulCore.X, oldBulCore.Y, "  ");
	//printf("  ");
}
void DrawBullet(PBULLET pbullet,PTANK ptank)
{
	//碰到边界，换成边界的颜色，实现子弹消失的效果
	if (pbullet->core.X <= 0 ||
		pbullet->core.X >= MAP_X_WALL / 2 ||
		pbullet->core.Y <= 0 ||
		pbullet->core.Y >= MAP_Y - 1)
	{
		setColor(7, 0);
	}
	else
	{
		if(ptank->who == 我方坦克)
			setColor(10, 0);
		else if(ptank->who == 敌方坦克)
			setColor(11, 0);
	}

	//碰到障碍，将子弹画为空格，实现子弹消失
	if (g_MAP[pbullet->core.X][pbullet->core.Y] == 土块障碍物)
	{
		GotoxyAndPrint(pbullet->core.X, pbullet->core.Y, "  ");
	}
	//碰到石块障碍物，换成其颜色，实现子弹消失的效果
	else if (g_MAP[pbullet->core.X][pbullet->core.Y] == 石块障碍物)
	{
		setColor(7, 0);
	}
	//一般运动状态
	else
	{
		GotoxyAndPrint(pbullet->core.X, pbullet->core.Y, "■");
	}	
	setColor(7, 0);
}

void IsBulMeetOther(PBULLET pbullet, PTANK penemytank)
{
	//是否遇到边界
	if (pbullet->core.X <= 0 ||
		pbullet->core.X >= MAP_X_WALL / 2 ||
		pbullet->core.Y <= 0 ||
		pbullet->core.Y >= MAP_Y - 1)
	{
		//PlaySound(TEXT("conf/duang.wav"), NULL, SND_FILENAME | SND_ASYNC);//播放音效
		pbullet->state = 不存在;
	}
	//是否遇到土块障碍物
	if (g_MAP[pbullet->core.X][pbullet->core.Y] == 土块障碍物)
	{
		//PlaySound(TEXT("conf/duang.wav"), NULL, SND_FILENAME | SND_ASYNC);//播放音效
		pbullet->state = 不存在;
		g_MAP[pbullet->core.X][pbullet->core.Y] = 空地;
	}
	//是否遇到石块障碍物
	if (g_MAP[pbullet->core.X][pbullet->core.Y] == 石块障碍物)
	{
		pbullet->state = 不存在;
		//g_MAP[pbullet->core.X][pbullet->core.Y] = 空地;
	}
	//是否遇敌方坦克
	for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
	{
		if (penemytank[i].isAlive == false) continue;
		switch (pbullet->dir)
		{
		case UP:
			if (
				(pbullet->core.X == penemytank[i].core.X) && (pbullet->core.Y - penemytank[i].core.Y == 0)||
				(pbullet->core.X == penemytank[i].body[0].X) && (pbullet->core.Y - penemytank[i].body[0].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[1].X) && (pbullet->core.Y - penemytank[i].body[1].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[2].X) && (pbullet->core.Y - penemytank[i].body[2].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[3].X) && (pbullet->core.Y - penemytank[i].body[3].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[4].X) && (pbullet->core.Y - penemytank[i].body[4].Y == 0)
				)
			{
				PlaySound(TEXT("conf/duang.wav"), NULL, SND_FILENAME | SND_ASYNC);//播放音效
				pbullet->state = 不存在;
				penemytank[i].blood--;
				if (penemytank[i].blood == 0)//如果减血后为0
					penemytank[i].isAlive = false;//声明为死亡
			}
			break;
		case DOWN:
			if (
				(pbullet->core.X == penemytank[i].core.X) && (pbullet->core.Y - penemytank[i].core.Y == 0) ||
				(pbullet->core.X == penemytank[i].body[0].X) && (pbullet->core.Y - penemytank[i].body[0].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[1].X) && (pbullet->core.Y - penemytank[i].body[1].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[2].X) && (pbullet->core.Y - penemytank[i].body[2].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[3].X) && (pbullet->core.Y - penemytank[i].body[3].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[4].X) && (pbullet->core.Y - penemytank[i].body[4].Y == 0)
				)
			{
				PlaySound(TEXT("conf/duang.wav"), NULL, SND_FILENAME | SND_ASYNC);//播放音效
				pbullet->state = 不存在;
				penemytank[i].blood--;
				if (penemytank[i].blood == 0)//如果减血后为0
					penemytank[i].isAlive = false;//声明为死亡
			}
			break;
		case LEFT:
			if (
				(pbullet->core.X == penemytank[i].core.X) && (pbullet->core.Y - penemytank[i].core.Y == 0) ||
				(pbullet->core.X == penemytank[i].body[0].X) && (pbullet->core.Y - penemytank[i].body[0].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[1].X) && (pbullet->core.Y - penemytank[i].body[1].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[2].X) && (pbullet->core.Y - penemytank[i].body[2].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[3].X) && (pbullet->core.Y - penemytank[i].body[3].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[4].X) && (pbullet->core.Y - penemytank[i].body[4].Y == 0)
				)
			{
				PlaySound(TEXT("conf/duang.wav"), NULL, SND_FILENAME | SND_ASYNC);//播放音效
				pbullet->state = 不存在;
				penemytank[i].blood--;
				if (penemytank[i].blood == 0)//如果减血后为0
					penemytank[i].isAlive = false;//声明为死亡
			}
			break;
		case RIGHT:
			if (
				(pbullet->core.X == penemytank[i].core.X) && (pbullet->core.Y - penemytank[i].core.Y == 0) ||
				(pbullet->core.X == penemytank[i].body[0].X) && (pbullet->core.Y - penemytank[i].body[0].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[1].X) && (pbullet->core.Y - penemytank[i].body[1].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[2].X) && (pbullet->core.Y - penemytank[i].body[2].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[3].X) && (pbullet->core.Y - penemytank[i].body[3].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[4].X) && (pbullet->core.Y - penemytank[i].body[4].Y == 0)
				)
			{
				PlaySound(TEXT("conf/duang.wav"), NULL, SND_FILENAME | SND_ASYNC);//播放音效
				pbullet->state = 不存在;
				penemytank[i].blood--;
				if (penemytank[i].blood == 0)//如果减血后为0
					penemytank[i].isAlive = false;//声明为死亡
			}
			break;
		default:
			break;
		}
	}
}
void IsBulMeetOther2(PBULLET pbullet, PTANK penemytank, PTANK ptank)
{
	//是否遇到边界
	if (pbullet->core.X <= 0 ||
		pbullet->core.X >= MAP_X_WALL / 2 ||
		pbullet->core.Y <= 0 ||
		pbullet->core.Y >= MAP_Y - 1)
	{
		pbullet->state = 不存在;
	}
	//是否遇到障碍物
	if (g_MAP[pbullet->core.X][pbullet->core.Y] == 土块障碍物)
	{
		pbullet->state = 不存在;
		g_MAP[pbullet->core.X][pbullet->core.Y] = 空地;
	}
	//是否遇到石块障碍物
	if (g_MAP[pbullet->core.X][pbullet->core.Y] == 石块障碍物)
	{
		pbullet->state = 不存在;
	}

	//是否遇到我方坦克
	switch (pbullet->dir)
	{
	case UP:
		if (
			(pbullet->core.X == ptank->core.X) && (pbullet->core.Y - ptank->core.Y == 0) ||
			(pbullet->core.X == ptank->body[0].X) && (pbullet->core.Y - ptank->body[0].Y == 0) ||
			(pbullet->core.X == ptank->body[1].X) && (pbullet->core.Y - ptank->body[1].Y == 0) ||
			(pbullet->core.X == ptank->body[2].X) && (pbullet->core.Y - ptank->body[2].Y == 0) ||
			(pbullet->core.X == ptank->body[3].X) && (pbullet->core.Y - ptank->body[3].Y == 0) ||
			(pbullet->core.X == ptank->body[4].X) && (pbullet->core.Y - ptank->body[4].Y == 0)
			)
		{
			PlaySound(TEXT("conf/duang.wav"), NULL, SND_FILENAME | SND_ASYNC);//播放音效
			pbullet->state = 不存在;
			(ptank->blood)--;
			if (ptank->blood == 0)//如果减血后为0
				ptank->isAlive = false;//声明为死亡
		}
		break;
	case DOWN:
		if (
			(pbullet->core.X == ptank->core.X) && (pbullet->core.Y - ptank->core.Y == 0) ||
			(pbullet->core.X == ptank->body[0].X) && (pbullet->core.Y - ptank->body[0].Y == 0) ||
			(pbullet->core.X == ptank->body[1].X) && (pbullet->core.Y - ptank->body[1].Y == 0) ||
			(pbullet->core.X == ptank->body[2].X) && (pbullet->core.Y - ptank->body[2].Y == 0) ||
			(pbullet->core.X == ptank->body[3].X) && (pbullet->core.Y - ptank->body[3].Y == 0) ||
			(pbullet->core.X == ptank->body[4].X) && (pbullet->core.Y - ptank->body[4].Y == 0)
			)
		{
			PlaySound(TEXT("conf/duang.wav"), NULL, SND_FILENAME | SND_ASYNC);//播放音效
			pbullet->state = 不存在;
			(ptank->blood)--;
			if (ptank->blood == 0)//如果减血后为0
				ptank->isAlive = false;//声明为死亡
		}
		break;
	case LEFT:
		if (
			(pbullet->core.X == ptank->core.X) && (pbullet->core.Y - ptank->core.Y == 0) ||
			(pbullet->core.X == ptank->body[0].X) && (pbullet->core.Y - ptank->body[0].Y == 0) ||
			(pbullet->core.X == ptank->body[1].X) && (pbullet->core.Y - ptank->body[1].Y == 0) ||
			(pbullet->core.X == ptank->body[2].X) && (pbullet->core.Y - ptank->body[2].Y == 0) ||
			(pbullet->core.X == ptank->body[3].X) && (pbullet->core.Y - ptank->body[3].Y == 0) ||
			(pbullet->core.X == ptank->body[4].X) && (pbullet->core.Y - ptank->body[4].Y == 0)
			)
		{
			PlaySound(TEXT("conf/duang.wav"), NULL, SND_FILENAME | SND_ASYNC);//播放音效
			pbullet->state = 不存在;
			(ptank->blood)--;
			if (ptank->blood == 0)//如果减血后为0
				ptank->isAlive = false;//声明为死亡
		}
		break;
	case RIGHT:
		if (
			(pbullet->core.X == ptank->core.X) && (pbullet->core.Y - ptank->core.Y == 0) ||
			(pbullet->core.X == ptank->body[0].X) && (pbullet->core.Y - ptank->body[0].Y == 0) ||
			(pbullet->core.X == ptank->body[1].X) && (pbullet->core.Y - ptank->body[1].Y == 0) ||
			(pbullet->core.X == ptank->body[2].X) && (pbullet->core.Y - ptank->body[2].Y == 0) ||
			(pbullet->core.X == ptank->body[3].X) && (pbullet->core.Y - ptank->body[3].Y == 0) ||
			(pbullet->core.X == ptank->body[4].X) && (pbullet->core.Y - ptank->body[4].Y == 0)
			)
		{
			PlaySound(TEXT("conf/duang.wav"), NULL, SND_FILENAME | SND_ASYNC);//播放音效
			pbullet->state = 不存在;
			(ptank->blood)--;
			if (ptank->blood == 0)//如果减血后为0
				ptank->isAlive = false;//声明为死亡
		}
		break;
	default:
		break;
	}
	//是否遇其他敌方坦克
	for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
	{
		if (penemytank[i].isAlive == false) continue;
		switch (pbullet->dir)
		{
		case UP:
			if (
				(pbullet->core.X == penemytank[i].core.X) && (pbullet->core.Y - penemytank[i].core.Y == 0) ||
				(pbullet->core.X == penemytank[i].body[0].X) && (pbullet->core.Y - penemytank[i].body[0].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[1].X) && (pbullet->core.Y - penemytank[i].body[1].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[2].X) && (pbullet->core.Y - penemytank[i].body[2].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[3].X) && (pbullet->core.Y - penemytank[i].body[3].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[4].X) && (pbullet->core.Y - penemytank[i].body[4].Y == 0)
				)
			{
				pbullet->state = 不存在;
				//penemytank[i].isAlive = false;//声明为死亡
			}
			break;
		case DOWN:
			if (
				(pbullet->core.X == penemytank[i].core.X) && (pbullet->core.Y - penemytank[i].core.Y == 0) ||
				(pbullet->core.X == penemytank[i].body[0].X) && (pbullet->core.Y - penemytank[i].body[0].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[1].X) && (pbullet->core.Y - penemytank[i].body[1].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[2].X) && (pbullet->core.Y - penemytank[i].body[2].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[3].X) && (pbullet->core.Y - penemytank[i].body[3].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[4].X) && (pbullet->core.Y - penemytank[i].body[4].Y == 0)
				)
			{
				pbullet->state = 不存在;
				//penemytank[i].isAlive = false;
			}
			break;
		case LEFT:
			if (
				(pbullet->core.X == penemytank[i].core.X) && (pbullet->core.Y - penemytank[i].core.Y == 0) ||
				(pbullet->core.X == penemytank[i].body[0].X) && (pbullet->core.Y - penemytank[i].body[0].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[1].X) && (pbullet->core.Y - penemytank[i].body[1].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[2].X) && (pbullet->core.Y - penemytank[i].body[2].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[3].X) && (pbullet->core.Y - penemytank[i].body[3].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[4].X) && (pbullet->core.Y - penemytank[i].body[4].Y == 0)
				)
			{
				pbullet->state = 不存在;
				//penemytank[i].isAlive = false;
			}
			break;
		case RIGHT:
			if (
				(pbullet->core.X == penemytank[i].core.X) && (pbullet->core.Y - penemytank[i].core.Y == 0) ||
				(pbullet->core.X == penemytank[i].body[0].X) && (pbullet->core.Y - penemytank[i].body[0].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[1].X) && (pbullet->core.Y - penemytank[i].body[1].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[2].X) && (pbullet->core.Y - penemytank[i].body[2].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[3].X) && (pbullet->core.Y - penemytank[i].body[3].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[4].X) && (pbullet->core.Y - penemytank[i].body[4].Y == 0)
				)
			{
				pbullet->state = 不存在;
				//penemytank[i].isAlive = false;
			}
			break;
		default:
			break;
		}
	}
}

//障碍物相关
void BarrierInit()
{
	for (int x = 0; x < MAP_X_WALL; x++)
	{
		for (int y = 0; y < MAP_Y; y++)
		{
			if (
				(x > MAP_X_WALL / 4 - 9 && x < MAP_X_WALL / 4 - 5 && y > MAP_Y / 2 - 8 && y < MAP_Y / 2 - 4) ||
				(x < MAP_X_WALL / 4 + 9 && x > MAP_X_WALL / 4 + 5 && y > MAP_Y / 2 - 8 && y < MAP_Y / 2 - 4) ||
				(x > MAP_X_WALL / 4 - 9 && x < MAP_X_WALL / 4 - 5 && y < MAP_Y / 2 + 8 && y > MAP_Y / 2 + 4) ||
				(x < MAP_X_WALL / 4 + 9 && x > MAP_X_WALL / 4 + 5 && y < MAP_Y / 2 + 8 && y > MAP_Y / 2 + 4)
				)
			{
				g_MAP[x][y] = 土块障碍物;
			}
			if (
				(x > MAP_X_WALL / 4 - 9 && x < MAP_X_WALL / 4 - 5 && y > MAP_Y / 2 - 9 && y < MAP_Y / 2 - 7) ||
				(x < MAP_X_WALL / 4 + 9 && x > MAP_X_WALL / 4 + 5 && y > MAP_Y / 2 - 9 && y < MAP_Y / 2 - 7) ||
				(x > MAP_X_WALL / 4 - 9 && x < MAP_X_WALL / 4 - 5 && y < MAP_Y / 2 + 9 && y > MAP_Y / 2 + 7) ||
				(x < MAP_X_WALL / 4 + 9 && x > MAP_X_WALL / 4 + 5 && y < MAP_Y / 2 + 9 && y > MAP_Y / 2 + 7)
				)
			{
				g_MAP[x][y] = 石块障碍物;
			}


		}
	}
}
void SetBarrier()
{
	DrawMapBorder();		//地图边界

	//提示信息
	setColor(12, 0);
	GotoxyAndPrint(MAP_X / 2 - 12, 4, "     编辑地图");
	GotoxyAndPrint(MAP_X / 2 - 12, 6, "左键单击：创建障碍");
	GotoxyAndPrint(MAP_X / 2 - 12, 8, "右键单击：消除障碍");
	GotoxyAndPrint(MAP_X / 2 - 12, 10, "界外双击：退出编辑");
	setColor(7, 0);

	//鼠标事件相关
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD ir = {};
	DWORD dwCount = 0;
	SetConsoleMode(hInput, ENABLE_MOUSE_INPUT);

	//捕获鼠标事件并反馈给屏幕
	while (true)
	{
		ReadConsoleInput(hInput, &ir, 1, &dwCount);
		if (ir.EventType == MOUSE_EVENT)
		{
			//左键绘制
			if (ir.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				COORD pos = ir.Event.MouseEvent.dwMousePosition;//获取按键的位置
				if (pos.X > 0 && pos.X < MAP_X_WALL && pos.Y >0 && pos.Y < MAP_Y)
				{
					g_MAP[pos.X / 2][pos.Y] = 土块障碍物;
					GotoxyAndPrint(pos.X / 2, pos.Y, "■");
				}
			}
			//右键消除
			if (ir.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			{
				COORD pos = ir.Event.MouseEvent.dwMousePosition;
				if (pos.X > 0 && pos.X < MAP_X_WALL && pos.Y >0 && pos.Y < MAP_Y)
				{
					g_MAP[pos.X / 2][pos.Y] = 空地;
					GotoxyAndPrint(pos.X / 2, pos.Y, "  ");
				}
			}
			//双击退出
			if (ir.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK)
			{
				COORD pos = ir.Event.MouseEvent.dwMousePosition;
				if (!(pos.X > 0 && pos.X < MAP_X_WALL && pos.Y >0 && pos.Y < MAP_Y))
				{
					//地图外双击才退出
					break;
				}
			}
		}
	}
}
void DrawBarr()
{
	for (int x = 0; x < MAP_X_WALL; x++)
	{
		for (int y = 0; y < MAP_Y; y++)
		{
			if(g_MAP[x][y] == 石块障碍物) GotoxyAndPrint(x, y, "■");
			if (g_MAP[x][y] == 土块障碍物) GotoxyAndPrint(x, y, "※");
		}
	}

}

