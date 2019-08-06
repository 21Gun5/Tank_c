#pragma once
#include "Data.h"

//ϵͳ����
void SetCursorState(bool b);//���ù��״̬
void GotoxyAndPrint(unsigned x, unsigned y,const char * str);//��굽ָ��λ��
void setColor(unsigned short ForeColor, unsigned short BackGroundColor);//������ɫ
void PlayBGM();//����BGM/��Ч

//��Ϸ���
void GameInit();//��ʼ������
void GameOver(PTANK penemytank);//�ƺ���

void DrawWelcome();//��ӭ����
void DrawMapBorder();//��ӡ��ͼ�߽�
void DrawGameHelp();//��ӡ������Ϣ
void DrawGameInfo(PTANK ptank, PTANK penemytank);//��ӡ��Ϸ��Ϣ

int SelectAction();//��ʼ/�˳���Ϸ
int SelectWhoMap();//ѡ��ϵͳ��ͼor�Զ����ͼ
int SelectWhenMap();//�»���or�ϵ�ͼ

int GetLiveEnemyAmount(PTANK penemytank);

void SetMap();

//̹�����
void DrawTank(PTANK ptank,int who);//��ӡ̹��
void SetTankShape(PTANK ptank);//���ݷ���ѡ��̹����̬
void CleanTankTail(COORD oldCore, PCOORD oldBody);//�����̹�ˣ���ǰ����

void ManipulateTank(PTANK ptank, int who, PTANK penemytank);//�ƶ�̹��
void ManipulateTank2(PTANK ptank, int who, PTANK pmytank, PTANK penemytank);
bool IsTankMeetOther(PTANK ptank, int dir, PTANK penemytank);//�Ƿ����������ϰ�/�з�̹��/�߽�
bool IsTankMeetOther2(PTANK ptank, int dir, PTANK pmytank, PTANK penemytank);//�Ƿ����������ϰ�/�з�̹��/�߽�

//�ӵ����
void MoveBullet(PBULLET pbullet);//�ƶ��ӵ�
void CleanBullet(COORD oldBulCore);//������ӵ�����ǰ����
void DrawBullet(PBULLET pbullet);//���ӵ�

void IsBulMeetOther(PBULLET pbullet, PTANK penemytank);//����ӵ��Ƿ������ϰ�����ʧ
void IsBulMeetOther2(PBULLET pbullet, PTANK penemytank, PTANK ptank);

//�ϰ������
void BarrierInit();//��ʼ���ϰ����ͼ
void DrawBarr();//��ӡ�ϰ���