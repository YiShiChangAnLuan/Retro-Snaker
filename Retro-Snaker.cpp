#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>  

//方向键的整数值
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

//分数
int score;

//难度  默认难度为简单
int dif = 500;

//蛇节点
typedef struct node
{
	//表示一个字符在控制台屏幕上的 坐标
	COORD coor;
	struct node * next;
}snake_node;

//建立蛇头
snake_node *snake_head;

//初始化蛇长为1
int len = 1;

//初始化食物坐标
COORD food = { 2,3 };

//坐标比较函数
bool coor_cmp(COORD ptr1, COORD ptr2)
{
	return (ptr1.X == ptr2.X && ptr1.Y == ptr2.Y);
}


//将光标移动到命令行的(x,y)位置，参数是一个坐标结构体 
void gotoxy(COORD pt)
{
	//句柄 
	HANDLE hand;

	//GetStdHandle函数获取一个指向特定标准设备的句柄
	//STD_OUTPUT_HANDLE正是代表标准输出的宏 
	hand = GetStdHandle(STD_OUTPUT_HANDLE);

	//SetConsoleCursorPosition函数用于设置控制台光标的位置
	SetConsoleCursorPosition(hand, pt);
}

//初始化游戏
void init_game()
{
	score = 0;
	//分配内存
	snake_head = (snake_node *)malloc(sizeof(snake_node));
	//初始化蛇头(3,8)
	(snake_head->coor).X = 3;
	(snake_head->coor).Y = 8;
	snake_head->next = NULL;

	int i, j;
	for (i = 0; i < 16; i++)
	{
		for (j = 0; j < 21; j++)
		{
			//打印围墙 用#表示
			if (i == 0 || i == 15 || j == 0 || j == 20)
			{
				printf("#");
			}

			//打印蛇 用*
			else if (i == (snake_head->coor).Y && j == (snake_head->coor).X)
			{
				printf("*");
			}

			//打印食物 用$表示
			else if (i == food.Y && j == food.X)
			{
				printf("$");
			}
			else
				printf(" ");
		}
		printf("\n");
	}
}

//随机生成食物
COORD generate_food()
{
	COORD newfood;
	snake_node * p = NULL;
	//判断食物是否在蛇身上
	int insnake = 0;
	//以当前的时间提供种子
	srand((unsigned int)time(NULL));
	do {
		newfood.X = rand() % 20;
		newfood.Y = rand() % 15;
		for (p = snake_head; p != NULL; p = p->next)
		{
			//食物在蛇身上
			if (coor_cmp(newfood, p->coor))
				insnake = 1;
		}
	} while (newfood.X == 0 || newfood.X == 20 || newfood.Y == 0 || newfood.Y == 15 || insnake);

	return newfood;
}


//移动蛇
void move_snake(int dir)
{
	snake_node *p, *t = NULL;

	//新蛇头
	snake_node *newhead = NULL;

	//给新蛇头分配空间
	newhead = (snake_node *)malloc(sizeof(snake_node));

	//旧头坐标给心新头
	newhead->coor = snake_head->coor;

	//新头的next的指针指向旧
	newhead->next = snake_head;

	//头指针指向新的内存空间
	snake_head = newhead;

	int tmp = 0;

	switch (dir)
	{
	case UP:(snake_head->coor).Y--;
		break;
	case DOWN:(snake_head->coor).Y++;
		break;
	case LEFT:(snake_head->coor).X--;
		break;
	case RIGHT:(snake_head->coor).X++;
		break;
	}

	//如果蛇吃到了食物
	if (coor_cmp(food, snake_head->coor))
	{
		score++;
		tmp = 1;
		//生成新的食物
		food = generate_food();
	}


	gotoxy(snake_head->coor);
	putchar('*');

	//长节就只打印新的食物
	if (tmp)
	{
		len++;
		gotoxy(food);
		putchar('$');
	}
	//不长节就得删除蛇尾
	else
	{
		p = snake_head;
		while (p->next->next != NULL)
		{
			p = p->next;
		}

		//记下最后一节的地址
		t = p->next;
		//倒数第二的指针的next赋值为NULL
		p->next = NULL;
		//避免蛇头蛇尾绕环清除蛇尾的时候把蛇头清理掉
		if (!coor_cmp(t->coor, snake_head->coor))
		{
			gotoxy(t->coor);
			putchar(' ');
		}
		//释放蛇尾空间
		t = NULL;

		//避免光标一直跟着蛇尾或者食物
		COORD null = { 0,15 };
		gotoxy(null);

		Sleep(dif);
	}
}

