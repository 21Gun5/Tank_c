#pragma once
#include "Data.h"

//系统功能
void SetCursorState(bool b);//设置光标状态
void GotoxyAndPrint(unsigned x, unsigned y,const char * str);//光标到指定位置
void setColor(unsigned short ForeColor, unsigned short BackGroundColor);//设置颜色

//游戏相关
void GameInit();//初始化工作
void DrawWelcome();//欢迎界面
void SelectAction();//开始/退出游戏
void DrawMapBorder();//打印地图边界

//坦克相关
void ManipulateTank(PTANK ptank,int who,PTANK penemytank , PBULLET pbullet);//移动坦克
void DrawTank(PTANK ptank,int who);//打印坦克
void SetTankShape(PTANK ptank);//根据方向选择坦克形态
void CleanTankTail(COORD oldCore, PCOORD oldBody);//清除旧坦克（画前必做
bool IsTankMeetOther(PTANK ptank,int dir, PTANK penemytank);//是否遇到其他障碍/敌方坦克/边界
void ManipulateTank2(PTANK ptank, int who, PTANK pmytank, PTANK penemytank);
bool IsTankMeetOther2(PTANK ptank, int dir, PTANK pmytank, PTANK penemytank);//是否遇到其他障碍/敌方坦克/边界

//子弹相关
void MoveBullet(PBULLET pbullet);//移动子弹
void CleanBullet(COORD oldBulCore);//清理旧子弹（画前必做
void DrawBullet(PBULLET pbullet);//画子弹
void IsBulMeetOther(PBULLET pbullet, PTANK penemytank);//检测子弹是否碰到障碍而消失
//void IsBulMeetOther(PBULLET pbullet);//子弹打障碍/敌坦克

//障碍物相关
void BarrierInit();//初始化障碍物地图
void DrawBarr();//打印障碍物