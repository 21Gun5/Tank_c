#pragma once
#include "Data.h"

void Gotoxy(unsigned x, unsigned y);//��굽ָ��λ��
void SetCursorState(bool b);//���ù��״̬
void GameInit();//��ʼ������
void DrawMapBorder();//��ӡ��ͼ�߽�
void setColor(unsigned short ForeColor, unsigned short BackGroundColor);//������ɫ
void DrawWelcome();//��ӭ����
void SelectAction();//��ʼ/�˳���Ϸ
void DrawMapBorder();//��ͼ�߽�
void MoveTank(PTANK ptank);//�ƶ�̹��
void CleanTankTail(COORD oldCore, PCOORD oldBody);//�����̹��
void DrawTank(PTANK ptank);//��ӡ̹��
void SetTankShape(PTANK ptank);//���ݷ���ѡ��̹����̬