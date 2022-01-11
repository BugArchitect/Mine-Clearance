/*
Created by BugArchitect on 2022/1/2.
Copyright © 2022年 BugArchitect. All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <graphics.h>

#define ROW 20   //ROW 10
#define COL 20	 //COL 10
#define IMGW 40    //IMGW 40

//void HideWindow();
void InitMap(int map[ROW][COL]);
void MakeMap(int map[ROW][COL]);
void DrawMap(int map[ROW][COL], IMAGE image[]);
void MouseOperate(int map[ROW][COL]);
void OpenBlank(int map[ROW][COL], int row, int col);
void Judge(int map[ROW][COL], int row, int col);
void Game(int map[ROW][COL]);

int cnt = 0;
int mine = 0;
IMAGE image[12];

int main()
{
	system("title Mine Clearance");
	//HideWindow();
	//创建图形窗口
	initgraph(ROW * IMGW, COL * IMGW, SHOWCONSOLE);
	//生成随机数种子
	srand((unsigned)time(NULL));
	//定义地图
	int map[ROW][COL] = { 0 };
	InitMap(map);
	//IMAGE Background;
	char fileName[20] = { 0 };
	for (int i = 0; i <= 11; i++)
	{
		sprintf_s(fileName, "./image/%d.jpg", i);
		loadimage(image + i, fileName, IMGW, IMGW);
		//putimage(i*IMGW,0,image + i);
	}
	//loadimage(&Background, "./image/Background.jpg", 400, 400);
	//putimage(0,0,&Background);
	Game(map);
	closegraph();
	//system("pause");
	return 0;
}

void InitMap(int map[ROW][COL])
{
	cnt = 0;
	//游戏开始前先清屏
	memset(map, 0, ROW * COL * sizeof(int));
	//雷的设置
	for (int i = 0; i < 20; )
	{
		int r = rand() % ROW;
		int c = rand() % COL;
		if (map[r][c] == 0)
		{
			map[r][c] = -1;
			i++;
		}
	}
	//雷所占九宫格中的处理
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (map[i][j] == -1)
			{
				for (int r1 = i - 1; r1 <= i + 1; r1++)
				{
					for (int c1 = j - 1; c1 <= j + 1; c1++)
					{
						if ((r1 >= 0 && r1 < ROW && c1 >= 0 && c1 < COL) && map[r1][c1] != -1)//避免数组越界
						{
							map[r1][c1]++;
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			map[i][j] += 10;
		}
	}
}

void MakeMap(int map[ROW][COL])
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			printf("%2d ", map[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void DrawMap(int map[ROW][COL], IMAGE image[])
{

	for (int i = 0; i < ROW; i++)				//将图片放到对应的数字格子上
	{
		for (int j = 0; j < COL; j++)
		{
			if (map[i][j] >= 0 && map[i][j] <= 8)
			{
				putimage(j * IMGW, i * IMGW, image + map[i][j]);		//翻开格子后周围雷的数量
			}
			else if (map[i][j] == -1)
			{
				putimage(j * IMGW, i * IMGW, image + 10);		//雷的图片
			}
			else if (map[i][j] >= 9 && map[i][j] <= 18)
			{
				putimage(j * IMGW, i * IMGW, image + 9);		//用空白格将每一个格子覆盖
			}
			else if (map[i][j] >= 19 && map[i][j] <= 28)
			{
				putimage(j * IMGW, i * IMGW, image + 11);
			}
		}
	}
}

void MouseOperate(int map[ROW][COL])
{
	MOUSEMSG msg;
	if (MouseHit())
	{
		msg = GetMouseMsg();
		int r = msg.y / IMGW;
		int c = msg.x / IMGW;
		switch (msg.uMsg)
		{
		case WM_LBUTTONDOWN:
			if (map[r][c] >= 9 && map[r][c] <= 18)//鼠标左键按下
			{
				map[r][c] -= 10;
				if (map[r][c] >= 0 && map[r][c] <= 8)
				{
					cnt++;
					OpenBlank(map, r, c);
					Judge(map, r, c);
					MakeMap(map);
				}
				else if (map[r][c] == -1)
				{
					mine++;
				}
			}
			break;
		case WM_RBUTTONDOWN:			//鼠标右键按下			
			if (map[r][c] >= 9 && map[r][c] <= 18)
			{
				map[r][c] += 10;
			}
			else if (map[r][c] >= 19 && map[r][c] <= 28)
			{
				map[r][c] -= 10;
			}
			break;

		}

	}
}

void OpenBlank(int map[ROW][COL], int row, int col)
{
	if (map[row][col] == 0)
	{
		for (int i = row - 1; i <= row + 1; i++)
		{
			for (int j = col - 1; j <= col + 1; j++)
			{
				if ((i >= 0 && i < ROW && j >= 0 && j < COL) && map[i][j] >= 9 && map[i][j] <= 18)
				{
					map[i][j] -= 10;
					cnt++;
					OpenBlank(map, i, j);
				}
			}
		}
	}
}

void Judge(int map[ROW][COL], int row, int col)
{
	if (map[row][col] == -1)
	{
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COL; j++)
			{
				if (map[i][j] == 9)
				{
					map[i][j] -= 10;
				}
				if (map[i][j] == 19)
				{
					map[i][j] -= 20;
				}
			}
		}
	}
}

void Game(int map[ROW][COL])
{
	while (true)
	{
		MouseOperate(map);
		DrawMap(map, image);
		if (mine == 1)
		{
			//Judge(map, row, col);
			int tmp = MessageBox(GetHWnd(), "很可惜，你竟然踩到雷了，要再来一把吗？关注嘉然，顿顿解馋", "Hint", MB_OKCANCEL);
			if (tmp == IDOK)
			{

				mine--;
				InitMap(map);
				MakeMap(map);
			}
			else
			{
				//printf("%d %d\n",cnt,mine);
				exit(233);
			}
		}
		if (cnt == ROW * COL - 40)  //40个雷
		{
			//Judge(map,row,col);
			int tmp = MessageBox(GetHWnd(), "恭喜你，获胜了！要再来一把吗？", "Hint", MB_OKCANCEL);
			if (tmp == IDOK)
			{
				InitMap(map);
				MakeMap(map);
			}
			else
			{
				//printf("%d %d\n",cnt,mine);
				exit(666);
			}
		}
	}
}

void HideWindow() {
	HWND hwnd = GetForegroundWindow(); //获取程序启动时的窗口
	if (hwnd) {
		ShowWindow(hwnd, SW_HIDE);    //设置指定窗口的显示状态
	}
}
