#pragma once
#include "Data.h"

//系统功能
void SetCursorState(bool b);								//设置光标状态
void setColor(int ForeColor, int BackGroundColor);			//设置颜色
void GotoxyAndPrint(unsigned x, unsigned y, const char* str);//光标到指定位置

//游戏相关
void GameInit();				        //初始化工作
char* ShowGameFile();					//显示存档
void GameOver(PTANK penemytank);		//善后工作
int SelectMenu(int size, int* pindex);  //选择菜单项
void SaveGame(PTANK ptank, PTANK penemytank);				//存档
void LoadGame(PTANK ptank, PTANK penemytank, char* str);	//读档

//打印相关
void DrawLogo();						//打印图标
void DrawBorder();						//打印边界
void DrawGameHelp();					//打印帮助
void DrawBarr();						//打印障碍
void DrawTank(PTANK ptank, int who);	//打印坦克
void DrawBullet(PBULLET pbullet, PTANK ptank);				//打印子弹
void DrawGameInfo(PTANK ptank, PTANK penemytank);			//打印游戏信息
void DrawMenu(const char** menu, int size, int index);		//打印各种菜单

//坦克相关
void SetTankShape(PTANK ptank);								  //设置坦克形态
int  GetLiveEnemyAmount(PTANK penemytank);					  //存活敌军数量	
void CleanTankTail(COORD oldCore, PCOORD oldBody);			  //清除旧坦克
void ManipulateMyTank(PTANK ptank, int who, PTANK penemytank); //操纵我方坦克
bool IsMyTankMeetOther(PTANK ptank, int dir, PTANK penemytank);//阻挡我方坦克
void ManipulateEneTank(PTANK ptank, int who, PTANK pmytank, PTANK penemytank);//操纵敌方坦克
bool IsEneTankMeetOther(PTANK ptank, int dir, PTANK pmytank, PTANK penemytank);//阻挡敌方坦克

//子弹相关
void MoveBullet(PBULLET pbullet);	//移动子弹
void CleanBullet(COORD oldBulCore);	//清理旧子弹
void IsMyBulMeetOther(PBULLET pbullet, PTANK penemytank, PTANK ptank); //我方子弹碰撞
void IsEneBulMeetOther(PBULLET pbullet, PTANK penemytank, PTANK ptank);//敌方子弹碰撞

//障碍物相关
char* ShowMapFile();			//显示地图文件
void LoadMapFile(char* str);	//导入地图文件
void SetDefaultMap();			//系统默认地图
void CustomizeMap(PTANK ptank, PTANK penemytank);//自定义地图