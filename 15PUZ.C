#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
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
	int i, j;
	for(i=0; i<S; i++)
	{
		for(j=0; j<S; j++)
		{
			if(a[i][j] == 0)
			{
				printf("%c\t", 254);
			}
			else
			{
				printf("%d\t", a[i][j]);
			}
		}
		printf("\n\n");
	}
	if(mode == USER)
	{
		printf("Use arrow keys to play\nQ: Quit\t\tG: God Mode\tR: Reset\n");
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
	clrscr();
	display(start->a, GOD);
	if(mode == USER)
	{
		printf("YOU WIN!!!");
	}
	printf("\n\nPlay again? (Y/N)");
	while(1)
	{
		ch = getch();
		if(ch == 'Y' || ch == 'y')
		{
			reset();
			generateQuestion();
			break;
		}
		else if(ch == 'N' || ch == 'n')
		{
			exit(0);
		}
	}
}


void godMode()
{
	node *temp;
	while(compare(start->a, initial) == 0)
	{
		clrscr();
		display(start->a, GOD);
		sleep(1);
		temp = start;
		start = start->next;
		free(temp);
	}
	youWin(GOD);
}



int main()
{
	int ch=0, i ,j;
	reset();
	generateQuestion();
	while(ch != 'Q' && ch != 'q')
	{
		clrscr();
		display(start->a, USER);
		ch = getch();
		if(ch == UP)
		{
			up();
		}
		else if(ch == DOWN)
		{
			down();
		}
		else if(ch == RIGHT)
		{
			right();
		}
		else if(ch == LEFT)
		{
			left();
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
	return 0;
}