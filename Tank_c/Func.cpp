#include "Func.h"
#include "Data.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

//ϵͳ����
void Gotoxy(unsigned x, unsigned y)
{
	COORD cur;
	cur.X = x * 2;
	cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
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
				y == 0 || y == MAP_Y - 1 ||						//�����
				(x > MAP_X_WALL && y == MAP_Y / 2))				//������Ϣ����Ϸ��Ϣ�ָ���
			{
				g_MAP[x][y] = 1;
			}
		}
	}

	//���ع��
	SetCursorState(false);

	//���ñ���
	system("title Tank");
}
void DrawMapBorder()
{
	system("cls");						//��ҳ������
	for (int x = 0; x < MAP_X; x++)
	{
		for (int y = 0; y < MAP_Y; y++)
		{
			if (g_MAP[x][y] == 1)
			{
				Gotoxy(x, y);
				//printf("��");		//ռ2�ַ�
				printf("��");
			}
		}
	}
}
void DrawWelcome()
{
	//��ӡͼ��
	Gotoxy(MAP_X / 2 - 25, MAP_Y / 2-10);
	printf("Welcome!");

	//��ʾ��Ϣ
	Gotoxy(MAP_X / 2 - 25, MAP_Y / 2 - 6);
	printf("1. ����Ϸ\n");
	Gotoxy(MAP_X / 2 - 25, MAP_Y / 2 - 4);
	printf("2. �˳���Ϸ\n");
	Gotoxy(MAP_X / 2 - 25, MAP_Y / 2 - 2);
	printf( "������ѡ��-> ");
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
		Gotoxy(MAP_X / 2 - 25, MAP_Y / 2 + 3);
		printf("Bye! \n");
		break;
	}
	default:
		Gotoxy(MAP_X / 2 - 25, MAP_Y / 2 + 3);
		printf("�������\n");
		break;
	}
}

//̹�����
void ManipulateTank(PTANK ptank)
{
	char ch = 0;
	if (_kbhit())				//���������� 
	{
		ch = _getch();			//�޻��Խ�������

		switch (ch)
		{
		case 'w':
			if (!IsTankMeetOther(ptank, ch))
				ptank->core.Y--;
			ptank->dir = UP;
			break;
		case 's':
			if (!IsTankMeetOther(ptank, ch))
				ptank->core.Y++;
			ptank->dir = DOWN;
			break;
		case 'a':
			if (!IsTankMeetOther(ptank, ch))
				ptank->core.X--;
			ptank->dir = LEFT;
			break;
		case 'd':
			if (!IsTankMeetOther(ptank, ch))
				ptank->core.X++;
			ptank->dir = RIGHT;
			break;
		case ' ':
			g_isBulExist++;
			break;
		default:
			break;
		}
	}
	SetTankShape(ptank);//ÿ���ƶ���Ҫ����������̬
}
void CleanTankTail(COORD oldCore,PCOORD oldBody)
{
	Gotoxy(oldCore.X, oldCore.Y);//���ĵ�
	printf("  ");		//��β��2�ո�
	for (int i = 0; i < 5; i++)//������
	{
		Gotoxy(oldBody[i].X, oldBody[i].Y);
		printf("  ");
	}
}
void DrawTank(PTANK ptank)
{
	setColor(10, 0);
	Gotoxy(ptank->core.X, ptank->core.Y);//���ĵ�
	printf("��");
	for (int i = 0; i < 5; i++)//������
	{
		Gotoxy(ptank->body[i].X, ptank->body[i].Y);
		printf("��");
	}
	setColor(7, 0);
}
void SetTankShape(PTANK ptank)
{
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
bool IsTankMeetOther(PTANK ptank,char dir)
{

	switch (dir)
	{
	case 'w':
	
		//�Ƿ�ײ�߽�
		if (ptank->core.Y <= 2)
		{
			return true;
		}
		//�Ƿ�ײ�ϰ���
		if (g_Bar[ptank->core.X][ptank->core.Y - 2] == 1||
			g_Bar[ptank->core.X-1][ptank->core.Y - 2] == 1||
			g_Bar[ptank->core.X+1][ptank->core.Y - 2] == 1)
		{
			return true;
		}
		break;
	
	case 's':
		//�Ƿ�ײ�߽�
		if (ptank->core.Y >= MAP_Y - 3)
		{
			return true;
		}
		//�Ƿ�ײ�ϰ���
		if (g_Bar[ptank->core.X][ptank->core.Y + 2] == 1 ||
			g_Bar[ptank->core.X - 1][ptank->core.Y + 2] == 1 ||
			g_Bar[ptank->core.X + 1][ptank->core.Y + 2] == 1)
		{
			return true;
		}
		break;
	case 'a':
		//�Ƿ�ײ�߽�
		if (ptank->core.X <= 2)
		{
			return true;
		}
		//�Ƿ�ײ�ϰ���
		if (g_Bar[ptank->core.X-2][ptank->core.Y] == 1 ||
			g_Bar[ptank->core.X - 2][ptank->core.Y -1] == 1 ||
			g_Bar[ptank->core.X -2][ptank->core.Y + 1] == 1)
		{
			return true;
		}
		break;
	case 'd':
		//�Ƿ�ײ�߽�
		if (ptank->core.X >= MAP_X_WALL / 2 - 2)
		{
			return true;
		}
		//�Ƿ�ײ�ϰ���
		if (g_Bar[ptank->core.X + 2][ptank->core.Y] == 1 ||
			g_Bar[ptank->core.X + 2][ptank->core.Y - 1] == 1 ||
			g_Bar[ptank->core.X + 2][ptank->core.Y + 1] == 1)
		{
			return true;
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
	Gotoxy(oldBulCore.X, oldBulCore.Y);
	printf("  ");
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
	Gotoxy(pbullet->core.X, pbullet->core.Y);
	//�����ϰ������ӵ���Ϊ�ո�ʵ���ӵ���ʧ
	if (g_Bar[pbullet->core.X][pbullet->core.Y] == 1)
	{
		printf("  ");
	}
	else
	{
		printf("��");
	}	
	setColor(7, 0);
}
void IsBulMeetOther(PBULLET pbullet)
{
	//�Ƿ������߽�
	if (pbullet->core.X <= 0 ||
		pbullet->core.X >= MAP_X_WALL / 2 ||
		pbullet->core.Y <= 0 ||
		pbullet->core.Y >= MAP_Y - 1)
	{
		g_isBulExist = 0;
	}
	//�Ƿ������ϰ���
	if (g_Bar[pbullet->core.X][pbullet->core.Y] == 1)
	{
		g_isBulExist = 0;
		g_Bar[pbullet->core.X][pbullet->core.Y] = 0;
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
				g_Bar[x][y] = 1;
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
			if(g_Bar[x][y] == 1)
			{
				Gotoxy(x, y);
				printf("��");
			}
		}
	}

}

