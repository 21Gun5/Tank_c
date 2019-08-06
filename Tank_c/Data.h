#pragma once
#include <Windows.h>

//宏定义
#define MAP_X 90		//右边界
#define MAP_Y 30		//下边界
#define MAP_X_WALL 60	//右边界

#define ENEMY_TANK_AMOUNT 3

#define 开始游戏 1
#define 退出游戏 2

#define 我方坦克 0
#define 敌方坦克 1

#define 空地 0
#define 地图边界 1
#define 障碍物 2

#define 不存在 0
#define 未赋值 1
#define 已赋值 2




//全局变量
extern int g_MAP[MAP_X][MAP_Y];	//地图数组
extern bool g_isRunning;		//游戏是否运行

//extern int g_isBulExist;		//子弹是否还存在（没打在边界或障碍物上
//extern int g_enemyTankAmount;	//敌坦数量
//extern int g_Bar[MAP_X_WALL][MAP_Y];//障碍物数组
//extern int g_EneTank[MAP_X_WALL][MAP_Y];//敌方坦克数组

//方向枚举
enum direction { UP, DOWN, LEFT, RIGHT };

//子弹结构体
typedef struct _BULLET
{
	COORD core;			//坐标
	enum  direction dir;//方向
	int state;			//子弹状态
}BULLET,*PBULLET;

//坦克结构体
typedef struct _TANK
{
	COORD core;			//中心点
	COORD body[5];		//其他节点
	enum direction dir;	//方向
	unsigned blood;		//血量
	bool isAlive;		//是否存活
	BULLET bullet;		//子弹
}TANK, * PTANK;