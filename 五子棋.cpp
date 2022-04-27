#include<easyx.h>//导入图形界面操作的库函数
#include<windows.h>
#include<graphics.h>
#include<conio.h>
#include<mmsystem.h>
#include<cmath>
#include<string>
#include<iostream>
#pragma comment(lib,"winmm.lib")


#define space 35

#define mode 15

#define A 4

int row;
int column;
int parity;

int map[mode][mode];
enum chessState { None, Black, White };
//加载窗口
void Load_window()
{
	//创建窗口
	initgraph(640, 640);
	IMAGE image;
	loadimage(&image, "./图片.jpg");
	putimage(0, 0, &image);
}

//播放音乐
void BGM()
{
	mciSendString("open ./音乐.mp3 alias music", 0, 0, 0);
	mciSendString("setaudio backmusic volume to 300", 0, 0, 0);


	mciSendString("play music repeat", 0, 0, 0);

}

//绘制横线
void draw_row()
{
	setlinecolor(BLACK);
	for (int i = 0; i < mode; i++)
	{
		setlinestyle(PS_SOLID, 1);
		if (i == 0 || i == mode - 1)
			setlinestyle(PS_SOLID, 2);
		line(2 * space, (2 + i) * space, (mode - 1) * space + 2 * space, (2 + i) * space);

	}
}

//绘制竖线
void draw_column()
{
	setlinecolor(BLACK);
	for (int i = 0; i < mode; i++)
	{
		setlinestyle(PS_SOLID, 1);
		if (i == 0 || i == mode - 1)
			setlinestyle(PS_SOLID, 2);
		line((2 + i) * space, 2 * space, (2 + i) * space, 2 * space + (mode - 1) * space);
	}
}

//绘制五个点
void draw_spot()
{
	setfillcolor(BLACK);
	fillcircle((mode / 2 + 2) * space, (mode / 2 + 2) * space, 3);
	fillcircle((mode / 2 + 2 - A) * space, (mode / 2 + 2 - A) * space, 3);
	fillcircle((mode / 2 + 2 - A) * space, (mode / 2 + 2 + A) * space, 3);
	fillcircle((mode / 2 + 2 + A) * space, (mode / 2 + 2 - A) * space, 3);
	fillcircle((mode / 2 + 2 + A) * space, (mode / 2 + 2 + A) * space, 3);
}

void chessBoard()
{
	draw_column();
	draw_row();
	draw_spot();

}

//记录当前操作的数据结构
typedef struct
{
	int x;
	int y;
	bool showyn;
}Position;

Position position;


//鼠标操作
void mouseOperation()
{
	//定义一个消息结构体变量
	ExMessage msg;
	//获取鼠标消息
	if (peekmessage(&msg, EM_MOUSE))
	{
		//坐标校准

		position.showyn = false;
		for (int i = 0; i < mode; i++)
		{
			for (int j = 0; j < mode; j++)
			{
				int lattice_x = (i + 2) * space;
				int lattice_y = (j + 2) * space;
				if (abs(msg.x - lattice_x) < space / 2.0 && abs(msg.y - lattice_y) < space / 2.0)
				{
					position.x = lattice_x;
					position.y = lattice_y;
					position.showyn = true;
					row = i;
					column = j;
				}
			}
		}

		//下棋
		if (msg.message == WM_LBUTTONDOWN && map[row][column] == 0)
		{
			parity++;
			if (parity % 2)
				map[row][column] = 1;
			else
				map[row][column] = 2;
		}

	}

}

