#pragma once
#include <Windows.h>

//宏定义
#define MAP_X 90		//右边界
#define MAP_Y 30		//下边界
#define MAP_X_WALL 60	//右边界
#define 开始游戏 1
#define 退出游戏 2

//全局变量
extern int g_MAP[MAP_X][MAP_Y];	//地图数组
extern bool g_isRunning;		//游戏是否运行
//extern bool g_isFire;			//是否发射子弹
extern int g_isBulExist;		//子弹是否还存在（没打在边界或障碍物上
extern int g_Bar[MAP_X_WALL][MAP_Y];//障碍物数组
//extern PCOORD g_Bar;

//方向枚举
enum direction { UP, DOWN, LEFT, RIGHT };

//坦克结构体
typedef struct _TANK
{
	COORD core;			//中心点
	COORD body[5];		//其他节点
	enum direction dir;	//方向
}TANK, * PTANK;

//子弹结构体
typedef struct _BULLET
{
	COORD core;			//坐标
	enum  direction dir;//方向
}BULLET,*PBULLET;

//障碍物结构体
//typedef struct _BARRIER
//{
//	PCOORD pbody;
//	int count = 20;
//}BARRIER, * PBARRIER;