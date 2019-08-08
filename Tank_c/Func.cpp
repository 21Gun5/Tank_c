#include "Func.h"
#include "Data.h"
#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#pragma comment(lib,"winmm.lib")

//系统功能
void SetCursorState(bool b)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);	//获取控制台光标信息
	CursorInfo.bVisible = b;					//显示/隐藏控制台光标
	SetConsoleCursorInfo(handle, &CursorInfo);	//设置控制台光标状态
}
void setColor(int ForeColor, int BackGroundColor)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);					 //获取当前窗口句柄
	SetConsoleTextAttribute(handle, ForeColor + BackGroundColor * 0x10);//设置颜色
}
void GotoxyAndPrint(unsigned x, unsigned y, const char* str)
{
	COORD cur;
	cur.X = x * 2;
	cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
	printf(str);
}

//游戏相关
void GameInit()
{
	//设置地图
	for (int x = 0; x < MAP_X / 2; x++)
	{
		for (int y = 0; y < MAP_Y; y++)
		{
			//地图边界
			if (x == 0 || x == MAP_X / 2 - 1 || x == MAP_X_WALL / 2 ||	//三竖边
				y == 0 || y == MAP_Y - 1 ||							//两横边
				(x > MAP_X_WALL / 2 && y == MAP_Y / 2))				//帮助信息与游戏信息分割线
			{
				g_MAP[x][y] = 地图边界;
			}
			if (x >= MAP_X_WALL / 4 - 2 && x <= MAP_X_WALL / 4 + 3 && y >= MAP_Y - 2 - 3 && y <= MAP_Y - 2)
			{
				if (x >= MAP_X_WALL / 4 && x <= MAP_X_WALL / 4 + 1 && y >= MAP_Y - 2 - 1 && y <= MAP_Y - 2)
					g_MAP[x][y] = 我家泉水;
				else
					g_MAP[x][y] = 土块障碍;
			}
		}
	}

	SetCursorState(false);				//隐藏光标
	system("title Tank");				//设置标题
	srand((unsigned int)time(0));		//初始化随机数种子

	//播放背景音乐(先不放，烦人
	mciSendString("open conf/BGM.mp3 alias bgm", NULL, 0, NULL);//打开文件
	mciSendString("play bgm repeat", NULL, 0, NULL);			  // 循环播放
}
char* ShowGameFile()
{
	//遍历指定目录及后缀的文件名并存入数组
	char* gameFiles[10] = { nullptr };
	long Handle;
	struct _finddata_t FileInfo;
	int count = 0;
	if ((Handle = _findfirst("conf/game/*.i", &FileInfo)) == -1L)
		printf("Not Found\n");
	else
	{
		gameFiles[count] = (char*)malloc(20);
		strcpy_s(gameFiles[count], 20, FileInfo.name);
		//g_Maps[count] = FileInfo.name;
		count++;
		while (_findnext(Handle, &FileInfo) == 0)
		{
			gameFiles[count] = (char*)malloc(20);
			strcpy_s(gameFiles[count], 20, FileInfo.name);
			//g_Maps[count] = FileInfo.name;
			count++;
		}
		_findclose(Handle);
	}
	//显示存档文件
	system("cls");
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 8, "请选择存档");
	int i = 0;								//循环变量在for外定义
	for (; i < count; i++)
	{
		GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 6 + i, "");
		printf("%d.%s", i + 1, gameFiles[i]);
	}
	//选择
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 6 + i, "请输入选择-> ");
	SetCursorState(true);
	int input = _getch() - '0';				//保证0-9而非ASCII
	SetCursorState(false);
	char* _file = (char*)malloc(15);
	strcpy_s(_file, 15, gameFiles[input - 1]);//数字始于1，而下标始于0	
	return _file;
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
	GotoxyAndPrint(MAP_X_WALL / 4 - 2, MAP_Y / 2 - 3, "");
	printf("Scores: %d", ENEMY_TANK_AMOUNT - eneTankCount);
	setColor(7, 0);
}
int  SelectMenu(int size, int* pindex)
{
	int input = _getch();			//无回显接收
	switch (input)
	{
	case _UP:
		if (*pindex > 0)
			* pindex -= 1;
		break;
	case _DOWN:
		if (*pindex < size - 1)//最后一条
			* pindex += 1;
		break;
	case ENTER:
		return ENTER;
		break;
	default:
		break;
	}
	return 0;
}
void SaveGame(PTANK ptank, PTANK penemytank)
{
	//提示信息
	system("cls");
	setColor(12, 0);
	GotoxyAndPrint(MAP_X / 2 - 12, 12, "请输入存档名字");
	GotoxyAndPrint(MAP_X / 2 - 12, 14, "");
	//输入文件名
	char str[15];
	char* filename = (char*)malloc(40);
	SetCursorState(true);
	scanf_s("%s", str, 15);
	SetCursorState(false);
	setColor(7, 0);
	sprintf_s(filename, 40, "%s%s%s", "conf/game/", str, ".i");
	//打开文件
	FILE* pFile = NULL;
	errno_t err = fopen_s(&pFile, filename, "wb");
	//写入全局变量
	fwrite(&g_isRunning, sizeof(int), 1, pFile);		//游戏运行状态
	fwrite(&g_levelEneTank, sizeof(int), 1, pFile);	//写入难度，控制敌方坦克速度
	fwrite(&g_levelEneBul, sizeof(int), 1, pFile);	//写入难度，控制敌方子弹速度
	//写入障碍
	for (int x = 0; x < MAP_X_WALL; x++)
	{
		for (int y = 0; y < MAP_Y; y++)
		{
			fwrite(&g_MAP[x][y], sizeof(int), 1, pFile);
		}
	}
	//写入我方坦克
	fwrite(&ptank->core, sizeof(COORD), 1, pFile);//中心节点
	for (int i = 0; i < 5; i++)
		fwrite(&ptank->body[i], sizeof(COORD), 1, pFile);//其他节点
	fwrite(&ptank->dir, sizeof(int), 1, pFile);//方向
	fwrite(&ptank->blood, sizeof(int), 1, pFile);//血量
	fwrite(&ptank->isAlive, sizeof(bool), 1, pFile);//是否存活
	fwrite(&ptank->who, sizeof(int), 1, pFile);//身份
	//写入我方子弹
	fwrite(&ptank->bullet.core, sizeof(COORD), 1, pFile);//坐标
	fwrite(&ptank->bullet.dir, sizeof(int), 1, pFile);//方向
	fwrite(&ptank->bullet.state, sizeof(int), 1, pFile);//状态
	//写入敌方
	for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
	{
		//写入敌方坦克
		fwrite(&penemytank[i].core, sizeof(COORD), 1, pFile);//中心节点
		for (int i = 0; i < 5; i++)
			fwrite(&penemytank[i].body[i], sizeof(COORD), 1, pFile);//其他节点
		fwrite(&penemytank[i].dir, sizeof(int), 1, pFile);//方向
		fwrite(&penemytank[i].blood, sizeof(int), 1, pFile);//血量
		fwrite(&penemytank[i].isAlive, sizeof(bool), 1, pFile);//是否存活
		fwrite(&penemytank[i].who, sizeof(int), 1, pFile);//身份
		//写入敌方子弹
		fwrite(&penemytank[i].bullet.core, sizeof(COORD), 1, pFile);//坐标
		fwrite(&penemytank[i].bullet.dir, sizeof(int), 1, pFile);//方向
		fwrite(&penemytank[i].bullet.state, sizeof(int), 1, pFile);//状态
	}
	fclose(pFile);
}
void LoadGame(PTANK ptank, PTANK penemytank, char* str)
{
	char* filename = (char*)malloc(40);
	sprintf_s(filename, 40, "%s%s", "conf/game/", str);
	FILE* pFile = NULL;
	errno_t err = fopen_s(&pFile, filename, "rb");
	//读取全局变量
	fread(&g_isRunning, sizeof(int), 1, pFile);		//游戏运行状态
	fread(&g_levelEneTank, sizeof(int), 1, pFile);	//读取难度，控制敌方坦克速度
	fread(&g_levelEneBul, sizeof(int), 1, pFile);	//读取难度，控制敌方子弹速度
	//读取障碍
	for (int x = 0; x < MAP_X_WALL; x++)
	{
		for (int y = 0; y < MAP_Y; y++)
		{
			fread(&g_MAP[x][y], sizeof(int), 1, pFile);
		}
	}
	//读取我方坦克
	fread(&ptank->core, sizeof(COORD), 1, pFile);//中心节点
	for (int i = 0; i < 5; i++)
		fread(&ptank->body[i], sizeof(COORD), 1, pFile);//其他节点
	fread(&ptank->dir, sizeof(int), 1, pFile);//方向
	fread(&ptank->blood, sizeof(int), 1, pFile);//血量
	fread(&ptank->isAlive, sizeof(bool), 1, pFile);//是否存活
	fread(&ptank->who, sizeof(int), 1, pFile);//身份
	//读取我方子弹
	fread(&ptank->bullet.core, sizeof(COORD), 1, pFile);//坐标
	fread(&ptank->bullet.dir, sizeof(int), 1, pFile);//方向
	fread(&ptank->bullet.state, sizeof(int), 1, pFile);//状态
	//读取敌方
	for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
	{
		//读取敌方坦克
		fread(&penemytank[i].core, sizeof(COORD), 1, pFile);//中心节点
		for (int i = 0; i < 5; i++)
			fread(&penemytank[i].body[i], sizeof(COORD), 1, pFile);//其他节点
		fread(&penemytank[i].dir, sizeof(int), 1, pFile);//方向
		fread(&penemytank[i].blood, sizeof(int), 1, pFile);//血量
		fread(&penemytank[i].isAlive, sizeof(bool), 1, pFile);//是否存活
		fread(&penemytank[i].who, sizeof(int), 1, pFile);//身份
		//读取敌方子弹
		fread(&penemytank[i].bullet.core, sizeof(COORD), 1, pFile);//坐标
		fread(&penemytank[i].bullet.dir, sizeof(int), 1, pFile);//方向
		fread(&penemytank[i].bullet.state, sizeof(int), 1, pFile);//状态
	}
	fclose(pFile);
}

