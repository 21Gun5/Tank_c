#pragma once
#include <Windows.h>

//�궨��
#define MAP_X 90		//�ұ߽�
#define MAP_Y 30		//�±߽�
#define MAP_X_WALL 60	//�ұ߽�
#define ��ʼ��Ϸ 1
#define �˳���Ϸ 2

//ȫ�ֱ���
extern int g_MAP[MAP_X][MAP_Y];	//��ͼ����
extern bool g_isRunning;		//��Ϸ�Ƿ�����

//����ö��
enum direction { UP, DOWN, LEFT, RIGHT };

//̹�˽ṹ��
typedef struct _TANK
{
	COORD core;			//���ĵ�
	COORD body[5];
	enum direction dir;	//����
}TANK, * PTANK;