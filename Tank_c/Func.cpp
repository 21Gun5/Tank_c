#include "Func.h"
#include "Data.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#pragma comment(lib,"winmm.lib")

//ϵͳ����
void GotoxyAndPrint(unsigned x, unsigned y, const char* str)
{
	COORD cur;
	cur.X = x * 2;
	cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
	printf(str);
}
void SetCursorState(bool b)
{

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);	//��ȡ����̨�����Ϣ
	CursorInfo.bVisible = b;					//��ʾ/���ؿ���̨���
	SetConsoleCursorInfo(handle, &CursorInfo);	//���ÿ���̨���״̬
}
void setColor(unsigned short ForeColor, unsigned short BackGroundColor)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);					//��ȡ��ǰ���ھ��
	SetConsoleTextAttribute(handle, ForeColor + BackGroundColor * 0x10);//������ɫ
}
void PlayBGM()
{
	/*
	1. ���ű������֣���ѭ��)
	2. ����Ϸ���ţ�������ֹͣ
	3. ��ͣʱֹͣ���ָ�ʱ����
	*/

	// ����Ƶ�ļ�������ʱ�ر�
	mciSendString("open conf/BGM.mp3 alias bgm", NULL, 0, NULL);//�������ɴ�д
	mciSendString("play bgm repeat", NULL, 0, NULL);			// ѭ������,������.mp3��ʽ
}

//��Ϸ���
void GameInit()
{
	//���õ�ͼ
	for (int x = 0; x < MAP_X/2; x++)
	{
		for (int y = 0; y < MAP_Y; y++)
		{
			//��ͼ�߽�
			if (x == 0 || x == MAP_X/2-1 || x == MAP_X_WALL/2 ||	//������
				y == 0 || y == MAP_Y - 1 ||							//�����
				(x > MAP_X_WALL/2 && y == MAP_Y / 2))				//������Ϣ����Ϸ��Ϣ�ָ���
			{
				g_MAP[x][y] = ��ͼ�߽�;
			}
		}
	}

	//�����ϰ���
	/*for (int x = 0; x < MAP_X_WALL; x++)
	{
		for (int y = 0; y < MAP_Y; y++)
		{
			if (
				(x > MAP_X_WALL / 4 - 9 && x < MAP_X_WALL / 4 - 5 && y > MAP_Y / 2 - 9 && y < MAP_Y / 2 - 4) ||
				(x < MAP_X_WALL / 4 + 9 && x > MAP_X_WALL / 4 + 5 && y > MAP_Y / 2 - 9 && y < MAP_Y / 2 - 4) ||
				(x > MAP_X_WALL / 4 - 9 && x < MAP_X_WALL / 4 - 5 && y < MAP_Y / 2 + 9 && y > MAP_Y / 2 + 4) ||
				(x < MAP_X_WALL / 4 + 9 && x > MAP_X_WALL / 4 + 5 && y < MAP_Y / 2 + 9 && y > MAP_Y / 2 + 4)
				)
			{
				g_MAP[x][y] = �ϰ���;
			}

		}
	}*/

	//���ع��
	SetCursorState(false);

	//���ñ���
	system("title Tank");

	//��ʼ�����������
	srand((unsigned int)time(0));

	//���ű�������
	//PlayBGM();
}
void GameOver(PTANK penemytank)
{
	//����̹����
	int eneTankCount = GetLiveEnemyAmount(penemytank);

	//�ر������ļ�
	//mciSendString("close bgm", NULL, 0, NULL);	//close�رն���stopֹͣ

	//��ʾ��Ϣ
	setColor(12, 0);
	GotoxyAndPrint(MAP_X_WALL / 4 - 2, MAP_Y / 2 - 7, "GAME OVER! ");

	if (eneTankCount == 0)
		GotoxyAndPrint(MAP_X_WALL / 4 - 2, MAP_Y / 2 - 5, "A Winner!");
	else
		GotoxyAndPrint(MAP_X_WALL / 4 - 2, MAP_Y / 2 - 5, "A Loser!");
	GotoxyAndPrint(MAP_X_WALL / 4 -2, MAP_Y / 2 - 3,"");

	printf("Scores: %d", ENEMY_TANK_AMOUNT-eneTankCount);
	setColor(7, 0);

}

void DrawMapBorder()
{
	system("cls");						//��ҳ������
	for (int x = 0; x < MAP_X; x++)
	{
		for (int y = 0; y < MAP_Y; y++)
		{
			if (g_MAP[x][y] == ��ͼ�߽�)
			{
				GotoxyAndPrint(x, y, "��");
			}
			//else if (g_MAP[x][y] == �ϰ���)
			//{
			//	GotoxyAndPrint(x, y, "��");
			//}
		}
	}
}
void DrawWelcome()
{
	//��ӡͼ��
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2-10, "Welcome!");
	//��ʾ��Ϣ
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 6, "1. ����Ϸ\n");
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 4, "2. �˳���Ϸ\n");
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 2, "������ѡ��-> ");
}
void DrawGameInfo(PTANK ptank, PTANK penemytank)
{
	//����̹����
	int eneTankCount = GetLiveEnemyAmount(penemytank);

	//����or��ͣ״̬
	setColor(12, 0);
	GotoxyAndPrint(MAP_X / 2 - 14, 1, "RUNNING");
	GotoxyAndPrint(MAP_X / 2 - 14, 2, "Q: ��ͣ��Ϸ");
	setColor(7, 0);

	//��Ϸ��Ϣ��ӡ
	setColor(12, 0);
	GotoxyAndPrint(MAP_X / 2 - 11, 5, "");
	printf("��ǰ����: %d", ptank->blood);
	GotoxyAndPrint(MAP_X / 2 - 11, 7, "");
	printf("��ǰ����: %d", ENEMY_TANK_AMOUNT - eneTankCount);
	GotoxyAndPrint(MAP_X / 2 - 11, 9, "");
	printf("��̹����: %d", eneTankCount);
	setColor(7, 0);
}
void DrawGameHelp()
{
	setColor(12, 0);
	GotoxyAndPrint(MAP_X / 2 - 10, 18, "����˵��");
	GotoxyAndPrint(MAP_X / 2 - 11, 20, "W: ��  S: ��");
	GotoxyAndPrint(MAP_X / 2 - 11, 22, "A: ��  D: ��");
	GotoxyAndPrint(MAP_X / 2 - 11, 24, "Q:  ��ͣ��Ϸ");
	GotoxyAndPrint(MAP_X / 2 - 11, 26, "�ո�: ����");
	
	setColor(7, 0);
}

