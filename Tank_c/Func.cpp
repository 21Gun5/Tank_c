#include "Func.h"
#include "Data.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

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
				y == 0 || y == MAP_Y - 1 ||						//两横边
				(x > MAP_X_WALL && y == MAP_Y / 2))				//帮助信息与游戏信息分割线
			{
				g_MAP[x][y] = 1;
			}
		}
	}

	//隐藏光标
	SetCursorState(false);

	//设置标题
	system("title Tank");

	//初始化随机数种子
	srand((unsigned int)time(0));
}
void DrawMapBorder()
{
	system("cls");						//换页则清屏
	for (int x = 0; x < MAP_X; x++)
	{
		for (int y = 0; y < MAP_Y; y++)
		{
			if (g_MAP[x][y] == 1)
			{
				GotoxyAndPrint(x, y, "■");
				//printf("※");		//占2字符
				//printf("■");
			}
		}
	}
}
void DrawWelcome()
{
	//打印图案
	GotoxyAndPrint(MAP_X / 2 - 25, MAP_Y / 2-10, "Welcome!");
	//printf("Welcome!");

	//提示信息
	GotoxyAndPrint(MAP_X / 2 - 25, MAP_Y / 2 - 6, "1. 新游戏\n");
	//printf("1. 新游戏\n");
	GotoxyAndPrint(MAP_X / 2 - 25, MAP_Y / 2 - 4, "2. 退出游戏\n");
	//printf("2. 退出游戏\n");
	GotoxyAndPrint(MAP_X / 2 - 25, MAP_Y / 2 - 2, "请输入选择-> ");
	//printf( "请输入选择-> ");
	SetCursorState(true);			//用户输入时，光标可见
}
void SelectAction()
{
	
	//int input = _getch() - '0';	//无回显接收；-'0'保证在0-9，而非ASCII
	int input = 1;
	SetCursorState(false);		//输入完隐藏光标

	switch (input)
	{
	case 开始游戏:
	{
		g_isRunning = true;
		break;
	}
	case 退出游戏:
	{
		GotoxyAndPrint(MAP_X / 2 - 25, MAP_Y / 2 + 3, "Bye! \n");
		//printf("Bye! \n");
		break;
	}
	default:
		GotoxyAndPrint(MAP_X / 2 - 25, MAP_Y / 2 + 3, "输入错误\n");
		//printf("输入错误\n");
		break;
	}
}




//坦克相关
void ManipulateTank(PTANK ptank,int who, PTANK penemytank)
//void ManipulateTank(PTANK ptank, int who)
{
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
				g_isBulExist++;
				break;
			default:
				break;
			}
		}
	}
	/*else if (who == 敌方坦克)
	{
		switch (rand() % 4)
		{
		case UP:
			if (!IsTankMeetOther(ptank, UP))
				ptank->core.Y--;
			ptank->dir = UP;
			break;
		case DOWN:
			if (!IsTankMeetOther(ptank, DOWN))
				ptank->core.Y++;
			ptank->dir = DOWN;
			break;
		case LEFT:
			if (!IsTankMeetOther(ptank, LEFT))
				ptank->core.X--;
			ptank->dir = LEFT;
			break;
		case RIGHT:
			if (!IsTankMeetOther(ptank, RIGHT))
				ptank->core.X++;
			ptank->dir = RIGHT;
			break;
		default:
			break;
		}
	}*/

	SetTankShape(ptank);//每次移动后都要重新设置形态
}
void ManipulateTank2(PTANK ptank, int who)
{
	//if (who == 我方坦克)
	//{
	//	char ch = 0;
	//	if (_kbhit())				//非阻塞函数 
	//	{
	//		ch = _getch();			//无回显接受输入
	//		switch (ch)
	//		{
	//		case 'w':
	//			if (!IsTankMeetOther(ptank, UP, ))
	//				ptank->core.Y--;
	//			ptank->dir = UP;
	//			break;
	//		case 's':
	//			if (!IsTankMeetOther(ptank, DOWN))
	//				ptank->core.Y++;
	//			ptank->dir = DOWN;
	//			break;
	//		case 'a':
	//			if (!IsTankMeetOther(ptank, LEFT))
	//				ptank->core.X--;
	//			ptank->dir = LEFT;
	//			break;
	//		case 'd':
	//			if (!IsTankMeetOther(ptank, RIGHT))
	//				ptank->core.X++;
	//			ptank->dir = RIGHT;
	//			break;
	//		case ' ':
	//			g_isBulExist++;
	//			break;
	//		default:
	//			break;
	//		}
	//	}
	//}
	if (who == 敌方坦克)
	{
		switch (rand() % 4)
		{
		case UP:
			if (!IsTankMeetOther2(ptank, UP))
				ptank->core.Y--;
			ptank->dir = UP;
			break;
		case DOWN:
			if (!IsTankMeetOther2(ptank, DOWN))
				ptank->core.Y++;
			ptank->dir = DOWN;
			break;
		case LEFT:
			if (!IsTankMeetOther2(ptank, LEFT))
				ptank->core.X--;
			ptank->dir = LEFT;
			break;
		case RIGHT:
			if (!IsTankMeetOther2(ptank, RIGHT))
				ptank->core.X++;
			ptank->dir = RIGHT;
			break;
		default:
			break;
		}
	}
	SetTankShape(ptank);//每次移动后都要重新设置形态
}


