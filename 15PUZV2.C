#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#include<graphics.h>
#define S 4
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define USER 1
#define GOD 0

struct node
{
	int a[S][S];
	struct node *next;
};
typedef struct node node;
node *start = NULL;

int initial[S][S];
int upEnabled, downEnabled, leftEnabled, rightEnabled;
int currX, currY;

void enabled()
{
	rightEnabled = currY == S-1 ? 0:1;
	leftEnabled = currY == 0 ? 0:1;
	upEnabled = currX == 0 ? 0:1;
	downEnabled = currX == S-1 ? 0:1;
}

void del(node *temp)
{
	node *temp1;
	while(start != temp)
	{
		temp1 = start;
		start = start->next;
		free(temp1);
	}
}

int compare(int a[S][S], int b[S][S])
{
	int i, j, ret = 1;
	for(i=0; i<S; i++)
	{
		for(j=0; j<S; j++)
		{
			if(a[i][j] != b[i][j])
			{
				ret = 0;
				break;
			}
		}
		if(ret == 0)
		{
			break;
		}
	}
	return ret;
}

int searchDel(int ar[S][S])
{
	int ret = 0;
	node *temp = start;
	while(temp != NULL)
	{
		if(compare(ar,temp->a) == 1)
		{

			del(temp);
			ret = 1;
			break;
		}
		temp = temp->next;
	}
	return ret;
}

node* getNode()
{
	node *temp = (node*) malloc(sizeof(node));
	return temp;
}

void copy(int s[S][S], int d[S][S])
{
	int i, j;
	for(i=0; i<S; i++)
	{
		for(j=0; j<S; j++)
		{
			d[i][j] = s[i][j];
		}
	}
}
void reset()
{
	int i, j, count = 1;
	for(i = 0; i < S; i++)
	{
		for(j = 0; j < S; j++)
		{
			initial[i][j] = count++;
		}
	}
	initial[S-1][S-1] = 0;
	currX = currY = S-1;
	enabled();
	del(NULL);
	start = getNode();
	copy(initial, start->a);
}

void display(int a[S][S], int mode)
{

	char ch[3];
	int i, j , k, l;

	for(i = 10, k=0;k<S ; i+=30,k++)
	{
		for(j=10, l=0; l<S; j+=30, l++)
		{
			rectangle(j,i,j+30,i+30);
			if(a[k][l] != 0)
			{
				sprintf(ch, "%d", a[k][l]);
				outtextxy(j+8, i+8, ch);
			}
			else
			{
				floodfill(j+1,i+1,LIGHTRED);
			}
		}
	}
	rectangle(10, 200, 40, 230);
	outtextxy(18, 208, "Q   Quit");
	if(mode == USER)
	{
		outtextxy(10, 150, "Use arrow keys to play");
		rectangle(10, 160, 40, 190); rectangle(42, 160, 72, 174); rectangle(42, 176, 72, 190); rectangle(74, 160, 104, 190);
		outtextxy(18, 168, "<");     outtextxy(50, 166, "^");     outtextxy(50, 181, "v");     outtextxy(82, 168, ">");
		rectangle(110, 200, 140, 230);       rectangle(250, 200, 280, 230);
		outtextxy(118, 208, "G   God Mode"); outtextxy(258, 208, "R   Reset");
	}
}

void insert(int a[S][S])
{
	node *temp;
	if(searchDel(a) == 0)
	{
		//printf("in insert\n");
		temp = getNode();
		copy(a, temp->a);
		temp->next = start;
		start = temp;
	}
}
void swap(int *a, int *b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void up()
{
	int a[S][S];
	if(upEnabled == 1)
	{
		copy(start->a, a);
		swap(&a[currX][currY], &a[currX-1][currY]);
		currX--;
		enabled();
		insert(a);
	}
}
void down()
{
	int a[S][S];
	if(downEnabled == 1)
	{
		copy(start->a, a);
		swap(&a[currX][currY], &a[currX+1][currY]);
		currX++;
		enabled();
		insert(a);
	}
}
void left()
{
	int a[S][S];
	if(leftEnabled == 1)
	{
		copy(start->a, a);
		swap(&a[currX][currY], &a[currX][currY-1]);
		currY--;
		enabled();
		insert(a);
	}
}
void right()
{
	int a[S][S];
	if(rightEnabled == 1)
	{
		copy(start->a, a);
		swap(&a[currX][currY], &a[currX][currY+1]);
		currY++;
		enabled();
		insert(a);
	}
}

void generateQuestion()
{
	int i, rnum;
	srand(time(NULL));
	for(i=0; i<100; i++)
	{
		rnum = rand() % 4;
		if(rnum == 0)
		{
			up();
		}
		else if(rnum == 1)
		{
			down();
		}
		else if(rnum == 2)
		{
			right();
		}
		else
		{
			left();
		}
	}
}

void youWin(int mode)
{
	char ch=0;
	cleardevice();
	display(start->a, GOD);
	if(mode == USER)
	{
		outtextxy(10, 150, "YOU WIN!!!");
	}
	rectangle(110, 200, 140, 230);
	outtextxy(118, 208, "R   Play Again");
	while(1)
	{
		ch = getch();
		if(ch == 'R' || ch == 'r')
		{
			reset();
			generateQuestion();
			break;
		}
		else if(ch == 'Q' || ch == 'q')
		{
			exit(0);
		}
	}
}


void godMode()
{
	node *temp;
	int i=0;
	char ch=0;
	while(compare(start->a, initial) == 0)
	{
		cleardevice();
		if(i%2 == 0)
		{
			outtextxy(10, 150, "GOD MODE");
		}
		display(start->a, GOD);
		sleep(1);
		temp = start;
		start = start->next;
		free(temp);
		i++;
		if(kbhit())
		{
			ch = getch();
			if(ch == 'q' || ch == 'Q')
			{
				break;
			}
		}
	}
	youWin(GOD);
}



int main()
{
	int ch=0, i ,j;
	int gd=DETECT,gm;
	initgraph(&gd, &gm, "..\\bgi");
	setcolor(LIGHTRED);
	setfillstyle(WIDE_DOT_FILL, LIGHTGRAY);
	reset();
	generateQuestion();
	while(ch != 'Q' && ch != 'q')
	{
		cleardevice();
		display(start->a, USER);
		ch = getch();
		if(ch == UP)
		{
			down();
		}
		else if(ch == DOWN)
		{
			up();
		}
		else if(ch == RIGHT)
		{
			left();
		}
		else if(ch == LEFT)
		{
			right();
		}
		else if(ch == 'G' || ch == 'g')
		{
			godMode();
		}
		else if(ch == 'r' || ch == 'R')
		{
			reset();
			generateQuestion();
		}
		if(compare(start->a, initial) == 1)
		{
			youWin(USER);
		}
	}
	closegraph();
	return 0;
}