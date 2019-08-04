#pragma once
#include "Data.h"

void Gotoxy(unsigned x, unsigned y);//光标到指定位置
void SetCursorState(bool b);//设置光标状态
void GameInit();//初始化工作
void DrawMapBorder();//打印地图边界
void setColor(unsigned short ForeColor, unsigned short BackGroundColor);//设置颜色
void DrawWelcome();//欢迎界面
void SelectAction();//开始/退出游戏
void DrawMapBorder();//地图边界
void MoveTank(PTANK ptank);//移动坦克
void CleanTankTail(COORD oldCore, PCOORD oldBody);//清除旧坦克
void DrawTank(PTANK ptank);//打印坦克
void SetTankShape(PTANK ptank);//根据方向选择坦克形态