void CleanTankTail(COORD oldCore,PCOORD oldBody)
{
	GotoxyAndPrint(oldCore.X, oldCore.Y, "  ");//中心点
	//printf("  ");		//清尾用2空格
	for (int i = 0; i < 5; i++)//其他点
	{
		GotoxyAndPrint(oldBody[i].X, oldBody[i].Y, "  ");
		//printf("  ");
	}
}
void DrawTank(PTANK ptank,int who)
{
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
		setColor(10, 0);
		GotoxyAndPrint(ptank->core.X, ptank->core.Y, "□");//中心点
		for (int i = 0; i < 5; i++)//其他点
		{
			GotoxyAndPrint(ptank->body[i].X, ptank->body[i].Y, "□");
		}
		setColor(7, 0);
	}
}
void SetTankShape(PTANK ptank)
{
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

bool IsTankMeetOther(PTANK ptank,int dir, PTANK penemytank)
//bool IsTankMeetOther(PTANK ptank, int dir)
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
		if (g_Bar[ptank->core.X][ptank->core.Y - 2] == 1||
			g_Bar[ptank->core.X-1][ptank->core.Y - 2] == 1||
			g_Bar[ptank->core.X+1][ptank->core.Y - 2] == 1)
		{
			return true;
		}
		//是否撞敌方坦克
		for (int i = 0; i < 3; i++)
		{
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
		if (g_Bar[ptank->core.X][ptank->core.Y + 2] == 1 ||
			g_Bar[ptank->core.X - 1][ptank->core.Y + 2] == 1 ||
			g_Bar[ptank->core.X + 1][ptank->core.Y + 2] == 1)
		{
			return true;
		}
		//是否撞敌方坦克
		for (int i = 0; i < 3; i++)
		{
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
		if (g_Bar[ptank->core.X-2][ptank->core.Y] == 1 ||
			g_Bar[ptank->core.X - 2][ptank->core.Y -1] == 1 ||
			g_Bar[ptank->core.X -2][ptank->core.Y + 1] == 1)
		{
			return true;
		}
		//是否撞敌方坦克
		for (int i = 0; i < 3; i++)
		{
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
		if (g_Bar[ptank->core.X + 2][ptank->core.Y] == 1 ||
			g_Bar[ptank->core.X + 2][ptank->core.Y - 1] == 1 ||
			g_Bar[ptank->core.X + 2][ptank->core.Y + 1] == 1)
		{
			return true;
		}
		for (int i = 0; i < 3; i++)
		{
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
bool IsTankMeetOther2(PTANK ptank, int dir)
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
		if (g_Bar[ptank->core.X][ptank->core.Y - 2] == 1 ||
			g_Bar[ptank->core.X - 1][ptank->core.Y - 2] == 1 ||
			g_Bar[ptank->core.X + 1][ptank->core.Y - 2] == 1)
		{
			return true;
		}
		//是否撞敌方坦克
		//if
		break;
	case DOWN:
		//是否撞边界
		if (ptank->core.Y >= MAP_Y - 3)
		{
			return true;
		}
		//是否撞障碍物
		if (g_Bar[ptank->core.X][ptank->core.Y + 2] == 1 ||
			g_Bar[ptank->core.X - 1][ptank->core.Y + 2] == 1 ||
			g_Bar[ptank->core.X + 1][ptank->core.Y + 2] == 1)
		{
			return true;
		}
		break;
	case LEFT:
		//是否撞边界
		if (ptank->core.X <= 2)
		{
			return true;
		}
		//是否撞障碍物
		if (g_Bar[ptank->core.X - 2][ptank->core.Y] == 1 ||
			g_Bar[ptank->core.X - 2][ptank->core.Y - 1] == 1 ||
			g_Bar[ptank->core.X - 2][ptank->core.Y + 1] == 1)
		{
			return true;
		}
		break;
	case RIGHT:
		//是否撞边界
		if (ptank->core.X >= MAP_X_WALL / 2 - 2)
		{
			return true;
		}
		//是否撞障碍物
		if (g_Bar[ptank->core.X + 2][ptank->core.Y] == 1 ||
			g_Bar[ptank->core.X + 2][ptank->core.Y - 1] == 1 ||
			g_Bar[ptank->core.X + 2][ptank->core.Y + 1] == 1)
		{
			return true;
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
void DrawBullet(PBULLET pbullet)
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
		setColor(12, 0);
	}
	//GotoxyAndPrint(pbullet->core.X, pbullet->core.Y);
	//碰到障碍，将子弹画为空格，实现子弹消失
	if (g_Bar[pbullet->core.X][pbullet->core.Y] == 1)
	{
		GotoxyAndPrint(pbullet->core.X, pbullet->core.Y, "  ");
		//printf("  ");
	}
	else
	{
		GotoxyAndPrint(pbullet->core.X, pbullet->core.Y, "■");
		//printf("■");
	}	
	setColor(7, 0);
}
void IsBulMeetOther(PBULLET pbullet)
{
	//是否遇到边界
	if (pbullet->core.X <= 0 ||
		pbullet->core.X >= MAP_X_WALL / 2 ||
		pbullet->core.Y <= 0 ||
		pbullet->core.Y >= MAP_Y - 1)
	{
		g_isBulExist = 0;
	}
	//是否遇到障碍物
	if (g_Bar[pbullet->core.X][pbullet->core.Y] == 1)
	{
		g_isBulExist = 0;
		g_Bar[pbullet->core.X][pbullet->core.Y] = 0;
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
				(x > MAP_X_WALL / 4 - 9 &&x < MAP_X_WALL / 4 - 5 &&y > MAP_Y / 2 - 9 &&y < MAP_Y/2 - 4) ||
				(x < MAP_X_WALL / 4 + 9 &&x > MAP_X_WALL / 4 + 5 && y > MAP_Y / 2 - 9 && y < MAP_Y / 2 - 4)||
				(x > MAP_X_WALL / 4 - 9 && x < MAP_X_WALL / 4 - 5 && y < MAP_Y / 2 + 9 && y > MAP_Y / 2 + 4)||
				(x < MAP_X_WALL / 4 + 9 && x > MAP_X_WALL / 4 + 5 && y < MAP_Y / 2 + 9 && y > MAP_Y / 2 + 4)
				)
			{
				g_Bar[x][y] = 1;
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
			if(g_Bar[x][y] == 1)
			{
				GotoxyAndPrint(x, y, "■");
				//printf("■");
			}
		}
	}

}