int SelectAction()
{
	SetCursorState(true);			//�û�����ʱ�����ɼ�
	int input = _getch() - '0';	//�޻��Խ��գ�-'0'��֤��0-9������ASCII
	//int input = 1;
	SetCursorState(false);		//���������ع��

	switch (input)
	{
	case ��ʼ��Ϸ:
	{
		g_isRunning = true;
		break;
	}
	case �˳���Ϸ:
	{
		GotoxyAndPrint(MAP_X / 2 - 25, MAP_Y / 2 + 3, "Bye! \n");
		break;
	}
	default:
		GotoxyAndPrint(MAP_X / 2 - 25, MAP_Y / 2 + 3, "�������\n");
		break;
	}
	return input;
}
int SelectWhoMap()
{
	//��ʾ��Ϣ
	system("cls");				//��ҳ������
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 6, "��ѡ���ͼ��");
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 4, "1. ϵͳĬ��");
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 2, "2. ����ṩ");
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2,"������ѡ��-> ");

	SetCursorState(true);		//�û�����ǰ��ʾ���
	int input = _getch() - '0';	//������0-9������ASCII
	SetCursorState(false);		//������������ع��

	return input;
}
int SelectWhenMap()
{
	//��ʾ��Ϣ
	system("cls");
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 6, "��ͼѡ��");
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 4, "1. �½���ͼ");
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2 - 2, "2. ���е�ͼ");
	GotoxyAndPrint(MAP_X / 4 - 5, MAP_Y / 2,"������ѡ��-> ");

	SetCursorState(true);//��ʾ���
	int input = _getch() - '0';
	SetCursorState(false);//���ع��

	return input;
}

int GetLiveEnemyAmount(PTANK penemytank)
{
	int count = 0;
	for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
	{
		if (penemytank[i].isAlive)
			count++;
	}
	return count;
}