//打印相关
void DrawLogo()
{
	GotoxyAndPrint(MAP_X / 4 - 9, MAP_Y / 2 - 15, "MMP\"\"MM\"\"YMM                      `7MM      ");
	GotoxyAndPrint(MAP_X / 4 - 9, MAP_Y / 2 - 14, "P'   MM   `7                        MM      ");
	GotoxyAndPrint(MAP_X / 4 - 9, MAP_Y / 2 - 13, "     MM       ,6\"Yb.  `7MMpMMMb.    MM, MP'");
	GotoxyAndPrint(MAP_X / 4 - 9, MAP_Y / 2 - 12, "     MM      8)   MM    MM    MM    MM ;Y   ");
	GotoxyAndPrint(MAP_X / 4 - 9, MAP_Y / 2 - 11, "     MM       ,pm9MM    MM    MM    MM;Mm   ");
	GotoxyAndPrint(MAP_X / 4 - 9, MAP_Y / 2 - 10, "     MM      8M   MM    MM    MM    MM `Mb. ");
	GotoxyAndPrint(MAP_X / 4 - 9, MAP_Y / 2 - 9, "   .JMML.    `Moo9^Yo..JMML  JMML..JMML. YA.");
}
void DrawBarr()
{
	for (int x = 0; x < MAP_X_WALL; x++)
	{
		for (int y = 0; y < MAP_Y; y++)
		{
			if (g_MAP[x][y] == 石块障碍) GotoxyAndPrint(x, y, "■");
			if (g_MAP[x][y] == 土块障碍) GotoxyAndPrint(x, y, "※");
		}
	}
}
void DrawBorder()
{
	system("cls");						//换页则清屏
	for (int x = 0; x < MAP_X; x++)
	{
		for (int y = 0; y < MAP_Y; y++)
		{
			if (g_MAP[x][y] == 地图边界)
			{
				GotoxyAndPrint(x, y, "■");
			}
			if (g_MAP[x][y] == 我家泉水)
			{
				setColor(12, 0);
				GotoxyAndPrint(x, y, "★");
				setColor(7, 0);
			}
		}
	}
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
void DrawBullet(PBULLET pbullet, PTANK ptank)
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
		if (ptank->who == 我方坦克)
			setColor(10, 0);
		else if (ptank->who == 敌方坦克)
			setColor(11, 0);
	}
	//碰到障碍，将子弹画为空格，实现子弹消失
	if (g_MAP[pbullet->core.X][pbullet->core.Y] == 土块障碍)
	{
		GotoxyAndPrint(pbullet->core.X, pbullet->core.Y, "  ");
	}
	//碰到石块障碍物，，实现子弹消失的效果
	else if (g_MAP[pbullet->core.X][pbullet->core.Y] == 石块障碍)
	{
		setColor(7, 0);
	}
	//碰到泉水，将子弹换成其颜色和形状，实现子弹消失
	else if (g_MAP[pbullet->core.X][pbullet->core.Y] == 我家泉水)
	{
		setColor(12, 0);
		GotoxyAndPrint(pbullet->core.X, pbullet->core.Y, "★");
	}
	//一般运动状态
	else
	{
		GotoxyAndPrint(pbullet->core.X, pbullet->core.Y, "■");
	}
	setColor(7, 0);
}
void DrawGameInfo(PTANK ptank, PTANK penemytank)
{
	//存活敌坦数量
	int eneTankCount = GetLiveEnemyAmount(penemytank);
	//当前难度
	char level[10];
	if (g_levelEneTank == 300) strcpy_s(level, 10, "简单\0");
	else if (g_levelEneTank == 200) strcpy_s(level, 10, "一般\0");
	else if (g_levelEneTank == 100)strcpy_s(level, 10, "困难\0");
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
	GotoxyAndPrint(MAP_X / 2 - 11, 11, "");
	printf("当前难度: %s", level);
	setColor(7, 0);
}
void DrawMenu(const char** menu, int size, int index)
{
	system("cls");
	DrawLogo();
	//提示信息
	for (int i = 0; i < size; i++)//菜单条数
	{
		if (i == index)
		{
			setColor(12, 0);
			GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 6 + 2 * i, menu[i]);
			setColor(7, 0);
		}
		else
			GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 6 + 2 * i, menu[i]);
	}
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
		if (penemytank[i].isAlive == true)
			count++;
	}
	return count;
}
void ManipulateMyTank(PTANK ptank, int who, PTANK penemytank)
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
				if (!IsMyTankMeetOther(ptank, UP, penemytank))
					ptank->core.Y--;
				ptank->dir = UP;
				break;
			case 's':
				if (!IsMyTankMeetOther(ptank, DOWN, penemytank))
					ptank->core.Y++;
				ptank->dir = DOWN;
				break;
			case 'a':
				if (!IsMyTankMeetOther(ptank, LEFT, penemytank))
					ptank->core.X--;
				ptank->dir = LEFT;
				break;
			case 'd':
				if (!IsMyTankMeetOther(ptank, RIGHT, penemytank))
					ptank->core.X++;
				ptank->dir = RIGHT;
				break;
			case ' ':
				if (ptank->bullet.state != 已赋值)
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
				} while (!(tmp == '1' || tmp == '2' || tmp == '3'));//只有输入123才可
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
					GotoxyAndPrint(MAP_X / 2 - 14, 1, "想如何退出?");
					GotoxyAndPrint(MAP_X / 2 - 14, 2, "1. 保存退出");
					GotoxyAndPrint(MAP_X / 2 - 14, 3, "2. 直接退出");
					char op = _getch();
					if (op == '1')		//保存退出
					{
						SaveGame(ptank, penemytank);
						GameOver(penemytank);
						g_isRunning = false;
						break;
					}
					else if (op == '2')	//直接退出
					{
						GameOver(penemytank);
						g_isRunning = false;
						break;
					}
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
void ManipulateEneTank(PTANK ptank, int who, PTANK pmytank, PTANK penemytank)
{
	if (ptank->isAlive == false) return;
	if (who == 敌方坦克)
	{
		switch (rand() % 5)
		{
		case UP:
			if (!IsEneTankMeetOther(ptank, UP, pmytank, penemytank))
				ptank->core.Y--;
			ptank->dir = UP;
			break;
		case DOWN:
			if (!IsEneTankMeetOther(ptank, DOWN, pmytank, penemytank))
				ptank->core.Y++;
			ptank->dir = DOWN;
			break;
		case LEFT:
			if (!IsEneTankMeetOther(ptank, LEFT, pmytank, penemytank))
				ptank->core.X--;
			ptank->dir = LEFT;
			break;
		case RIGHT:
			if (!IsEneTankMeetOther(ptank, RIGHT, pmytank, penemytank))
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
bool IsMyTankMeetOther(PTANK ptank, int dir, PTANK penemytank)
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
		if ((g_MAP[ptank->core.X][ptank->core.Y - 2] == 土块障碍 ||
			g_MAP[ptank->core.X - 1][ptank->core.Y - 2] == 土块障碍 ||
			g_MAP[ptank->core.X + 1][ptank->core.Y - 2] == 土块障碍) ||
			(g_MAP[ptank->core.X][ptank->core.Y - 2] == 石块障碍 ||
				g_MAP[ptank->core.X - 1][ptank->core.Y - 2] == 石块障碍 ||
				g_MAP[ptank->core.X + 1][ptank->core.Y - 2] == 石块障碍))
		{
			return true;
		}
		//是否撞敌方坦克
		for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
		{
			if (penemytank[i].isAlive == false) continue;
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
		if ((g_MAP[ptank->core.X][ptank->core.Y + 2] == 土块障碍 ||
			g_MAP[ptank->core.X - 1][ptank->core.Y + 2] == 土块障碍 ||
			g_MAP[ptank->core.X + 1][ptank->core.Y + 2] == 土块障碍) ||
			(g_MAP[ptank->core.X][ptank->core.Y + 2] == 石块障碍 ||
				g_MAP[ptank->core.X - 1][ptank->core.Y + 2] == 石块障碍 ||
				g_MAP[ptank->core.X + 1][ptank->core.Y + 2] == 石块障碍))
		{
			return true;
		}
		//是否遇到我家泉水
		if (g_MAP[ptank->core.X][ptank->core.Y] == 我家泉水)
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
		if ((g_MAP[ptank->core.X - 2][ptank->core.Y] == 土块障碍 ||
			g_MAP[ptank->core.X - 2][ptank->core.Y - 1] == 土块障碍 ||
			g_MAP[ptank->core.X - 2][ptank->core.Y + 1] == 土块障碍) ||
			(g_MAP[ptank->core.X - 2][ptank->core.Y] == 石块障碍 ||
				g_MAP[ptank->core.X - 2][ptank->core.Y - 1] == 石块障碍 ||
				g_MAP[ptank->core.X - 2][ptank->core.Y + 1] == 石块障碍))
		{
			return true;
		}
		//是否遇到我家泉水
		if (g_MAP[ptank->core.X][ptank->core.Y] == 我家泉水)
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
		if ((g_MAP[ptank->core.X + 2][ptank->core.Y] == 土块障碍 ||
			g_MAP[ptank->core.X + 2][ptank->core.Y - 1] == 土块障碍 ||
			g_MAP[ptank->core.X + 2][ptank->core.Y + 1] == 土块障碍) ||
			(g_MAP[ptank->core.X + 2][ptank->core.Y] == 石块障碍 ||
				g_MAP[ptank->core.X + 2][ptank->core.Y - 1] == 石块障碍 ||
				g_MAP[ptank->core.X + 2][ptank->core.Y + 1] == 石块障碍))
		{
			return true;
		}
		//是否遇到我家泉水
		if (g_MAP[ptank->core.X][ptank->core.Y] == 我家泉水)
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
bool IsEneTankMeetOther(PTANK ptank, int dir, PTANK pmytank, PTANK penemytank)
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
		if ((g_MAP[ptank->core.X][ptank->core.Y - 2] == 土块障碍 ||
			g_MAP[ptank->core.X - 1][ptank->core.Y - 2] == 土块障碍 ||
			g_MAP[ptank->core.X + 1][ptank->core.Y - 2] == 土块障碍) ||
			(g_MAP[ptank->core.X][ptank->core.Y - 2] == 石块障碍 ||
				g_MAP[ptank->core.X - 1][ptank->core.Y - 2] == 石块障碍 ||
				g_MAP[ptank->core.X + 1][ptank->core.Y - 2] == 石块障碍))
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
		if ((g_MAP[ptank->core.X][ptank->core.Y + 2] == 土块障碍 ||
			g_MAP[ptank->core.X - 1][ptank->core.Y + 2] == 土块障碍 ||
			g_MAP[ptank->core.X + 1][ptank->core.Y + 2] == 土块障碍) ||
			(g_MAP[ptank->core.X][ptank->core.Y + 2] == 石块障碍 ||
				g_MAP[ptank->core.X - 1][ptank->core.Y + 2] == 石块障碍 ||
				g_MAP[ptank->core.X + 1][ptank->core.Y + 2] == 石块障碍))
		{
			return true;
		}
		//是否遇到我家泉水
		if (g_MAP[ptank->core.X][ptank->core.Y] == 我家泉水)
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
		if ((g_MAP[ptank->core.X - 2][ptank->core.Y] == 土块障碍 ||
			g_MAP[ptank->core.X - 2][ptank->core.Y - 1] == 土块障碍 ||
			g_MAP[ptank->core.X - 2][ptank->core.Y + 1] == 土块障碍) ||
			(g_MAP[ptank->core.X - 2][ptank->core.Y] == 石块障碍 ||
				g_MAP[ptank->core.X - 2][ptank->core.Y - 1] == 石块障碍 ||
				g_MAP[ptank->core.X - 2][ptank->core.Y + 1] == 石块障碍))
		{
			return true;
		}
		//是否遇到我家泉水
		if (g_MAP[ptank->core.X][ptank->core.Y] == 我家泉水)
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
		if ((g_MAP[ptank->core.X + 2][ptank->core.Y] == 土块障碍 ||
			g_MAP[ptank->core.X + 2][ptank->core.Y - 1] == 土块障碍 ||
			g_MAP[ptank->core.X + 2][ptank->core.Y + 1] == 土块障碍) ||
			(g_MAP[ptank->core.X + 2][ptank->core.Y] == 石块障碍 ||
				g_MAP[ptank->core.X + 2][ptank->core.Y - 1] == 石块障碍 ||
				g_MAP[ptank->core.X + 2][ptank->core.Y + 1] == 石块障碍))
		{
			return true;
		}
		//是否遇到我家泉水
		if (g_MAP[ptank->core.X][ptank->core.Y] == 我家泉水)
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
void IsMyBulMeetOther(PBULLET pbullet, PTANK penemytank, PTANK ptank)
{
	//遇边界
	if (pbullet->core.X <= 0 ||
		pbullet->core.X >= MAP_X_WALL / 2 ||
		pbullet->core.Y <= 0 ||
		pbullet->core.Y >= MAP_Y - 1)
	{
		pbullet->state = 不存在;
	}
	//遇土块障碍
	if (g_MAP[pbullet->core.X][pbullet->core.Y] == 土块障碍)
	{
		pbullet->state = 不存在;
		g_MAP[pbullet->core.X][pbullet->core.Y] = 空地;
	}
	//遇石块障碍
	if (g_MAP[pbullet->core.X][pbullet->core.Y] == 石块障碍)
	{
		pbullet->state = 不存在;
	}
	//遇泉水
	if (g_MAP[pbullet->core.X][pbullet->core.Y] == 我家泉水)
	{
		pbullet->state = 不存在;
	}
	//遇敌方坦克
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
				PlaySound(TEXT("conf/duang.wav"), NULL, SND_FILENAME | SND_ASYNC);//播放音效
				pbullet->state = 不存在;
				penemytank[i].blood--;
				if (penemytank[i].blood == 0)//减血后为0则死亡
					penemytank[i].isAlive = false;
				if (!penemytank[i].isAlive && (ENEMY_TANK_AMOUNT - GetLiveEnemyAmount(penemytank)) % 3 == 0)//每打死三个生命值+1
					(ptank->blood)++;//要加!penemytank[i].isAlive，要不打到多条命的敌坦也加命
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
				if (penemytank[i].blood == 0)//减血后为0则死亡
					penemytank[i].isAlive = false;
				if (!penemytank[i].isAlive && (ENEMY_TANK_AMOUNT - GetLiveEnemyAmount(penemytank)) % 3 == 0)//每打死三个生命值+1
					(ptank->blood)++;
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
				if (penemytank[i].blood == 0)//减血后为0则死亡
					penemytank[i].isAlive = false;
				if (!penemytank[i].isAlive && (ENEMY_TANK_AMOUNT - GetLiveEnemyAmount(penemytank)) % 3 == 0)//每打死三个生命值+1
					(ptank->blood)++;
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
				if (penemytank[i].blood == 0)//减血后为0则死亡
					penemytank[i].isAlive = false;
				if (!penemytank[i].isAlive && (ENEMY_TANK_AMOUNT - GetLiveEnemyAmount(penemytank)) % 3 == 0)//每打死三个生命值+1
					(ptank->blood)++;
			}
			break;
		default:
			break;
		}
	}
}
void IsEneBulMeetOther(PBULLET pbullet, PTANK penemytank, PTANK ptank)
{
	//遇边界
	if (pbullet->core.X <= 0 ||
		pbullet->core.X >= MAP_X_WALL / 2 ||
		pbullet->core.Y <= 0 ||
		pbullet->core.Y >= MAP_Y - 1)
	{
		pbullet->state = 不存在;
	}
	//遇土块障碍
	if (g_MAP[pbullet->core.X][pbullet->core.Y] == 土块障碍)
	{
		pbullet->state = 不存在;
		g_MAP[pbullet->core.X][pbullet->core.Y] = 空地;
	}
	//遇石块障碍
	if (g_MAP[pbullet->core.X][pbullet->core.Y] == 石块障碍)
	{
		pbullet->state = 不存在;
	}
	//遇泉水
	if (g_MAP[pbullet->core.X][pbullet->core.Y] == 我家泉水)
	{
		pbullet->state = 不存在;
		ptank->blood = 0;//泉水打到，我方坦克当场去世
	}
	//遇到我方坦克
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
	//遇其他敌方坦克
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
			}
			break;
		default:
			break;
		}
	}
}

