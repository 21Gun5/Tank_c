#pragma once
#include <Windows.h>

//�궨��
#define MAP_X 90		//�ұ߽�
#define MAP_Y 30		//�±߽�
#define MAP_X_WALL 60	//�ұ߽�
#define ��ʼ��Ϸ 1
#define �˳���Ϸ 2

#define �յ� 0
#define ��ͼ�߽� 1
#define �ϰ��� 2
#define �ҷ�̹�� 3
#define �з�̹�� 4



//ȫ�ֱ���
extern int g_MAP[MAP_X][MAP_Y];	//��ͼ����
extern bool g_isRunning;		//��Ϸ�Ƿ�����
extern int g_isBulExist;		//�ӵ��Ƿ񻹴��ڣ�û���ڱ߽���ϰ�����
//extern int g_Bar[MAP_X_WALL][MAP_Y];//�ϰ�������
//extern int g_EneTank[MAP_X_WALL][MAP_Y];//�з�̹������

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