void SetMap()
{
	DrawMapBorder();		//��ͼ�߽�

	//��ʾ��Ϣ
	setColor(12, 0);
	GotoxyAndPrint(MAP_X / 2 - 12, 4, "     �༭��ͼ");
	GotoxyAndPrint(MAP_X / 2 - 12, 6, "��������������ϰ�");
	GotoxyAndPrint(MAP_X / 2 - 12, 8, "�Ҽ������������ϰ�");
	GotoxyAndPrint(MAP_X / 2 - 12, 10, "����˫�����˳��༭");
	setColor(7, 0);

	//����¼����
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD ir = {};
	DWORD dwCount = 0;
	SetConsoleMode(hInput, ENABLE_MOUSE_INPUT);

	//��������¼�����������Ļ
	while (true)
	{
		ReadConsoleInput(hInput, &ir, 1, &dwCount);
		if (ir.EventType == MOUSE_EVENT)
		{
			//�������
			if (ir.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				COORD pos = ir.Event.MouseEvent.dwMousePosition;//��ȡ������λ��
				if (pos.X > 0 && pos.X < MAP_X_WALL && pos.Y >0 && pos.Y < MAP_Y)
				{
					g_MAP[pos.X/2][pos.Y] = �ϰ���;
					GotoxyAndPrint(pos.X/2 , pos.Y, "��");
				}
			}
			//�Ҽ�����
			if (ir.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			{
				COORD pos = ir.Event.MouseEvent.dwMousePosition;
				if (pos.X > 0 && pos.X < MAP_X_WALL && pos.Y >0 && pos.Y < MAP_Y)
				{
					g_MAP[pos.X/2][pos.Y] = �յ�;
					GotoxyAndPrint(pos.X/2, pos.Y, "  ");
				}
			}
			//˫���˳�
			if (ir.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK)
			{
				COORD pos = ir.Event.MouseEvent.dwMousePosition;
				if (!(pos.X > 0 && pos.X < MAP_X_WALL && pos.Y >0 && pos.Y < MAP_Y))
				{
					//��ͼ��˫�����˳�
					break;
				}
			}
		}
	}

	////����Ļ״̬�����ά����
	//for (int i = 0; i < MAP_X_WALL; i++)
	//{
	//	for (int j = 0; j < MAP_Y; j++)
	//	{
	//		if (g_BarrMAP[i][j] == 1)
	//		{
	//			/*
	//			ԭ����51������x����������/2*2����5/2*2=4������41
	//			��ֱ�ӵ�X=x*2���Ǹ��ƶ�����������81��ӡ
	//			��Ҫ����ȷ��41������/2
	//			������Ӱ����ص����x��һ����λ����y��������λ���ӣ����ǣ�
	//			*/
	//			int t = i / 2;//5/2=2
	//			t = t * 2;//2*2=4���������
	//			COORD tmp = { t / 2,j };//����Ϊ��ӡ��ʱ��Ҫ*2�����ٳ���
	//			BarrTmp.push_back(tmp);
	//			barrTmpSize++;
	//		}
	//	}
	//}

	////��ʾ��Ϣ
	//system("cls");
	//setColor(12, 0);
	//GotoxyAndPrint(MAP_X - 24, 12,"�������ͼ����");
	//GotoxyAndPrint(MAP_X - 24, 14,"");
	//char str[20];
	//scanf_s("%s", str, 20);
	//setColor(7, 0);
	//char _file[] = str + ".i";				//�ļ���
	//string filename = "conf\\map\\" + _file;	//������·�����ļ�
	////����д���ļ�
	//FILE* pFile = NULL;
	//errno_t err = fopen_s(&pFile, filename.c_str(), "wb");
	//fwrite(&barrTmpSize, sizeof(int), 1, pFile);//д���ϰ�������
	//for (int i = 0; i < BarrTmp.size(); i++)	//����д���ϰ���
	//{
	//	fwrite(&BarrTmp[i], sizeof(COORD), 1, pFile);
	//}
	//fclose(pFile);

	//return _file;
}

//string ShowMaps()
//{
//	/*
//	1. ��ʾ���е�ͼ�Թ��û�ѡ��
//	2. ����ѡ����ļ���
//	*/
//
//	//����ָ��Ŀ¼���ļ���here
//	string inPath = "conf/map/*.i";
//	long handle;							//���ڲ��ҵľ��
//	_finddata_t fileinfo;
//	handle = _findfirst(inPath.c_str(), &fileinfo);
//	if (handle == -1)
//		return 0;
//	do
//	{
//		g_Maps.push_back(fileinfo.name);	//���ļ�����������
//	} while (!_findnext(handle, &fileinfo));
//	_findclose(handle);
//
//	//��ʾĿ¼
//	system("cls");
//	Gotoxy(MAP_X / 2 - 10, MAP_Y / 2 - 8);
//	cout << "��ѡ���ͼ" << endl;
//
//	int i = 0;								//ѭ��������for�ⶨ��
//	for (; i < g_Maps.size(); i++)
//	{
//		Gotoxy(MAP_X / 2 - 10, MAP_Y / 2 - 6 + i);
//		cout << i + 1 << ". " << g_Maps[i] << endl;
//	}
//	Gotoxy(MAP_X / 2 - 10, MAP_Y / 2 - 6 + i);
//	cout << "������ѡ��-> ";
//
//	int input = _getch() - 48;				//��֤0-9����ASCII
//	string _file = g_Maps[input - 1];		//����ʼ��1�����±�ʼ��0
//
//	return _file;
//}
//void LoadMap(CBarrier& barrier, string str)
//{
//	string filename = "conf\\map\\" + str;
//
//	COORD tmp;
//	FILE* pFile = NULL;
//	errno_t err = fopen_s(&pFile, filename.c_str(), "rb");
//	fread(&barrier.m_size, sizeof(int), 1, pFile);	//��ȡ�ϰ�������
//	for (int i = 0; i < barrier.m_size; i++)		//������ȡ�ϰ���
//	{
//		fread(&tmp, sizeof(COORD), 1, pFile);
//		barrier.m_BarrArr.push_back(tmp);			//����tmp����������ֱ�Ӷ���
//	}
//	fclose(pFile);
//}

//̹�����
void CleanTankTail(COORD oldCore, PCOORD oldBody)
{
	GotoxyAndPrint(oldCore.X, oldCore.Y, "  ");//���ĵ�
	for (int i = 0; i < 5; i++)//������
	{
		GotoxyAndPrint(oldBody[i].X, oldBody[i].Y, "  ");
	}
}
void DrawTank(PTANK ptank, int who)
{
	if (ptank->isAlive == false) return;
	if (who == �ҷ�̹��)
	{
		setColor(10, 0);
		GotoxyAndPrint(ptank->core.X, ptank->core.Y, "��");//���ĵ�
		for (int i = 0; i < 5; i++)//������
		{
			GotoxyAndPrint(ptank->body[i].X, ptank->body[i].Y, "��");
		}
		setColor(7, 0);
	}
	else if (who == �з�̹��)
	{
		setColor(10, 0);
		GotoxyAndPrint(ptank->core.X, ptank->core.Y, "��");//���ĵ�
		for (int i = 0; i < 5; i++)//������
		{
			GotoxyAndPrint(ptank->body[i].X, ptank->body[i].Y, "��");
		}
		setColor(7, 0);
	}
}
void SetTankShape(PTANK ptank)
{
	if (ptank->isAlive == false) return;
	if (ptank->dir == UP)
	{
		ptank->body[0] = { ptank->core.X, ptank->core.Y - 1 };
		ptank->body[1] = { ptank->core.X - 1, ptank->core.Y };
		ptank->body[2] = { ptank->core.X + 1, ptank->core.Y };
		ptank->body[3] = { ptank->core.X - 1, ptank->core.Y + 1 };
		ptank->body[4] = { ptank->core.X + 1, ptank->core.Y + 1 };
	}
	else if (ptank->dir == DOWN)
	{
		ptank->body[0] = { ptank->core.X, ptank->core.Y + 1 };
		ptank->body[1] = { ptank->core.X - 1, ptank->core.Y };
		ptank->body[2] = { ptank->core.X + 1, ptank->core.Y };
		ptank->body[3] = { ptank->core.X - 1, ptank->core.Y - 1 };
		ptank->body[4] = { ptank->core.X + 1, ptank->core.Y - 1 };
	}
	else if (ptank->dir == LEFT)
	{
		ptank->body[0] = { ptank->core.X - 1, ptank->core.Y };
		ptank->body[1] = { ptank->core.X , ptank->core.Y + 1 };
		ptank->body[2] = { ptank->core.X , ptank->core.Y - 1 };
		ptank->body[3] = { ptank->core.X + 1, ptank->core.Y + 1 };
		ptank->body[4] = { ptank->core.X + 1, ptank->core.Y - 1 };
	}
	else if (ptank->dir == RIGHT)
	{
		ptank->body[0] = { ptank->core.X + 1, ptank->core.Y };
		ptank->body[1] = { ptank->core.X , ptank->core.Y + 1 };
		ptank->body[2] = { ptank->core.X , ptank->core.Y - 1 };
		ptank->body[3] = { ptank->core.X - 1, ptank->core.Y + 1 };
		ptank->body[4] = { ptank->core.X - 1, ptank->core.Y - 1 };
	}
}

void ManipulateTank(PTANK ptank, int who, PTANK penemytank)
{
	if (ptank->isAlive == false) return;
	if (who == �ҷ�̹��)
	{
		char ch = 0;
		if (_kbhit())				//���������� 
		{
			ch = _getch();			//�޻��Խ�������
			switch (ch)
			{
			case 'w':
				if (!IsTankMeetOther(ptank, UP, penemytank))
					ptank->core.Y--;
				ptank->dir = UP;
				break;
			case 's':
				if (!IsTankMeetOther(ptank, DOWN, penemytank))
					ptank->core.Y++;
				ptank->dir = DOWN;
				break;
			case 'a':
				if (!IsTankMeetOther(ptank, LEFT, penemytank))
					ptank->core.X--;
				ptank->dir = LEFT;
				break;
			case 'd':
				if (!IsTankMeetOther(ptank, RIGHT, penemytank))
					ptank->core.X++;
				ptank->dir = RIGHT;
				break;
			case ' ':
				if(ptank->bullet.state != �Ѹ�ֵ)
					ptank->bullet.state = δ��ֵ;//�Ѹ�ֵ������ʱ���ٿ��𣬲��ɸ�ֵΪ1��Ӧ����ʧΪ0ʱ����������Ч
				break;
			case 'q':
			{
				//��ͣ���ָ�
				//mciSendString("pause bgm", NULL, 0, NULL);	//��ͣbgm
				setColor(12, 0);
				GotoxyAndPrint(MAP_X / 2 - 14, 1, "       ");//�Ȱѽϳ���running���
				GotoxyAndPrint(MAP_X / 2 - 14, 1, "PAUSE");
				GotoxyAndPrint(MAP_X / 2 - 14, 2, "1. �ص���Ϸ");
				GotoxyAndPrint(MAP_X / 2 - 14, 3, "2. �˳���Ϸ");
				char tmp;
				do
				{
					tmp = _getch();	//��������������ͣ��Ϸ
				} while (!(tmp == '1' || tmp == '2' || tmp == '3'));//ֻ������123�ſ�

				switch (tmp)
				{
				case '1'://�ָ���Ϸ
				{
					//mciSendString("resume bgm", NULL, 0, NULL);//�ָ�bgm
					GotoxyAndPrint(MAP_X / 2 - 14, 1, "RUNNING");
					GotoxyAndPrint(MAP_X / 2 - 14, 2, "Q: ��ͣ��Ϸ");
					GotoxyAndPrint(MAP_X / 2 - 14, 3, "           ");
					GotoxyAndPrint(MAP_X / 2 - 14, 4, "           ");
					break;
				}
				case '2'://�˳���Ϸ
				{
					GameOver(penemytank);
					g_isRunning = false;
					break;
				}
				default:
					break;
				}
				break;
			}
			default:
				break;
			}
		}
	}

	SetTankShape(ptank);//ÿ���ƶ���Ҫ����������̬
}
void ManipulateTank2(PTANK ptank, int who, PTANK pmytank, PTANK penemytank)
{
	if (ptank->isAlive == false) return;
	if (who == �з�̹��)
	{
		switch (rand() % 5)
		{
		case UP:
			if (!IsTankMeetOther2(ptank, UP, pmytank, penemytank))
				ptank->core.Y--;
			ptank->dir = UP;
			break;
		case DOWN:
			if (!IsTankMeetOther2(ptank, DOWN, pmytank, penemytank))
				ptank->core.Y++;
			ptank->dir = DOWN;
			break;
		case LEFT:
			if (!IsTankMeetOther2(ptank, LEFT, pmytank, penemytank))
				ptank->core.X--;
			ptank->dir = LEFT;
			break;
		case RIGHT:
			if (!IsTankMeetOther2(ptank, RIGHT, pmytank, penemytank))
				ptank->core.X++;
			ptank->dir = RIGHT;
			break;
		case 4:
			if (ptank->bullet.state != �Ѹ�ֵ)
				ptank->bullet.state = δ��ֵ;
			break;
		default:
			break;
		}
	}
	SetTankShape(ptank);//ÿ���ƶ���Ҫ����������̬
}
bool IsTankMeetOther(PTANK ptank,int dir, PTANK penemytank)
{
	switch (dir)
	{
	case UP:
		//�Ƿ�ײ�߽�
		if (ptank->core.Y <= 2)
		{
			return true;
		}
		//�Ƿ�ײ�ϰ���
		if (g_MAP[ptank->core.X][ptank->core.Y - 2] == �ϰ���||
			g_MAP[ptank->core.X-1][ptank->core.Y - 2] == �ϰ��� ||
			g_MAP[ptank->core.X+1][ptank->core.Y - 2] == �ϰ���)
		{
			return true;
		}
		//�Ƿ�ײ�з�̹��
		for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
		{
			if(penemytank[i].isAlive ==false) continue;
			if (
				((ptank->core.X == penemytank[i].core.X) && (ptank->core.Y - penemytank[i].core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X - 1) && (ptank->core.Y - penemytank[i].core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X - 2) && (ptank->core.Y - penemytank[i].core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X + 1) && (ptank->core.Y - penemytank[i].core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X + 2) && (ptank->core.Y - penemytank[i].core.Y == 3))
				)//Ҫ==3,����<=��ֻ���ڰ��ŵ�ʱ��ɱ��������С�ڣ���û���ŵ�̹1��������ȴС��2����2����
			{
				return true;
			}
		}
		
		break;
	case DOWN:
		//�Ƿ�ײ�߽�
		if (ptank->core.Y >= MAP_Y - 3)
		{
			return true;
		}
		//�Ƿ�ײ�ϰ���
		if (g_MAP[ptank->core.X][ptank->core.Y + 2] == �ϰ��� ||
			g_MAP[ptank->core.X - 1][ptank->core.Y + 2] == �ϰ��� ||
			g_MAP[ptank->core.X + 1][ptank->core.Y + 2] == �ϰ���)
		{
			return true;
		}
		//�Ƿ�ײ�з�̹��
		for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
		{
			if (penemytank[i].isAlive == false) continue;
			if (
				((ptank->core.X == penemytank[i].core.X) && (penemytank[i].core.Y - ptank->core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X - 1) && (penemytank[i].core.Y - ptank->core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X - 2) && (penemytank[i].core.Y - ptank->core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X + 1) && (penemytank[i].core.Y - ptank->core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X + 2) && (penemytank[i].core.Y - ptank->core.Y == 3))
				)
			{
				return true;
			}
		}
		break;
	case LEFT:
		//�Ƿ�ײ�߽�
		if (ptank->core.X <= 2)
		{
			return true;
		}
		//�Ƿ�ײ�ϰ���
		if (g_MAP[ptank->core.X-2][ptank->core.Y] == �ϰ��� ||
			g_MAP[ptank->core.X - 2][ptank->core.Y -1] == �ϰ��� ||
			g_MAP[ptank->core.X -2][ptank->core.Y + 1] == �ϰ���)
		{
			return true;
		}
		//�Ƿ�ײ�з�̹��
		for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
		{
			if (penemytank[i].isAlive == false) continue;
			if (
				((ptank->core.Y == penemytank[i].core.Y) && (ptank->core.X - penemytank[i].core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y - 1) && (ptank->core.X - penemytank[i].core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y - 2) && (ptank->core.X - penemytank[i].core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y + 1) && (ptank->core.X - penemytank[i].core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y + 2) && (ptank->core.X - penemytank[i].core.X == 3))
				)
			{
				return true;
			}
		}
		break;
	case RIGHT:
		//�Ƿ�ײ�߽�
		if (ptank->core.X >= MAP_X_WALL / 2 - 2)
		{
			return true;
		}
		//�Ƿ�ײ�ϰ���
		if (g_MAP[ptank->core.X + 2][ptank->core.Y] == �ϰ��� ||
			g_MAP[ptank->core.X + 2][ptank->core.Y - 1] == �ϰ��� ||
			g_MAP[ptank->core.X + 2][ptank->core.Y + 1] == �ϰ���)
		{
			return true;
		}
		//�Ƿ�ײ�з�̹��
		for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
		{
			if (penemytank[i].isAlive == false) continue;
			if (
				((ptank->core.Y == penemytank[i].core.Y) && (penemytank[i].core.X - ptank->core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y - 1) && (penemytank[i].core.X - ptank->core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y - 2) && (penemytank[i].core.X - ptank->core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y + 1) && (penemytank[i].core.X - ptank->core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y + 2) && (penemytank[i].core.X - ptank->core.X == 3))
				)
			{
				return true;
			}
		}
		break;
	default:
		break;
	}
	return false;
}
bool IsTankMeetOther2(PTANK ptank, int dir,  PTANK pmytank, PTANK penemytank)
{
	switch (dir)
	{
	case UP:
		//�Ƿ�ײ�߽�
		if (ptank->core.Y <= 2)
		{
			return true;
		}
		//�Ƿ�ײ�ϰ���
		if (g_MAP[ptank->core.X][ptank->core.Y - 2] == �ϰ��� ||
			g_MAP[ptank->core.X - 1][ptank->core.Y - 2] == �ϰ��� ||
			g_MAP[ptank->core.X + 1][ptank->core.Y - 2] == �ϰ���)
		{
			return true;
		}
		//�Ƿ�ײ�ҷ�̹��
		if (
			((ptank->core.X == pmytank->core.X - 0) && (ptank->core.Y - pmytank->core.Y == 3)) ||
			((ptank->core.X == pmytank->core.X - 1) && (ptank->core.Y - pmytank->core.Y == 3)) ||
			((ptank->core.X == pmytank->core.X - 2) && (ptank->core.Y - pmytank->core.Y == 3)) ||
			((ptank->core.X == pmytank->core.X + 1) && (ptank->core.Y - pmytank->core.Y == 3)) ||
			((ptank->core.X == pmytank->core.X + 2) && (ptank->core.Y - pmytank->core.Y == 3))
			)//Ҫ==3,����<=��ֻ���ڰ��ŵ�ʱ��ɱ��������С�ڣ���û���ŵ�̹1��������ȴС��2����2����
		{
			return true;
		}
		//�Ƿ�ײ�����з�̹��
		for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
		{
			if (ptank->core.X == penemytank[i].core.X && ptank->core.Y == penemytank[i].core.Y)//�ų��Լ�
				continue;
			if (
				((ptank->core.X == penemytank[i].core.X) && (ptank->core.Y - penemytank[i].core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X - 1) && (ptank->core.Y - penemytank[i].core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X - 2) && (ptank->core.Y - penemytank[i].core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X + 1) && (ptank->core.Y - penemytank[i].core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X + 2) && (ptank->core.Y - penemytank[i].core.Y == 3))
				)//Ҫ==3,����<=��ֻ���ڰ��ŵ�ʱ��ɱ��������С�ڣ���û���ŵ�̹1��������ȴС��2����2����
			{
				return true;
			}
		}

		break;
	case DOWN:
		//�Ƿ�ײ�߽�
		if (ptank->core.Y >= MAP_Y - 3)
		{
			return true;
		}
		//�Ƿ�ײ�ϰ���
		if (g_MAP[ptank->core.X][ptank->core.Y + 2] == �ϰ��� ||
			g_MAP[ptank->core.X - 1][ptank->core.Y + 2] == �ϰ��� ||
			g_MAP[ptank->core.X + 1][ptank->core.Y + 2] == �ϰ���)
		{
			return true;
		}
		//�Ƿ�ײ�ҷ�̹��
		if (
			((ptank->core.X == pmytank->core.X - 0) && (pmytank->core.Y - ptank->core.Y == 3)) ||
			((ptank->core.X == pmytank->core.X - 1) && (pmytank->core.Y - ptank->core.Y == 3)) ||
			((ptank->core.X == pmytank->core.X - 2) && (pmytank->core.Y - ptank->core.Y == 3)) ||
			((ptank->core.X == pmytank->core.X + 1) && (pmytank->core.Y - ptank->core.Y == 3)) ||
			((ptank->core.X == pmytank->core.X + 2) && (pmytank->core.Y - ptank->core.Y == 3))
			)
		{
			return true;
		}
		//�Ƿ�ײ�����з�̹��
		for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
		{
			if (ptank->core.X == penemytank[i].core.X && ptank->core.Y == penemytank[i].core.Y)//�ų��Լ�
				continue;
			if (
				((ptank->core.X == penemytank[i].core.X) && (penemytank[i].core.Y - ptank->core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X - 1) && (penemytank[i].core.Y - ptank->core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X - 2) && (penemytank[i].core.Y - ptank->core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X + 1) && (penemytank[i].core.Y - ptank->core.Y == 3)) ||
				((ptank->core.X == penemytank[i].core.X + 2) && (penemytank[i].core.Y - ptank->core.Y == 3))
				)
			{
				return true;
			}
		}
		break;
	case LEFT:
		//�Ƿ�ײ�߽�
		if (ptank->core.X <= 2)
		{
			return true;
		}
		//�Ƿ�ײ�ϰ���
		if (g_MAP[ptank->core.X - 2][ptank->core.Y] == �ϰ��� ||
			g_MAP[ptank->core.X - 2][ptank->core.Y - 1] == �ϰ��� ||
			g_MAP[ptank->core.X - 2][ptank->core.Y + 1] == �ϰ���)
		{
			return true;
		}
		//�Ƿ�ײ�ҷ�̹��
		if (
			((ptank->core.Y == pmytank->core.Y - 0) && (ptank->core.X - pmytank->core.X == 3)) ||
			((ptank->core.Y == pmytank->core.Y - 1) && (ptank->core.X - pmytank->core.X == 3)) ||
			((ptank->core.Y == pmytank->core.Y - 2) && (ptank->core.X - pmytank->core.X == 3)) ||
			((ptank->core.Y == pmytank->core.Y + 1) && (ptank->core.X - pmytank->core.X == 3)) ||
			((ptank->core.Y == pmytank->core.Y + 2) && (ptank->core.X - pmytank->core.X == 3))
			)
		{
			return true;
		}
		//�Ƿ�ײ�����з�̹��
		for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
		{
			if (ptank->core.X == penemytank[i].core.X && ptank->core.Y == penemytank[i].core.Y)//�ų��Լ�
				continue;
			if (
				((ptank->core.Y == penemytank[i].core.Y) && (ptank->core.X - penemytank[i].core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y - 1) && (ptank->core.X - penemytank[i].core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y - 2) && (ptank->core.X - penemytank[i].core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y + 1) && (ptank->core.X - penemytank[i].core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y + 2) && (ptank->core.X - penemytank[i].core.X == 3))
				)
			{
				return true;
			}
		}
		break;
	case RIGHT:
		//�Ƿ�ײ�߽�
		if (ptank->core.X >= MAP_X_WALL / 2 - 2)
		{
			return true;
		}
		//�Ƿ�ײ�ϰ���
		if (g_MAP[ptank->core.X + 2][ptank->core.Y] == �ϰ��� ||
			g_MAP[ptank->core.X + 2][ptank->core.Y - 1] == �ϰ��� ||
			g_MAP[ptank->core.X + 2][ptank->core.Y + 1] == �ϰ���)
		{
			return true;
		}
		//�Ƿ�ײ�ҷ�̹��
		if (
			((ptank->core.Y == pmytank->core.Y - 0) && (pmytank->core.X - ptank->core.X == 3)) ||
			((ptank->core.Y == pmytank->core.Y - 1) && (pmytank->core.X - ptank->core.X == 3)) ||
			((ptank->core.Y == pmytank->core.Y - 2) && (pmytank->core.X - ptank->core.X == 3)) ||
			((ptank->core.Y == pmytank->core.Y + 1) && (pmytank->core.X - ptank->core.X == 3)) ||
			((ptank->core.Y == pmytank->core.Y + 2) && (pmytank->core.X - ptank->core.X == 3))
			)
		{
			return true;
		}
		//�Ƿ�ײ�����з�̹��
		for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
		{
			if (ptank->core.X == penemytank[i].core.X && ptank->core.Y == penemytank[i].core.Y)//�ų��Լ�
				continue;
			if (
				((ptank->core.Y == penemytank[i].core.Y) && (penemytank[i].core.X - ptank->core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y - 1) && (penemytank[i].core.X - ptank->core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y - 2) && (penemytank[i].core.X - ptank->core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y + 1) && (penemytank[i].core.X - ptank->core.X == 3)) ||
				((ptank->core.Y == penemytank[i].core.Y + 2) && (penemytank[i].core.X - ptank->core.X == 3))
				)
			{
				return true;
			}
		}
		break;
	default:
		break;
	}
	return false;
}


//�ӵ����
void MoveBullet(PBULLET pbullet)
{
	switch (pbullet->dir)
	{
	case UP:
		pbullet->core.Y--;
		break;
	case DOWN:
		pbullet->core.Y++;
		break;
	case LEFT:
		pbullet->core.X--;
		break;
	case RIGHT:
		pbullet->core.X++;
		break;
	default:
		break;
	}


}
void CleanBullet(COORD oldBulCore)
{
	GotoxyAndPrint(oldBulCore.X, oldBulCore.Y, "  ");
	//printf("  ");
}
void DrawBullet(PBULLET pbullet)
{
	//�����߽磬���ɱ߽����ɫ��ʵ���ӵ���ʧ��Ч��
	if (pbullet->core.X <= 0 ||
		pbullet->core.X >= MAP_X_WALL / 2 ||
		pbullet->core.Y <= 0 ||
		pbullet->core.Y >= MAP_Y - 1)
	{
		setColor(7, 0);
	}
	else
	{
		setColor(12, 0);
	}

	//�����ϰ������ӵ���Ϊ�ո�ʵ���ӵ���ʧ
	if (g_MAP[pbullet->core.X][pbullet->core.Y] == �ϰ���)
	{
		GotoxyAndPrint(pbullet->core.X, pbullet->core.Y, "  ");
	}
	else
	{
		GotoxyAndPrint(pbullet->core.X, pbullet->core.Y, "��");
	}	
	setColor(7, 0);
}

void IsBulMeetOther(PBULLET pbullet, PTANK penemytank)
{
	//�Ƿ������߽�
	if (pbullet->core.X <= 0 ||
		pbullet->core.X >= MAP_X_WALL / 2 ||
		pbullet->core.Y <= 0 ||
		pbullet->core.Y >= MAP_Y - 1)
	{
		pbullet->state = ������;
	}
	//�Ƿ������ϰ���
	if (g_MAP[pbullet->core.X][pbullet->core.Y] == �ϰ���)
	{
		pbullet->state = ������;
		g_MAP[pbullet->core.X][pbullet->core.Y] = �յ�;
	}
	//�Ƿ����з�̹��
	for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
	{
		if (penemytank[i].isAlive == false) continue;
		switch (pbullet->dir)
		{
		case UP:
			if (
				(pbullet->core.X == penemytank[i].core.X) && (pbullet->core.Y - penemytank[i].core.Y == 0)||
				(pbullet->core.X == penemytank[i].body[0].X) && (pbullet->core.Y - penemytank[i].body[0].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[1].X) && (pbullet->core.Y - penemytank[i].body[1].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[2].X) && (pbullet->core.Y - penemytank[i].body[2].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[3].X) && (pbullet->core.Y - penemytank[i].body[3].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[4].X) && (pbullet->core.Y - penemytank[i].body[4].Y == 0)
				)
			{
				pbullet->state = ������;
				penemytank[i].blood--;
				if (penemytank[i].blood == 0)//�����Ѫ��Ϊ0
					penemytank[i].isAlive = false;//����Ϊ����
			}
			break;
		case DOWN:
			if (
				(pbullet->core.X == penemytank[i].core.X) && (pbullet->core.Y - penemytank[i].core.Y == 0) ||
				(pbullet->core.X == penemytank[i].body[0].X) && (pbullet->core.Y - penemytank[i].body[0].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[1].X) && (pbullet->core.Y - penemytank[i].body[1].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[2].X) && (pbullet->core.Y - penemytank[i].body[2].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[3].X) && (pbullet->core.Y - penemytank[i].body[3].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[4].X) && (pbullet->core.Y - penemytank[i].body[4].Y == 0)
				)
			{
				pbullet->state = ������;
				penemytank[i].blood--;
				if (penemytank[i].blood == 0)//�����Ѫ��Ϊ0
					penemytank[i].isAlive = false;//����Ϊ����
			}
			break;
		case LEFT:
			if (
				(pbullet->core.X == penemytank[i].core.X) && (pbullet->core.Y - penemytank[i].core.Y == 0) ||
				(pbullet->core.X == penemytank[i].body[0].X) && (pbullet->core.Y - penemytank[i].body[0].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[1].X) && (pbullet->core.Y - penemytank[i].body[1].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[2].X) && (pbullet->core.Y - penemytank[i].body[2].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[3].X) && (pbullet->core.Y - penemytank[i].body[3].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[4].X) && (pbullet->core.Y - penemytank[i].body[4].Y == 0)
				)
			{
				pbullet->state = ������;
				penemytank[i].blood--;
				if (penemytank[i].blood == 0)//�����Ѫ��Ϊ0
					penemytank[i].isAlive = false;//����Ϊ����
			}
			break;
		case RIGHT:
			if (
				(pbullet->core.X == penemytank[i].core.X) && (pbullet->core.Y - penemytank[i].core.Y == 0) ||
				(pbullet->core.X == penemytank[i].body[0].X) && (pbullet->core.Y - penemytank[i].body[0].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[1].X) && (pbullet->core.Y - penemytank[i].body[1].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[2].X) && (pbullet->core.Y - penemytank[i].body[2].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[3].X) && (pbullet->core.Y - penemytank[i].body[3].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[4].X) && (pbullet->core.Y - penemytank[i].body[4].Y == 0)
				)
			{
				pbullet->state = ������;
				penemytank[i].blood--;
				if (penemytank[i].blood == 0)//�����Ѫ��Ϊ0
					penemytank[i].isAlive = false;//����Ϊ����
			}
			break;
		default:
			break;
		}
	}
}
void IsBulMeetOther2(PBULLET pbullet, PTANK penemytank, PTANK ptank)
{
	//�Ƿ������߽�
	if (pbullet->core.X <= 0 ||
		pbullet->core.X >= MAP_X_WALL / 2 ||
		pbullet->core.Y <= 0 ||
		pbullet->core.Y >= MAP_Y - 1)
	{
		pbullet->state = ������;
	}
	//�Ƿ������ϰ���
	if (g_MAP[pbullet->core.X][pbullet->core.Y] == �ϰ���)
	{
		pbullet->state = ������;
		g_MAP[pbullet->core.X][pbullet->core.Y] = �յ�;
	}
	//�Ƿ������ҷ�̹��
	switch (pbullet->dir)
	{
	case UP:
		if (
			(pbullet->core.X == ptank->core.X) && (pbullet->core.Y - ptank->core.Y == 0) ||
			(pbullet->core.X == ptank->body[0].X) && (pbullet->core.Y - ptank->body[0].Y == 0) ||
			(pbullet->core.X == ptank->body[1].X) && (pbullet->core.Y - ptank->body[1].Y == 0) ||
			(pbullet->core.X == ptank->body[2].X) && (pbullet->core.Y - ptank->body[2].Y == 0) ||
			(pbullet->core.X == ptank->body[3].X) && (pbullet->core.Y - ptank->body[3].Y == 0) ||
			(pbullet->core.X == ptank->body[4].X) && (pbullet->core.Y - ptank->body[4].Y == 0)
			)
		{
			pbullet->state = ������;
			(ptank->blood)--;
			if (ptank->blood == 0)//�����Ѫ��Ϊ0
				ptank->isAlive = false;//����Ϊ����
		}
		break;
	case DOWN:
		if (
			(pbullet->core.X == ptank->core.X) && (pbullet->core.Y - ptank->core.Y == 0) ||
			(pbullet->core.X == ptank->body[0].X) && (pbullet->core.Y - ptank->body[0].Y == 0) ||
			(pbullet->core.X == ptank->body[1].X) && (pbullet->core.Y - ptank->body[1].Y == 0) ||
			(pbullet->core.X == ptank->body[2].X) && (pbullet->core.Y - ptank->body[2].Y == 0) ||
			(pbullet->core.X == ptank->body[3].X) && (pbullet->core.Y - ptank->body[3].Y == 0) ||
			(pbullet->core.X == ptank->body[4].X) && (pbullet->core.Y - ptank->body[4].Y == 0)
			)
		{
			pbullet->state = ������;
			(ptank->blood)--;
			if (ptank->blood == 0)//�����Ѫ��Ϊ0
				ptank->isAlive = false;//����Ϊ����
		}
		break;
	case LEFT:
		if (
			(pbullet->core.X == ptank->core.X) && (pbullet->core.Y - ptank->core.Y == 0) ||
			(pbullet->core.X == ptank->body[0].X) && (pbullet->core.Y - ptank->body[0].Y == 0) ||
			(pbullet->core.X == ptank->body[1].X) && (pbullet->core.Y - ptank->body[1].Y == 0) ||
			(pbullet->core.X == ptank->body[2].X) && (pbullet->core.Y - ptank->body[2].Y == 0) ||
			(pbullet->core.X == ptank->body[3].X) && (pbullet->core.Y - ptank->body[3].Y == 0) ||
			(pbullet->core.X == ptank->body[4].X) && (pbullet->core.Y - ptank->body[4].Y == 0)
			)
		{
			pbullet->state = ������;
			(ptank->blood)--;
			if (ptank->blood == 0)//�����Ѫ��Ϊ0
				ptank->isAlive = false;//����Ϊ����
		}
		break;
	case RIGHT:
		if (
			(pbullet->core.X == ptank->core.X) && (pbullet->core.Y - ptank->core.Y == 0) ||
			(pbullet->core.X == ptank->body[0].X) && (pbullet->core.Y - ptank->body[0].Y == 0) ||
			(pbullet->core.X == ptank->body[1].X) && (pbullet->core.Y - ptank->body[1].Y == 0) ||
			(pbullet->core.X == ptank->body[2].X) && (pbullet->core.Y - ptank->body[2].Y == 0) ||
			(pbullet->core.X == ptank->body[3].X) && (pbullet->core.Y - ptank->body[3].Y == 0) ||
			(pbullet->core.X == ptank->body[4].X) && (pbullet->core.Y - ptank->body[4].Y == 0)
			)
		{
			pbullet->state = ������;
			(ptank->blood)--;
			if (ptank->blood == 0)//�����Ѫ��Ϊ0
				ptank->isAlive = false;//����Ϊ����
		}
		break;
	default:
		break;
	}
	//�Ƿ��������з�̹��
	for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
	{
		if (penemytank[i].isAlive == false) continue;
		switch (pbullet->dir)
		{
		case UP:
			if (
				(pbullet->core.X == penemytank[i].core.X) && (pbullet->core.Y - penemytank[i].core.Y == 0) ||
				(pbullet->core.X == penemytank[i].body[0].X) && (pbullet->core.Y - penemytank[i].body[0].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[1].X) && (pbullet->core.Y - penemytank[i].body[1].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[2].X) && (pbullet->core.Y - penemytank[i].body[2].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[3].X) && (pbullet->core.Y - penemytank[i].body[3].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[4].X) && (pbullet->core.Y - penemytank[i].body[4].Y == 0)
				)
			{
				pbullet->state = ������;
				//penemytank[i].isAlive = false;//����Ϊ����
			}
			break;
		case DOWN:
			if (
				(pbullet->core.X == penemytank[i].core.X) && (pbullet->core.Y - penemytank[i].core.Y == 0) ||
				(pbullet->core.X == penemytank[i].body[0].X) && (pbullet->core.Y - penemytank[i].body[0].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[1].X) && (pbullet->core.Y - penemytank[i].body[1].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[2].X) && (pbullet->core.Y - penemytank[i].body[2].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[3].X) && (pbullet->core.Y - penemytank[i].body[3].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[4].X) && (pbullet->core.Y - penemytank[i].body[4].Y == 0)
				)
			{
				pbullet->state = ������;
				//penemytank[i].isAlive = false;
			}
			break;
		case LEFT:
			if (
				(pbullet->core.X == penemytank[i].core.X) && (pbullet->core.Y - penemytank[i].core.Y == 0) ||
				(pbullet->core.X == penemytank[i].body[0].X) && (pbullet->core.Y - penemytank[i].body[0].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[1].X) && (pbullet->core.Y - penemytank[i].body[1].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[2].X) && (pbullet->core.Y - penemytank[i].body[2].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[3].X) && (pbullet->core.Y - penemytank[i].body[3].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[4].X) && (pbullet->core.Y - penemytank[i].body[4].Y == 0)
				)
			{
				pbullet->state = ������;
				//penemytank[i].isAlive = false;
			}
			break;
		case RIGHT:
			if (
				(pbullet->core.X == penemytank[i].core.X) && (pbullet->core.Y - penemytank[i].core.Y == 0) ||
				(pbullet->core.X == penemytank[i].body[0].X) && (pbullet->core.Y - penemytank[i].body[0].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[1].X) && (pbullet->core.Y - penemytank[i].body[1].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[2].X) && (pbullet->core.Y - penemytank[i].body[2].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[3].X) && (pbullet->core.Y - penemytank[i].body[3].Y == 0) ||
				(pbullet->core.X == penemytank[i].body[4].X) && (pbullet->core.Y - penemytank[i].body[4].Y == 0)
				)
			{
				pbullet->state = ������;
				//penemytank[i].isAlive = false;
			}
			break;
		default:
			break;
		}
	}
}

//�ϰ������
void BarrierInit()
{
	for (int x = 0; x < MAP_X_WALL; x++)
	{
		for (int y = 0; y < MAP_Y; y++)
		{
			if (
				(x > MAP_X_WALL / 4 - 9 &&x < MAP_X_WALL / 4 - 5 &&y > MAP_Y / 2 - 9 &&y < MAP_Y/2 - 4) ||
				(x < MAP_X_WALL / 4 + 9 &&x > MAP_X_WALL / 4 + 5 && y > MAP_Y / 2 - 9 && y < MAP_Y / 2 - 4)||
				(x > MAP_X_WALL / 4 - 9 && x < MAP_X_WALL / 4 - 5 && y < MAP_Y / 2 + 9 && y > MAP_Y / 2 + 4)||
				(x < MAP_X_WALL / 4 + 9 && x > MAP_X_WALL / 4 + 5 && y < MAP_Y / 2 + 9 && y > MAP_Y / 2 + 4)
				)
			{
				g_MAP[x][y] = �ϰ���;
			}

		}
	}
}
void DrawBarr()
{
	for (int x = 0; x < MAP_X_WALL; x++)
	{
		for (int y = 0; y < MAP_Y; y++)
		{
			if(g_MAP[x][y] == �ϰ���)
			{
				GotoxyAndPrint(x, y, "��");
			}
		}
	}

}

