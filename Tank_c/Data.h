#pragma once
#include <Windows.h>

//�궨��
#define MAP_X 90		//�ұ߽�
#define MAP_Y 30		//�±߽�
#define MAP_X_WALL 60	//�ұ߽�

#define ENEMY_TANK_AMOUNT 3

#define ��ʼ��Ϸ 1
#define �˳���Ϸ 2

#define �ҷ�̹�� 0
#define �з�̹�� 1

#define �յ� 0
#define ��ͼ�߽� 1
#define �ϰ��� 2

#define ������ 0
#define δ��ֵ 1
#define �Ѹ�ֵ 2




//ȫ�ֱ���
extern int g_MAP[MAP_X][MAP_Y];	//��ͼ����
extern bool g_isRunning;		//��Ϸ�Ƿ�����

//extern int g_isBulExist;		//�ӵ��Ƿ񻹴��ڣ�û���ڱ߽���ϰ�����
//extern int g_enemyTankAmount;	//��̹����
//extern int g_Bar[MAP_X_WALL][MAP_Y];//�ϰ�������
//extern int g_EneTank[MAP_X_WALL][MAP_Y];//�з�̹������

//����ö��
enum direction { UP, DOWN, LEFT, RIGHT };

//�ӵ��ṹ��
typedef struct _BULLET
{
	COORD core;			//����
	enum  direction dir;//����
	int state;			//�ӵ�״̬
}BULLET,*PBULLET;

//̹�˽ṹ��
typedef struct _TANK
{
	COORD core;			//���ĵ�
	COORD body[5];		//�����ڵ�
	enum direction dir;	//����
	unsigned blood;		//Ѫ��
	bool isAlive;		//�Ƿ���
	BULLET bullet;		//�ӵ�
}TANK, * PTANK;