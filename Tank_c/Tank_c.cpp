#include <stdio.h>
#include <time.h>
#include "Data.h"
#include "Func.h"
#include <conio.h>

int main()
{
	int nTime1 = 0;
	int nTime2 = 0;

	TANK tank = { {MAP_X_WALL / 4, MAP_Y / 2}, {0},UP };
	SetTankShape(&tank);	//����̹����̬
	
	GameInit();
	DrawWelcome();
	SelectAction();
	DrawMapBorder();
	BarrierInit();
	DrawBarr();

	//g_isRunning = 0;
	while (g_isRunning)
	{

		//̹���ƶ��߳�
		if (clock() - nTime1 >= 100)
		{
			nTime1 = clock();
			COORD oldCore = tank.core;
			COORD oldBody[5] = { tank.body[0],tank.body[1],tank.body[2],tank.body[3],tank.body[4] };
			MoveTank(&tank);
			CleanTankTail(oldCore, oldBody);
			DrawTank(&tank);
		}

		//�ӵ��ƶ��߳�
		if (g_isBulExist != 0)//1��2���ɣ�ֻҪ��Ϊ0
		{
			//�ӵ���������ֵ
			static BULLET bullet;//�����ſɴ�������
			if (g_isBulExist == 1)//ֻ��1ʱ���ſɸ�ֵ
			{
				bullet = { {tank.body[0].X, tank.body[0].Y}, tank.dir };//������̹���ṩ
				g_isBulExist++;//����++����ʱΪ2��ֻ��1�ſɸ�ֵ
			}

			//�ӵ��ƶ�
			if (clock() - nTime2 >= 50)
			{
				nTime2 = clock();
				COORD oldBulCore = bullet.core;//��¼�¾ɵ�ַ
				MoveBullet(&bullet);//�ƶ�
				CleanBullet(oldBulCore);//�����
				DrawBullet(&bullet);//��
				IsBulMeetOther(&bullet);//�Ƿ�����������
			}
		}
	}
	return 0;
}