//地图相关
char* ShowMapFile()
{
	//遍历指定目录及后缀的文件名并存入数组
	char* mapFiles[10] = { nullptr };
	long Handle;
	struct _finddata_t FileInfo;
	int count = 0;
	if ((Handle = _findfirst("conf/map/*.i", &FileInfo)) == -1L)
		printf("Not Found\n");
	else
	{
		mapFiles[count] = (char*)malloc(20);
		strcpy_s(mapFiles[count], 20, FileInfo.name);
		//g_Maps[count] = FileInfo.name;
		count++;
		while (_findnext(Handle, &FileInfo) == 0)
		{
			mapFiles[count] = (char*)malloc(20);
			strcpy_s(mapFiles[count], 20, FileInfo.name);
			//g_Maps[count] = FileInfo.name;
			count++;
		}
		_findclose(Handle);
	}
	//显示地图文件
	system("cls");
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 8, "请选择地图");
	int i = 0;								//循环变量在for外定义
	for (; i < count; i++)
	{
		GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 6 + i, "");
		printf("%d.%s", i + 1, mapFiles[i]);
	}
	//选择
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 6 + i, "请输入选择-> ");
	SetCursorState(true);
	int input = _getch() - '0';				//保证0-9而非ASCII
	SetCursorState(false);
	char* _file = (char*)malloc(15);
	strcpy_s(_file, 15, mapFiles[input - 1]);//数字始于1，而下标始于0	
	return _file;
}
void SetDefaultMap()
{
	for (int x = 0; x < MAP_X_WALL; x++)
	{
		for (int y = 0; y < MAP_Y; y++)
		{
			if (
				(x > MAP_X_WALL / 4 - 9 && x < MAP_X_WALL / 4 - 5 && y > MAP_Y / 2 - 8 && y < MAP_Y / 2 - 4) ||
				(x < MAP_X_WALL / 4 + 9 && x > MAP_X_WALL / 4 + 5 && y > MAP_Y / 2 - 8 && y < MAP_Y / 2 - 4) ||
				(x > MAP_X_WALL / 4 - 9 && x < MAP_X_WALL / 4 - 5 && y < MAP_Y / 2 + 6 && y > MAP_Y / 2 + 2) ||
				(x < MAP_X_WALL / 4 + 9 && x > MAP_X_WALL / 4 + 5 && y < MAP_Y / 2 + 6 && y > MAP_Y / 2 + 2)
				)
			{
				g_MAP[x][y] = 土块障碍;
			}
			if (
				(x > MAP_X_WALL / 4 - 9 && x < MAP_X_WALL / 4 - 5 && y > MAP_Y / 2 - 9 && y < MAP_Y / 2 - 7) ||
				(x < MAP_X_WALL / 4 + 9 && x > MAP_X_WALL / 4 + 5 && y > MAP_Y / 2 - 9 && y < MAP_Y / 2 - 7) ||
				(x > MAP_X_WALL / 4 - 9 && x < MAP_X_WALL / 4 - 5 && y < MAP_Y / 2 + 7 && y > MAP_Y / 2 + 5) ||
				(x < MAP_X_WALL / 4 + 9 && x > MAP_X_WALL / 4 + 5 && y < MAP_Y / 2 + 7 && y > MAP_Y / 2 + 5)
				)
			{
				g_MAP[x][y] = 石块障碍;
			}
		}
	}
}
void LoadMapFile(char* str)
{
	char* filename = (char*)malloc(40);
	sprintf_s(filename, 40, "%s%s", "conf/map/", str);
	FILE* pFile = NULL;
	errno_t err = fopen_s(&pFile, filename, "rb");
	for (int x = 0; x < MAP_X_WALL; x++)
	{
		for (int y = 0; y < MAP_Y; y++)
		{
			fread(&g_MAP[x][y], sizeof(int), 1, pFile);
		}
	}
	fclose(pFile);
}
void CustomizeMap(PTANK ptank, PTANK penemytank)
{
	DrawBorder();		//地图边界
	//提示信息
	setColor(12, 0);
	GotoxyAndPrint(MAP_X / 2 - 12, 2, "     编辑地图");
	GotoxyAndPrint(MAP_X / 2 - 12, 4, "左键单击：创建土块");
	GotoxyAndPrint(MAP_X / 2 - 12, 6, "滚轮单击：创建石块");
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
			//左键绘制土块障碍
			if (ir.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				COORD pos = ir.Event.MouseEvent.dwMousePosition;//获取按键的位置
				//不可在我方坦克处绘制
				if (pos.X / 2 >= ptank->core.X - 1 &&
					pos.X / 2 <= ptank->core.X + 1 &&
					pos.Y >= ptank->core.Y - 1 &&
					pos.Y <= ptank->core.Y + 1)
					continue;
				//不可在我方坦克绘制
				int flag = 0;
				for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
				{
					if (pos.X / 2 >= penemytank[i].core.X - 1 &&
						pos.X / 2 <= penemytank[i].core.X + 1 &&
						pos.Y >= penemytank[i].core.Y - 1 &&
						pos.Y <= penemytank[i].core.Y + 1)
						flag = 1;
					//continue;//作用域是此for
				}
				if (flag == 1) continue;
				//不可在泉水绘制
				if (pos.X / 2 >= MAP_X_WALL / 4 &&
					pos.X / 2 <= MAP_X_WALL / 4 + 1 &&
					pos.Y >= MAP_Y - 2 - 1 &&
					pos.Y <= MAP_Y - 2) continue;
				//可绘制处（除边界、坦克、泉水）
				if (pos.X > 1 && pos.X < MAP_X_WALL && pos.Y >0 && pos.Y < MAP_Y - 1)
				{
					g_MAP[pos.X / 2][pos.Y] = 土块障碍;
					GotoxyAndPrint(pos.X / 2, pos.Y, "※");
				}
			}
			//滚轮绘制石块障碍
			if (ir.Event.MouseEvent.dwButtonState == FROM_LEFT_2ND_BUTTON_PRESSED)
			{
				COORD pos = ir.Event.MouseEvent.dwMousePosition;//获取按键的位置
				//不可在我方坦克处绘制
				if (pos.X / 2 >= ptank->core.X - 1 &&
					pos.X / 2 <= ptank->core.X + 1 &&
					pos.Y >= ptank->core.Y - 1 &&
					pos.Y <= ptank->core.Y + 1)
					continue;
				//不可在我方坦克绘制
				int flag = 0;
				for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
				{
					if (pos.X / 2 >= penemytank[i].core.X - 1 &&
						pos.X / 2 <= penemytank[i].core.X + 1 &&
						pos.Y >= penemytank[i].core.Y - 1 &&
						pos.Y <= penemytank[i].core.Y + 1)
						flag = 1;
					//continue;//作用域是此for
				}
				if (flag == 1) continue;
				//不可在泉水绘制
				if (pos.X / 2 >= MAP_X_WALL / 4 - 2 &&
					pos.X / 2 <= MAP_X_WALL / 4 + 3 &&
					pos.Y >= MAP_Y - 2 - 3 &&
					pos.Y <= MAP_Y - 2) continue;
				//可绘制处（除边界、坦克、泉水）
				if (pos.X > 1 && pos.X < MAP_X_WALL && pos.Y >0 && pos.Y < MAP_Y - 1)
				{
					g_MAP[pos.X / 2][pos.Y] = 石块障碍;
					GotoxyAndPrint(pos.X / 2, pos.Y, "■");
				}
			}
			//右键消除
			if (ir.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			{
				COORD pos = ir.Event.MouseEvent.dwMousePosition;
				if (pos.X > 1 && pos.X < MAP_X_WALL && pos.Y >0 && pos.Y < MAP_Y - 1)
				{
					g_MAP[pos.X / 2][pos.Y] = 空地;
					GotoxyAndPrint(pos.X / 2, pos.Y, "  ");
				}
			}
			//双击退出
			if (ir.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK)
			{
				COORD pos = ir.Event.MouseEvent.dwMousePosition;
				if (!(pos.X > 1 && pos.X < MAP_X_WALL && pos.Y >0 && pos.Y < MAP_Y - 1))
				{
					//地图外双击才退出
					break;
				}
			}
		}
	}
	//提示信息
	system("cls");
	setColor(12, 0);
	GotoxyAndPrint(MAP_X / 2 - 12, 12, "请输入地图名字:");
	GotoxyAndPrint(MAP_X - 24, 14, "");
	//输入文件名
	char str[15];
	char* filename = (char*)malloc(40);
	SetCursorState(true);
	scanf_s("%s", str, 15);
	SetCursorState(false);
	setColor(7, 0);
	sprintf_s(filename, 40, "%s%s%s", "conf/map/", str, ".i");
	//数据写入文件
	FILE* pFile = NULL;
	errno_t err = fopen_s(&pFile, filename, "wb");
	for (int x = 0; x < MAP_X_WALL; x++)
	{
		for (int y = 0; y < MAP_Y; y++)
		{
			fwrite(&g_MAP[x][y], sizeof(int), 1, pFile);
		}
	}
	fclose(pFile);
}