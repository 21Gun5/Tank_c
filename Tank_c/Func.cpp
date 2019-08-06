#include "Func.h"
#include "Data.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

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
				(x > MAP_X_WALL && y == MAP_Y / 2))					//������Ϣ����Ϸ��Ϣ�ָ���
			{
				g_MAP[x][y] = ��ͼ�߽�;
			}
		}
	}

	//���ع��
	SetCursorState(false);

	//���ñ���
	system("title Tank");

	//��ʼ�����������
	srand((unsigned int)time(0));
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
		}
	}
}
void DrawWelcome()
{
	//��ӡͼ��
	GotoxyAndPrint(MAP_X / 2 - 25, MAP_Y / 2-10, "Welcome!");
	//printf("Welcome!");

	//��ʾ��Ϣ
	GotoxyAndPrint(MAP_X / 2 - 25, MAP_Y / 2 - 6, "1. ����Ϸ\n");
	//printf("1. ����Ϸ\n");
	GotoxyAndPrint(MAP_X / 2 - 25, MAP_Y / 2 - 4, "2. �˳���Ϸ\n");
	//printf("2. �˳���Ϸ\n");
	GotoxyAndPrint(MAP_X / 2 - 25, MAP_Y / 2 - 2, "������ѡ��-> ");
	//printf( "������ѡ��-> ");
	SetCursorState(true);			//�û�����ʱ�����ɼ�
}
void SelectAction()
{
	
	//int input = _getch() - '0';	//�޻��Խ��գ�-'0'��֤��0-9������ASCII
	int input = 1;
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
		//printf("Bye! \n");
		break;
	}
	default:
		GotoxyAndPrint(MAP_X / 2 - 25, MAP_Y / 2 + 3, "�������\n");
		//printf("�������\n");
		break;
	}
}

//̹�����
void ManipulateTank(PTANK ptank,int who, PTANK penemytank,PBULLET pbullet)
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
				//g_isBulExist++;
				pbullet->state = δ��ֵ;
				break;
			default:
				break;
			}
		}
	}

	SetTankShape(ptank);//ÿ���ƶ���Ҫ����������̬
}
void ManipulateTank2(PTANK ptank, int who,PTANK pmytank, PTANK penemytank)
{
	//if (ptank->isAlive == false) return;
	if (who == �з�̹��)
	{
		switch (rand() % 4)
		{
		case UP:
			if (!IsTankMeetOther2(ptank, UP,  pmytank,  penemytank))
				ptank->core.Y--;
			ptank->dir = UP;
			break;
		case DOWN:
			if (!IsTankMeetOther2(ptank, DOWN,pmytank, penemytank))
				ptank->core.Y++;
			ptank->dir = DOWN;
			break;
		case LEFT:
			if (!IsTankMeetOther2(ptank, LEFT,pmytank, penemytank))
				ptank->core.X--;
			ptank->dir = LEFT;
			break;
		case RIGHT:
			if (!IsTankMeetOther2(ptank, RIGHT, pmytank, penemytank))
				ptank->core.X++;
			ptank->dir = RIGHT;
			break;
		default:
			break;
		}
	}
	SetTankShape(ptank);//ÿ���ƶ���Ҫ����������̬
}
void CleanTankTail(COORD oldCore,PCOORD oldBody)
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
	
	////��λ����Ϣ���浽��ͼ
	//if (who == �ҷ�̹��)
	//{
	//	g_MAP[ptank->core.X][ptank->core.Y] = �ҷ�̹��;
	//	for (int i = 0; i < 5; i++)
	//		g_MAP[ptank->body[i].X][ptank->body[i].Y] = �ҷ�̹��;
	//}
	//else if (who == �з�̹��)
	//{
	//	g_MAP[ptank->core.X][ptank->core.Y] = �з�̹��;
	//	for (int i = 0; i < 5; i++)
	//		g_MAP[ptank->body[i].X][ptank->body[i].Y] = �з�̹��;
	//}

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
		//�Ƿ�ײ�ҷ��ӵ�

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
		//g_isBulExist = 0;
		pbullet->state = ������;
	}
	//�Ƿ������ϰ���
	if (g_MAP[pbullet->core.X][pbullet->core.Y] == �ϰ���)
	{
		//g_isBulExist = 0;
		pbullet->state = ������;
		g_MAP[pbullet->core.X][pbullet->core.Y] = �յ�;
	}
	//�Ƿ������з�̹��
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
				
				//g_isBulExist = 0;
				pbullet->state = ������;
				penemytank[i].isAlive = false;//����Ϊ����

				//penemytank[i].core.X = 9999;//�������٣����ʬ������ڵ�ͼ������赲
				//penemytank[i].core.Y = 9999;//�������٣����ʬ������ڵ�ͼ������赲
				//for (int j = 0; j < 5; j++)
				//{
				//	penemytank[i].body[j].X = 9999;
				//	penemytank[i].body[j].Y = 9999;
				//}
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
				//g_isBulExist = 0;
				pbullet->state = ������;
				penemytank[i].isAlive = false;
				//penemytank[i].core = { (-100,-100) };//�������٣����ʬ������ڵ�ͼ������赲
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
				//g_isBulExist = 0;
				pbullet->state = ������;
				penemytank[i].isAlive = false;
				//penemytank[i].core = { (-100,-100) };//�������٣����ʬ������ڵ�ͼ������赲
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
				//g_isBulExist = 0;
				pbullet->state = ������;
				penemytank[i].isAlive = false;
				//penemytank[i].core = { (-100,-100) };//�������٣����ʬ������ڵ�ͼ������赲
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

