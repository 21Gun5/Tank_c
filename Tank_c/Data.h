#pragma once
#include <Windows.h>

//�궨��
#define MAP_X 90		//�ұ߽�
#define MAP_Y 30		//�±߽�
#define MAP_X_WALL 60	//�ұ߽�
#define ��ʼ��Ϸ 1
#define �˳���Ϸ 2

#define δ���� 0
#define ������δ��ֵ 1
#define �������˶� 2
#define ��ʧ 0


//ȫ�ֱ���
extern int g_MAP[MAP_X][MAP_Y];	//��ͼ����
extern bool g_isRunning;		//��Ϸ�Ƿ�����
extern int g_isBulExist;		//�ӵ��Ƿ񻹴��ڣ�û���ڱ߽���ϰ�����
extern int g_Bar[MAP_X_WALL][MAP_Y];//�ϰ�������

//����ö��
enum direction { UP, DOWN, LEFT, RIGHT };

//̹�˽ṹ��
typedef struct _TANK
{
	COORD core;			//���ĵ�
	COORD body[5];		//�����ڵ�
	enum direction dir;	//����
}TANK, * PTANK;

//�ӵ��ṹ��
typedef struct _BULLET
{
	COORD core;			//����
	enum  direction dir;//����
}BULLET,*PBULLET;

//�ϰ���ṹ��
//typedef struct _BARRIER
//{
//	PCOORD pbody;
//	int count = 20;
//}BARRIER, * PBARRIER;