#include <stdio.h>
#include <time.h>
#include "Data.h"
#include "Func.h"
#include <conio.h>

int main()
{
	//ģ����߳�
	int time4Tank = 0;
	int time4Bullet = 0;
	int time4EnemyTank = 0;
	int time4EnemyBullet = 0;
	
	//�ҷ�̹��
	TANK tank = {
		{MAP_X_WALL / 4, MAP_Y / 2},
		{0},UP,3,true,
		{{0},UP,������}
	};
	SetTankShape(&tank);//������̬

	//�з�̹��
	TANK enemyTank[ENEMY_TANK_AMOUNT] = {
		{{2, 2}, {0},DOWN ,1,true,{{0},UP,������}},
		{{MAP_X_WALL / 4, 2}, {0},UP,1,true,{{0},UP,������} },
		{{MAP_X_WALL / 2 - 2, 2}, {0},DOWN,1,true,{{0},UP,������} },

		{{2,  MAP_Y / 2}, {0},UP ,1,true,{{0},UP,������}},
		{{MAP_X_WALL / 2 - 2,  MAP_Y / 2}, {0},UP,1,true,{{0},UP,������} },

		{{2,  MAP_Y - 3}, {0},UP ,1,true,{{0},UP,������}},
		{{MAP_X_WALL / 4, MAP_Y - 3} ,{0},UP ,1,true,{{0},UP,������} },
		{{MAP_X_WALL / 2 - 2,  MAP_Y - 3}, {0},UP,1,true,{{0},UP,������} }
	};
	for (int i = 0; i < ENEMY_TANK_AMOUNT; i++) {SetTankShape(&enemyTank[i]);}//������̬

	//��������
	GameInit();
	DrawWelcome();
	SelectAction();
	DrawMapBorder();
	DrawGameHelp();
	BarrierInit();
	DrawBarr();

	//��ѭ��
	while (g_isRunning)
	{
		//��Ϣʵʱ��ʾ
		DrawGameInfo(&tank, enemyTank);

		//�ҷ�̹���߳�
		if (clock() - time4Tank >= 100)
		{
			time4Tank = clock();
			COORD oldCore = tank.core;
			COORD oldBody[5] = { tank.body[0],tank.body[1],tank.body[2],tank.body[3],tank.body[4] };
			//ManipulateTank(&tank,�ҷ�̹��,enemyTank,&bullet);
			ManipulateTank(&tank, �ҷ�̹��, enemyTank);
			CleanTankTail(oldCore, oldBody);
			DrawTank(&tank,�ҷ�̹��);
			//�Ƿ���
			if (tank.blood == 0)
			{
				GameOver(enemyTank);
				break;
			}
		}

		//�ҷ��ӵ��߳�
		if (clock() - time4Bullet >= 50)
		{
			if (tank.bullet.state != ������)
			{
				//�ӵ���ֵ
				if (tank.bullet.state == δ��ֵ)//==1δ��ֵ״̬
				{
					tank.bullet = { {tank.body[0].X, tank.body[0].Y}, tank.dir };
					tank.bullet.state = �Ѹ�ֵ;
					//g_isBulExist++;//==2�Ѹ�ֵ״̬
				}
				//�ӵ��ƶ�
				time4Bullet = clock();
				COORD oldBulCore = tank.bullet.core;
				MoveBullet(&tank.bullet);
				CleanBullet(oldBulCore);
				DrawBullet(&tank.bullet);
				IsBulMeetOther(&tank.bullet, enemyTank);
				//IsBulMeetOther(&tank.bullet, enemyTank,&tank);
			}
		}

		//�з�̹���߳�
		if (clock() - time4EnemyTank >= 100)
		{
			for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
			{
				time4EnemyTank = clock();
				COORD oldCore = enemyTank[i].core;
				COORD oldBody[5] = { enemyTank[i].body[0],enemyTank[i].body[1],enemyTank[i].body[2],enemyTank[i].body[3],enemyTank[i].body[4] };
				ManipulateTank2(&enemyTank[i], �з�̹��,&tank,enemyTank);
				CleanTankTail(oldCore, oldBody);
				DrawTank(&enemyTank[i], �з�̹��);
			}
		}

		//�з��ӵ��߳�
		if (clock() - time4EnemyBullet >= 50)
		{
			for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
			{
				if (enemyTank[i].bullet.state != ������)
				{
					//�ӵ���ֵ
					if (enemyTank[i].bullet.state == δ��ֵ)//==1δ��ֵ״̬
					{
						enemyTank[i].bullet = { {enemyTank[i].body[0].X, enemyTank[i].body[0].Y}, enemyTank[i].dir };
						enemyTank[i].bullet.state = �Ѹ�ֵ;
					}
					//�ӵ��ƶ�
					time4EnemyBullet = clock();
					COORD oldBulCore = enemyTank[i].bullet.core;
					MoveBullet(&enemyTank[i].bullet);
					CleanBullet(oldBulCore);
					DrawBullet(&enemyTank[i].bullet);
					IsBulMeetOther2(&enemyTank[i].bullet, enemyTank,&tank);
				}
			}
		}
	}

	// ���Ķ����ַ�
	char ch = _getch();
	ch = _getch();
	return 0;
}