//Will Spencer
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int r = 30;
int c = 40;


typedef struct ListNode
{
    int row ;
    int col ;
    //
    struct ListNode* next ;
    //
} Node ;

Node* createNode(int i, int j)
{
	Node* temp;
	temp -> row = i;
	temp -> col = j;
	return temp;
}

void setOnFire(char m[][c], int i, int j, Node* currNode)
{
	if(i < r && i > -1 && j < c && j > -1 && m[i][j] == '+')
	{
		m[i][j] = '?';
		Node* temp = createNode(i, j);
		currNode -> next = temp;
		currNode = temp;
	}
}

void printMatrix(char m[][c])
{
	for (int i=0; i<r; i++)
	{
		for(int j=0; j<c; j++)
		{
			printf("%c", m[i][j]);
		}	
		printf("\n");
	}
}

float fire(float p)
{
	//system( "clear" ) ;
	
	char m[r][c];
	int startTrees = 0;
	for(int i = 0; i < r; i++)
	{
		for(int j = 0; j < c; j++)
		{
			double randD = (double)rand() / (double)RAND_MAX ;
			if(randD < p)
			{
				m[i][j] = '+';
				startTrees++;
			}
			else
				m[i][j] = ' ';
		}
	}
	
	Node* currNode;
	Node* firstNode;
	
	printf("here1\n");
	
	int count = 0;
	for(int i = 0; i < r; i++)
	{
		if(m[i][0] == '+')
		{
			m[i][0] = 'f';
			Node* temp = createNode(i, 0);
			if(count == 0)
			{
				firstNode = temp;
				currNode = temp;
				count++;
			}
			else
			{
				currNode -> next = temp;
				currNode = temp;
			}
		}
	}
	
	printf("here2\n");
	//printf("Start\n");
	//printMatrix(m);
	int steps = 0;
	
	while(1)
	{
		Node* nextNode;
		Node* originalNode;
		originalNode -> next = nextNode;
		
		while(firstNode != NULL)
		{
			Node* temp = firstNode;
			int j = temp -> col;
			int i = temp -> row;
			m[i][j] = ' ';
			setOnFire(m, i, j+1, nextNode);
			setOnFire(m, i, j-1, nextNode);
			setOnFire(m, i+1, j, nextNode);
			setOnFire(m, i-1, j, nextNode);
			firstNode = firstNode -> next;
		}
		
		firstNode = nextNode;
		steps++;
		
		printf("here3\n");
		if(nextNode == NULL) // might need to bve nextNode -> next
			break;
		
	}

	/*int treesLeft = 0;
	for(int i = 0; i < r; i++)
			for(int j = 0; j < c; j++)
				if(m[i][j] == '+')
					treesLeft++;*/

	/*printf("End\n");
	printMatrix(m);
	printf("Steps: %d\n", steps);
	printf("Initial Trees: %d\n", startTrees);
	printf("Trees Left: %d\n", treesLeft);
	printf("Proportion: %f\n", ((float) treesLeft)/((float)startTrees));*/
	
	float returnVal = (float)steps / (float)c;
	return returnVal;
}

int main()
{
	int rseed;
	rseed = time( NULL ) ;
	srand( rseed ) ;
	
	printf("Graph\n");
	for(float x = 0; x < 1.04; x = x + .05)
	{
		float avg = 0;
		float total = 0;
		for(int y = 0; y < 100; y++)
			total += fire(x);
		avg = total / 100.0;
		printf("%f %f\n", x, avg);
	}
	
}
