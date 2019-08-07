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
void SelectLevel();//��Ϸ�Ѷ�

//̹�����
void DrawTank(PTANK ptank,int who);//��ӡ̹��
void SetTankShape(PTANK ptank);//���ݷ���ѡ��̹����̬
void CleanTankTail(COORD oldCore, PCOORD oldBody);//�����̹�ˣ���ǰ����
int GetLiveEnemyAmount(PTANK penemytank);//��ô��о�����

void ManipulateMyTank(PTANK ptank, int who, PTANK penemytank);//�ƶ�̹��
void ManipulateEneTank(PTANK ptank, int who, PTANK pmytank, PTANK penemytank);
bool IsMyTankMeetOther(PTANK ptank, int dir, PTANK penemytank);//�Ƿ����������ϰ�/�з�̹��/�߽�
bool IsEneTankMeetOther(PTANK ptank, int dir, PTANK pmytank, PTANK penemytank);//�Ƿ����������ϰ�/�з�̹��/�߽�

//�ӵ����
void MoveBullet(PBULLET pbullet);//�ƶ��ӵ�
void CleanBullet(COORD oldBulCore);//������ӵ�����ǰ����
void DrawBullet(PBULLET pbullet, PTANK ptank);//���ӵ�

void IsMyBulMeetOther(PBULLET pbullet, PTANK penemytank,PTANK ptank);//����ӵ��Ƿ������ϰ�����ʧ
void IsEneBulMeetOther(PBULLET pbullet, PTANK penemytank, PTANK ptank);

//�ϰ������
void BarrierInit();//Ĭ���ϰ����ͼ
void SetBarrier(PTANK ptank, PTANK penemytank);//�ֶ������ϰ�
char* ShowMaps();//��ʾ���е�ͼ
void LoadMap(char* str);//�����ͼ
void DrawBarr();//��ӡ�ϰ���