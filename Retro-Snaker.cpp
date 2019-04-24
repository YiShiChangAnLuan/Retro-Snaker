#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>  

//�����������ֵ
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

//����
int score;

//�Ѷ�  Ĭ���Ѷ�Ϊ��
int dif = 500;

//�߽ڵ�
typedef struct node
{
	//��ʾһ���ַ��ڿ���̨��Ļ�ϵ� ����
	COORD coor;
	struct node * next;
}snake_node;

//������ͷ
snake_node *snake_head;

//��ʼ���߳�Ϊ1
int len = 1;

//��ʼ��ʳ������
COORD food = { 2,3 };

//����ȽϺ���
bool coor_cmp(COORD ptr1, COORD ptr2)
{
	return (ptr1.X == ptr2.X && ptr1.Y == ptr2.Y);
}


//������ƶ��������е�(x,y)λ�ã�������һ������ṹ�� 
void gotoxy(COORD pt)
{
	//��� 
	HANDLE hand;

	//GetStdHandle������ȡһ��ָ���ض���׼�豸�ľ��
	//STD_OUTPUT_HANDLE���Ǵ����׼����ĺ� 
	hand = GetStdHandle(STD_OUTPUT_HANDLE);

	//SetConsoleCursorPosition�����������ÿ���̨����λ��
	SetConsoleCursorPosition(hand, pt);
}

//��ʼ����Ϸ
void init_game()
{
	score = 0;
	//�����ڴ�
	snake_head = (snake_node *)malloc(sizeof(snake_node));
	//��ʼ����ͷ(3,8)
	(snake_head->coor).X = 3;
	(snake_head->coor).Y = 8;
	snake_head->next = NULL;

	int i, j;
	for (i = 0; i < 16; i++)
	{
		for (j = 0; j < 21; j++)
		{
			//��ӡΧǽ ��#��ʾ
			if (i == 0 || i == 15 || j == 0 || j == 20)
			{
				printf("#");
			}

			//��ӡ�� ��*
			else if (i == (snake_head->coor).Y && j == (snake_head->coor).X)
			{
				printf("*");
			}

			//��ӡʳ�� ��$��ʾ
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

//�������ʳ��
COORD generate_food()
{
	COORD newfood;
	snake_node * p = NULL;
	//�ж�ʳ���Ƿ���������
	int insnake = 0;
	//�Ե�ǰ��ʱ���ṩ����
	srand((unsigned int)time(NULL));
	do {
		newfood.X = rand() % 20;
		newfood.Y = rand() % 15;
		for (p = snake_head; p != NULL; p = p->next)
		{
			//ʳ����������
			if (coor_cmp(newfood, p->coor))
				insnake = 1;
		}
	} while (newfood.X == 0 || newfood.X == 20 || newfood.Y == 0 || newfood.Y == 15 || insnake);

	return newfood;
}


//�ƶ���
void move_snake(int dir)
{
	snake_node *p, *t = NULL;

	//����ͷ
	snake_node *newhead = NULL;

	//������ͷ����ռ�
	newhead = (snake_node *)malloc(sizeof(snake_node));

	//��ͷ���������ͷ
	newhead->coor = snake_head->coor;

	//��ͷ��next��ָ��ָ���
	newhead->next = snake_head;

	//ͷָ��ָ���µ��ڴ�ռ�
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

	//����߳Ե���ʳ��
	if (coor_cmp(food, snake_head->coor))
	{
		score++;
		tmp = 1;
		//�����µ�ʳ��
		food = generate_food();
	}


	gotoxy(snake_head->coor);
	putchar('*');

	//���ھ�ֻ��ӡ�µ�ʳ��
	if (tmp)
	{
		len++;
		gotoxy(food);
		putchar('$');
	}
	//�����ھ͵�ɾ����β
	else
	{
		p = snake_head;
		while (p->next->next != NULL)
		{
			p = p->next;
		}

		//�������һ�ڵĵ�ַ
		t = p->next;
		//�����ڶ���ָ���next��ֵΪNULL
		p->next = NULL;
		//������ͷ��β�ƻ������β��ʱ�����ͷ�����
		if (!coor_cmp(t->coor, snake_head->coor))
		{
			gotoxy(t->coor);
			putchar(' ');
		}
		//�ͷ���β�ռ�
		t = NULL;

		//������һֱ������β����ʳ��
		COORD null = { 0,15 };
		gotoxy(null);

		Sleep(dif);
	}
}

//�ͷſռ�
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

//�ж�����
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

//�ٶȵõ�������
//�����µķ���
int get_dir(int old_dir)
{

	int new_dir = old_dir;
	//kbhit() ��鵱ǰ�Ƿ��м������룬�����򷵻�һ����0ֵ�����򷵻�0
	//getch() ��ch=_getch();��ȴ��㰴�������֮�󣬰Ѹü��ַ�����Ӧ��ASCII�븳��ch,��ִ���������䡣 
	if (_kbhit())
	{
		_getch();
		new_dir = _getch();
		//getch()����Ҫʹ������,ԭ������Ϊ��һ�η��ص�ֵָʾ�ü���չ���ַ����ڶ��ε��òŷ���ʵ�ʵļ�����
		//��������ȴ���1�����ܻ�ͷ���������ͷ������ԭ�������� 
		//abs(new_dir - old_dir) == 2 ��ʾ |LEFT-RIGHT|
		//abs(new_dir - old_dir) == 8 ��ʾ |UP-DOWN|
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
	printf("*       ѡ����Ҫ���ѵ�      *\n");
	printf("*          1.��           *\n");
	printf("*          2.�е�           *\n");
	printf("*          3.����           *\n");
	printf("*          4.����           *\n");
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
	printf("*   ��ӭ����̰������Ϸ����  *\n");
	printf("*****************************\n");
	while (1)
	{
		tmp = 0;
		printf("*****************************\n");
		printf("*        1.��ʼ��Ϸ         *\n");
		printf("*        2.������Ϸ         *\n");
		printf("*****************************\n");
		scanf("%d", &tmp);
		if (tmp == 1)
		{
			choosedif();
			system("cls");
			int dir = 1;//Ĭ�Ϸ���Ϊ��
			init_game();//��ʼ����Ϸ
			while (1)
			{
				//��ȡ����
				dir = get_dir(dir);
				//�ƶ�����
				move_snake(dir);
				//�ж�����
				if (!isalive())
				{
					break;
				}
			}
			//����
			system("cls");
			printf("*****************************\n");
			printf("*          ��Ϸ����         *\n");
			printf("*          �÷֣�%d          *\n", score);
			//�ͷſռ�
			free_node(snake_head);
			printf("*****************************\n");
			printf("*        �Ƿ��˳���Ϸ��     *\n");
			printf("*****************************\n");
			printf("*           1.��            *\n");
			printf("*           2.��            *\n");
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
