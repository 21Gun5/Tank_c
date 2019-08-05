#include <stdio.h>
#include <time.h>
#include "Data.h"
#include "Func.h"
#include <conio.h>

int main()
{
	//ģ����߳�
	int time4Tank = 0;
	int time4EnemyTank = 0;
	int time4Bullet = 0;

	//�ҷ�̹��
	TANK tank = { {MAP_X_WALL / 4, MAP_Y / 2}, {0},UP };
	g_MAP[tank.core.X][tank.core.Y] = �ҷ�̹��;
	SetTankShape(&tank,�ҷ�̹��);//�����ҷ�̹����̬

	//�з�̹��
	TANK enemyTank[3] = {
		{{2, 2}, {0},DOWN },
		{{MAP_X_WALL / 4, 2}, {0},DOWN },
		{{MAP_X_WALL/2 -2, 2}, {0},DOWN }
	};
	g_MAP[2][2] = �з�̹��;
	g_MAP[MAP_X_WALL / 4][2] = �з�̹��;
	g_MAP[MAP_X_WALL / 2 - 2][2] = �з�̹��;
	for (int i = 0; i < 3; i++) {SetTankShape(&enemyTank[i],�з�̹��);}//���õз�̹����̬

	
	//�ӵ�����
	BULLET bullet = {{0},UP};

	//��������
	GameInit();
	DrawWelcome();
	SelectAction();
	DrawMapBorder();
	BarrierInit();
	DrawBarr();

	//��ѭ��
	while (g_isRunning)
	{
		//�ҷ�̹���ƶ��߳�
		if (clock() - time4Tank >= 100)
		{
			time4Tank = clock();
			COORD oldCore = tank.core;
			COORD oldBody[5] = { tank.body[0],tank.body[1],tank.body[2],tank.body[3],tank.body[4] };
			ManipulateTank(&tank,�ҷ�̹��,enemyTank);
			CleanTankTail(oldCore, oldBody);
			DrawTank(&tank,�ҷ�̹��);
		}

		//�ҷ��ӵ��ƶ��߳�
		if (g_isBulExist != 0)//1��2���ɣ�ֻҪ��Ϊ0
		{
			//�ӵ���ֵ
			if (g_isBulExist == 1)//==1δ��ֵ״̬
			{
				bullet = { {tank.body[0].X, tank.body[0].Y}, tank.dir };
				g_isBulExist++;//==2�Ѹ�ֵ״̬
			}
			//�ӵ��ƶ�
			if (clock() - time4Bullet >= 50)
			{
				time4Bullet = clock();
				COORD oldBulCore = bullet.core;
				MoveBullet(&bullet);
				CleanBullet(oldBulCore);
				DrawBullet(&bullet);
				IsBulMeetOther(&bullet);
			}
		}

		//�з�̹���ƶ��߳�
		if (clock() - time4EnemyTank >= 5)
		{
			for (int i = 0; i < 3; i++)
			{
				time4EnemyTank = clock();
				COORD oldCore = enemyTank[i].core;
				COORD oldBody[5] = { enemyTank[i].body[0],enemyTank[i].body[1],enemyTank[i].body[2],enemyTank[i].body[3],enemyTank[i].body[4] };
				ManipulateTank2(&enemyTank[i], �з�̹��,&tank,enemyTank);
				CleanTankTail(oldCore, oldBody);
				DrawTank(&enemyTank[i], �з�̹��);
			}
		}

	}
	return 0;
}