#include "Data.h"

int g_MAP[MAP_X][MAP_Y] = { 空地 };
bool g_isRunning = false;
int g_levelEneTank = 200;
int g_levelEneBul = 70;
const char* menuPlay[3] = { "开始游戏","读取游戏","退出游戏" };
const char* menuWhoMap[3] = { "系统默认","玩家提供","返回上页" };
const char* menuWhenMap[3] = { "新建地图","已有地图","返回上页" };
const char* menuLevel[3] = { "简单","一般","困难" };