//显示鼠标移动
void drawMouseMove()
{
	if (position.showyn)
	{
		if (parity % 2)
			setfillcolor(WHITE);
		else
			setfillcolor(BLACK);
		fillcircle(position.x, position.y, 10);
		//rectangle(position.x-space/2,position.y-space/2,position.x+space/2,position.y+space/2);
	}
	for (int i = 0; i < mode; i++)
	{
		for (int j = 0; j < mode; j++)
		{
			switch (map[i][j])
			{
			case None:
				break;
			case Black:
				setfillcolor(BLACK);
				fillcircle((i + 2) * space, (j + 2) * space, 10);
				break;
			case White:
				setfillcolor(WHITE);
				fillcircle((i + 2) * space, (j + 2) * space, 10);
				break;
			default:
				break;
			}
		}
	}
}

bool judge(int row, int column)
{
	//判断竖
	for (int i = row - 4 >= 0 ? row - 4 : 0; i <= row; i++)
	{
		int sum = 0;
		int j = 0;
		for (j; j < 5 && column + j < mode && map[i + j][column]; j++)
		{
			sum += map[i + j][column];
		}
		if (sum == 5 && j == 5 || sum == 10)
			return true;
	}
	//判断横
	for (int i = column - 4 >= 0 ? column - 4 : 0; i <= column; i++)
	{
		int sum = 0;
		int j = 0;
		for (j; j < 5 && i + j < mode && map[row][i + j]; j++)
		{
			sum += map[row][i + j];
		}
		if (sum == 5 && j == 5 || sum == 10)
			return true;
	}
	//判断正斜
	int c1 = column;
	int r1 = row;
	int count = 0;
	while (count++ < 4)
	{
		c1--;
		r1--;
		if (c1 * r1 == 0)
			break;
	}
	for (int i = r1, j = c1; i <= row && j <= column; i++, j++)
	{
		int sum = 0;
		int k = 0;
		for (k; k < 5 && i + k < mode && j + k < mode && map[i + k][j + k]; k++)
		{
			sum += map[i + k][j + k];
		}
		if (sum == 5 && k == 5 || sum == 10)
			return true;
	}

	//判断反斜
	int c2 = column;
	int r2 = row;
	count = 0;
	while (count++ < 4)
	{
		r2--;
		c2++;
		if (r2 == 0 || c2 == mode - 1)
			break;
	}
	for (int i = r2, j = c2; i <= row && j >= 0; i++, j--)
	{
		int sum = 0;
		int k = 0;
		for (k; k < 5 && i + k < mode && j - k >= 0 && map[i + k][j - k]; k++)
		{
			sum += map[i + k][j - k];
		}
		if (sum == 5 && k == 5 || sum == 10)
			return true;
	}
	return false;
}

void changetitle()
{
	//获取窗口句柄
	HWND hwd = GetHWnd();
	//设置窗口标题
	SetWindowText(hwd, "五子棋");
}
int main()
{
	//initgraph(640, 640, SHOWCONSOLE);
	initgraph(640, 640);

	IMAGE image;
	loadimage(&image, "./图片.jpg");
	putimage(0, 0, &image);
	BGM();
	BeginBatchDraw();
	changetitle();


	while (1)
	{
		cleardevice();
		putimage(0, 0, &image);
		chessBoard();
		drawMouseMove();

		mouseOperation();
		//drawMouseMove();
		FlushBatchDraw();
		if (judge(row, column))
			break;
	}
	EndBatchDraw();
	settextcolor(RED);
	settextstyle(50, 0, "仿宋");
	setbkmode(TRANSPARENT);
	if (parity % 2 == 1)
		outtextxy(120, 200, "游戏结束!\n黑棋获胜");
	else
		outtextxy(120, 200, "游戏结束!\n白棋获胜");
	//弹出窗口，提示用户操作
	/*HWND hwd = GetHWnd();
	int isok = MessageBox(hwd, "你还要继续吗", "提示", MB_OKCANCEL);
	if (isok == IDCANCEL)
		break;
	else if(isok==IDOK)
	for (int i=0;i<mode;i++)
	{
		for (int j = 0; j < mode; j++)
		{
			map[i][j]=0;
		}
	}*/
	outtextxy(120, 280, "请按任意键继续！");

	system("pause");

	closegraph();

	return 0;
}