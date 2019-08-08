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
		{0},UP,3,true,�ҷ�̹��,
		{{0},UP,������}
	};
	SetTankShape(&tank);//������̬

	//�з�̹��
	TANK enemyTank[ENEMY_TANK_AMOUNT] = {
		{{2, 2}, {0},DOWN ,2,true,�з�̹��,{{0},UP,������}},
		{{MAP_X_WALL / 4, 2}, {0},UP,1,true,�з�̹��,{{0},UP,������} },
		{{MAP_X_WALL / 2 - 2, 2}, {0},DOWN,2,true,�з�̹��,{{0},UP,������} },

		{{2,  MAP_Y / 2}, {0},UP ,1,true,�з�̹��,{{0},UP,������}},
		{{MAP_X_WALL / 2 - 2,  MAP_Y / 2}, {0},UP,1,true,�з�̹��,{{0},UP,������} },

		{{2,  MAP_Y - 3}, {0},UP ,2,true,�з�̹��,{{0},UP,������}},
		//{{MAP_X_WALL / 4, MAP_Y - 3} ,{0},UP ,2,true,�з�̹��,{{0},UP,������} },
		{{MAP_X_WALL / 2 - 2,  MAP_Y - 3}, {0},UP,2,true,�з�̹��,{{0},UP,������} }
	};
	for (int i = 0; i < ENEMY_TANK_AMOUNT; i++) {SetTankShape(&enemyTank[i]);}//������̬

	//��ʼ������ӭ����
	GameInit();
	//DrawWelcome();

	//���̿���
	int index = 0;//�˵�����
	int flag = 0; //ѡ�������������Ϸ
	while (true)
	{
		DrawWelcome(menuAction, _countof(menuAction), index);
		int action = SelectAction(_countof(menuAction), &index);
		if (action == ENTER)
		{
			switch (index)
			{
			case ��ʼ��Ϸ:
			{
				int index2 = 0;//�˵�����
				int flag2 = 0; //ѡ���ѭ������
				while (true)
				{
					DrawWhoMap(menuWhoMap, _countof(menuWhoMap), index2);
					int whoMap = SelectWhoMap(_countof(menuWhoMap), &index2);
					if (whoMap == ENTER)
					{
						switch (index2)
						{
						case ϵͳĬ��:
						{
							int index4 = 0;
							int flag4 = 0;
							while (true)
							{
								DrawLevel(menuLevel, _countof(menuLevel), index4);
								int level = SelectLevel(_countof(menuLevel), &index4);
								if (level == ENTER)
								{
									switch (index4)
									{
									case ��:
										g_levelEneTank = 300;
										g_levelEneBul = 90;
										flag4 = 1;
										break;
									case һ��:
										g_levelEneTank = 200;
										g_levelEneBul = 70;
										flag4 = 1;
										break;
									case ����:
										g_levelEneTank = 100;
										g_levelEneBul = 50;
										flag4 = 1;
										break;
									default:
										break;
									}
								}
								if (flag4 == 1)
									break;
							}

							BarrierInit();//ʹ��Ĭ�ϵ�
							flag = 1;
							flag2 = 1;
							g_isRunning = 1;//��Ϸ����
							break;
						}
						case ����ṩ:
						{
							int index3 = 0;//�˵�����
							int flag3 = 0; //ѡ���ѭ������
							while(true)
							{
								DrawWhenMap(menuWhenMap, _countof(menuWhenMap), index3);
								int whenMap = SelectWhenMap(_countof(menuWhenMap), &index3);
								if (whenMap == ENTER)
								{
									switch (index3)
									{
									case �½���ͼ:
										flag = 1;
										flag2 = 1;
										flag3 = 1;
										g_isRunning = 1;//Ҫ������Ϸ��
										SetBarrier(&tank, enemyTank);//�ֶ����ò�ֱ��ʹ��
										break;
									case ���е�ͼ:
									{
										flag = 1;
										flag2 = 1;
										flag3 = 1;
										g_isRunning = 1;//Ҫ������Ϸ��
										char* _map = ShowMaps();
										LoadMap(_map);//�������е�ͼ
										break;
									}
									case ������ҳ:
									{
										flag3 = 1;
										break;
									}
									default:
										break;
									}	
								}
								if (flag3 == 1)
									break;
							}
							break;
						}
						case ������ҳ:
						{
							flag2 = 1;
							break;
						}
						default:
							break;
						}
					}				
					if (flag2 == 1)
						break;
				}
				break;
			}
			case ��ȡ��Ϸ:
			{
				flag = 1;
				char* _game = ShowGames();
				LoadGame(&tank, enemyTank, _game);
				break;
			}
			case �˳���Ϸ:
				return 0;
			default:
				break;
			}
		}
		if (flag == 1)//ѡ�������������Ϸ
			break;
	}
	
	//�߽缰�ϰ�
	DrawMapBorder();
	DrawGameHelp();
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
			ManipulateMyTank(&tank, �ҷ�̹��, enemyTank);
			CleanTankTail(oldCore, oldBody);
			DrawTank(&tank,�ҷ�̹��);
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
				DrawBullet(&tank.bullet,&tank);
				IsMyBulMeetOther(&tank.bullet, enemyTank,&tank);
				//IsBulMeetOther(&tank.bullet, enemyTank,&tank);
			}
		}

		//�з�̹���߳�
		if (clock() - time4EnemyTank >= g_levelEneTank)
		{
			for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
			{
				time4EnemyTank = clock();
				COORD oldCore = enemyTank[i].core;
				COORD oldBody[5] = { enemyTank[i].body[0],enemyTank[i].body[1],enemyTank[i].body[2],enemyTank[i].body[3],enemyTank[i].body[4] };
				ManipulateEneTank(&enemyTank[i], �з�̹��,&tank,enemyTank);
				CleanTankTail(oldCore, oldBody);
				DrawTank(&enemyTank[i], �з�̹��);
			}
		}

		//�з��ӵ��߳�
		if (clock() - time4EnemyBullet >= g_levelEneBul)
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
					DrawBullet(&enemyTank[i].bullet, &enemyTank[i]);
					IsEneBulMeetOther(&enemyTank[i].bullet, enemyTank,&tank);
				}
			}
		}

		//�ж���Ϸ����
		if (tank.blood == 0 || GetLiveEnemyAmount(enemyTank) == 0)
		{
			GameOver(enemyTank);
			break;
		}
	}

	// ���Ķ����ַ�
	char ch = _getch();
	ch = _getch();
	return 0;
}