//释放空间
void free_node(snake_node * h)
{
	snake_node *p = h, *q;
	while (p != NULL)
	{
		q = p;
		p = p->next;
		free(q);
	}
}

//判断死活
int isalive()
{
	int tmp = 0;
	snake_node *p = NULL;
	for (p = snake_head->next; p != NULL; p = p->next)
	{
		if (coor_cmp(p->coor, snake_head->coor))
		{
			tmp = 1;
		}
	}
	return ((snake_head->coor).X == 0 || (snake_head->coor).X == 20 || (snake_head->coor).Y == 0 || (snake_head->coor).Y == 15 || tmp) ? 0 : 1;
}

//百度得到的内容
//返回新的方向
int get_dir(int old_dir)
{

	int new_dir = old_dir;
	//kbhit() 检查当前是否有键盘输入，若有则返回一个非0值，否则返回0
	//getch() 用ch=_getch();会等待你按下任意键之后，把该键字符所对应的ASCII码赋给ch,再执行下面的语句。 
	if (_kbhit())
	{
		_getch();
		new_dir = _getch();
		//getch()函数要使用两次,原因是因为第一次返回的值指示该键扩展的字符，第二次调用才返回实际的键代码
		//如果蛇身长度大于1，则不能回头，如果摁回头方向，则按原来方向走 
		//abs(new_dir - old_dir) == 2 表示 |LEFT-RIGHT|
		//abs(new_dir - old_dir) == 8 表示 |UP-DOWN|
		if (len > 1 && (abs(new_dir - old_dir) == 2 || abs(new_dir - old_dir) == 8))
		{
			new_dir = old_dir;
		}
	}
	return new_dir;
}

void choosedif()
{
	int tmp;
	printf("*****************************\n");
	printf("*       选择你要的难得      *\n");
	printf("*          1.简单           *\n");
	printf("*          2.中等           *\n");
	printf("*          3.困难           *\n");
	printf("*          4.地狱           *\n");
	printf("*****************************\n");
	scanf("%d", &tmp);
	if (tmp == 1)
	{
		dif == 100;
	}
	if (tmp == 2)
	{
		dif = 400;
	}
	if (tmp == 3)
	{
		dif = 200;
	}
	if (tmp == 4)
	{
		dif = 100;
	}
}


void menu()
{
	int tmp;
	printf("*****************************\n");
	printf("*   欢迎来到贪吃蛇游戏！！  *\n");
	printf("*****************************\n");
	while (1)
	{
		tmp = 0;
		printf("*****************************\n");
		printf("*        1.开始游戏         *\n");
		printf("*        2.结束游戏         *\n");
		printf("*****************************\n");
		scanf("%d", &tmp);
		if (tmp == 1)
		{
			choosedif();
			system("cls");
			int dir = 1;//默认方向为上
			init_game();//初始化游戏
			while (1)
			{
				//获取方向
				dir = get_dir(dir);
				//移动蛇身
				move_snake(dir);
				//判断死活
				if (!isalive())
				{
					break;
				}
			}
			//清屏
			system("cls");
			printf("*****************************\n");
			printf("*          游戏结束         *\n");
			printf("*          得分：%d          *\n", score);
			//释放空间
			free_node(snake_head);
			printf("*****************************\n");
			printf("*        是否退出游戏：     *\n");
			printf("*****************************\n");
			printf("*           1.是            *\n");
			printf("*           2.否            *\n");
			printf("*****************************\n");
			scanf("%d", &tmp);
			if (tmp == 1)
			{
				break;
			}
		}
		else
			break;
	}
}

int main()
{
	menu();
	return 0;
}
