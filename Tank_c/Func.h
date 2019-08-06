#pragma once
#include "Data.h"

//ϵͳ����
void SetCursorState(bool b);//���ù��״̬
void GotoxyAndPrint(unsigned x, unsigned y,const char * str);//��굽ָ��λ��
void setColor(unsigned short ForeColor, unsigned short BackGroundColor);//������ɫ

//��Ϸ���
void GameInit();//��ʼ������
void DrawWelcome();//��ӭ����
void SelectAction();//��ʼ/�˳���Ϸ
void DrawMapBorder();//��ӡ��ͼ�߽�

//̹�����
void ManipulateTank(PTANK ptank,int who,PTANK penemytank , PBULLET pbullet);//�ƶ�̹��
void DrawTank(PTANK ptank,int who);//��ӡ̹��
void SetTankShape(PTANK ptank);//���ݷ���ѡ��̹����̬
void CleanTankTail(COORD oldCore, PCOORD oldBody);//�����̹�ˣ���ǰ����
bool IsTankMeetOther(PTANK ptank,int dir, PTANK penemytank);//�Ƿ����������ϰ�/�з�̹��/�߽�
void ManipulateTank2(PTANK ptank, int who, PTANK pmytank, PTANK penemytank);
bool IsTankMeetOther2(PTANK ptank, int dir, PTANK pmytank, PTANK penemytank);//�Ƿ����������ϰ�/�з�̹��/�߽�

//�ӵ����
void MoveBullet(PBULLET pbullet);//�ƶ��ӵ�
void CleanBullet(COORD oldBulCore);//������ӵ�����ǰ����
void DrawBullet(PBULLET pbullet);//���ӵ�
void IsBulMeetOther(PBULLET pbullet, PTANK penemytank);//����ӵ��Ƿ������ϰ�����ʧ
//void IsBulMeetOther(PBULLET pbullet);//�ӵ����ϰ�/��̹��

//�ϰ������
void BarrierInit();//��ʼ���ϰ����ͼ
void DrawBarr();//��ӡ�ϰ���