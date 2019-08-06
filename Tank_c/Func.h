#pragma once
#include "Data.h"

//系统功能
void SetCursorState(bool b);//设置光标状态
void GotoxyAndPrint(unsigned x, unsigned y,const char * str);//光标到指定位置
void setColor(unsigned short ForeColor, unsigned short BackGroundColor);//设置颜色
void PlayBGM();//播放BGM/音效

//游戏相关
void GameInit();//初始化工作
void GameOver(PTANK penemytank);//善后工作

void DrawWelcome();//欢迎界面
void DrawMapBorder();//打印地图边界
void DrawGameHelp();//打印帮助信息
void DrawGameInfo(PTANK ptank, PTANK penemytank);//打印游戏信息

int SelectAction();//开始/退出游戏
int SelectWhoMap();//选择系统地图or自定义地图
int SelectWhenMap();//新绘制or老地图

int GetLiveEnemyAmount(PTANK penemytank);

void SetMap();

//坦克相关
void DrawTank(PTANK ptank,int who);//打印坦克
void SetTankShape(PTANK ptank);//根据方向选择坦克形态
void CleanTankTail(COORD oldCore, PCOORD oldBody);//清除旧坦克（画前必做

void ManipulateTank(PTANK ptank, int who, PTANK penemytank);//移动坦克
void ManipulateTank2(PTANK ptank, int who, PTANK pmytank, PTANK penemytank);
bool IsTankMeetOther(PTANK ptank, int dir, PTANK penemytank);//是否遇到其他障碍/敌方坦克/边界
bool IsTankMeetOther2(PTANK ptank, int dir, PTANK pmytank, PTANK penemytank);//是否遇到其他障碍/敌方坦克/边界

//子弹相关
void MoveBullet(PBULLET pbullet);//移动子弹
void CleanBullet(COORD oldBulCore);//清理旧子弹（画前必做
void DrawBullet(PBULLET pbullet);//画子弹

void IsBulMeetOther(PBULLET pbullet, PTANK penemytank);//检测子弹是否碰到障碍而消失
void IsBulMeetOther2(PBULLET pbullet, PTANK penemytank, PTANK ptank);

//障碍物相关
void BarrierInit();//初始化障碍物地图
void DrawBarr();//打印障碍物