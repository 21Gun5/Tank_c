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
void SelectLevel();//游戏难度

//坦克相关
void DrawTank(PTANK ptank,int who);//打印坦克
void SetTankShape(PTANK ptank);//根据方向选择坦克形态
void CleanTankTail(COORD oldCore, PCOORD oldBody);//清除旧坦克（画前必做
int GetLiveEnemyAmount(PTANK penemytank);//获得存活敌军数量

void ManipulateMyTank(PTANK ptank, int who, PTANK penemytank);//移动坦克
void ManipulateEneTank(PTANK ptank, int who, PTANK pmytank, PTANK penemytank);
bool IsMyTankMeetOther(PTANK ptank, int dir, PTANK penemytank);//是否遇到其他障碍/敌方坦克/边界
bool IsEneTankMeetOther(PTANK ptank, int dir, PTANK pmytank, PTANK penemytank);//是否遇到其他障碍/敌方坦克/边界

//子弹相关
void MoveBullet(PBULLET pbullet);//移动子弹
void CleanBullet(COORD oldBulCore);//清理旧子弹（画前必做
void DrawBullet(PBULLET pbullet, PTANK ptank);//画子弹

void IsMyBulMeetOther(PBULLET pbullet, PTANK penemytank,PTANK ptank);//检测子弹是否碰到障碍而消失
void IsEneBulMeetOther(PBULLET pbullet, PTANK penemytank, PTANK ptank);

//障碍物相关
void BarrierInit();//默认障碍物地图
void SetBarrier(PTANK ptank, PTANK penemytank);//手动设置障碍
char* ShowMaps();//显示所有地图并选择
void LoadMap(char* str);//导入地图
void DrawBarr();//打印障碍物

//游戏存档
void SaveGame(PTANK ptank, PTANK penemytank);//保存游戏
char* ShowGames();//显示所有存档并选择
void LoadGame(PTANK ptank, PTANK penemytank, char* str);//读档