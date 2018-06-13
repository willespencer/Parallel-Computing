//Will Spencer
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int r = 30;
int c = 40;
int trialNum = 500;
float deltaP = .002;

void setOnFire(char m[][c], int i, int j)
{
	if(i < r && i > -1 && j < c && j > -1 && m[i][j] == '+')
		m[i][j] = '?';
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
	
	for(int i = 0; i < r; i++)
	{
		if(m[i][0] == '+')
			m[i][0] = 'f';
	}
	
	//printf("Start\n");
	//printMatrix(m);
	
	int steps = 0;
	while(1)
	{
		steps++;
		int count = 0;
		for(int i = 0; i < r; i++)
		{
			for(int j = 0; j < c; j++)
			{
				if(m[i][j] == 'f')
				{
					count++;
					m[i][j] = ' ';
					setOnFire(m, i, j+1);
					setOnFire(m, i, j-1);
					setOnFire(m, i+1, j);
					setOnFire(m, i-1, j);
				}
			}
		}
		for(int i = 0; i < r; i++)
			for(int j = 0; j < c; j++)
				if(m[i][j] == '?') // temporary character to keep it step by step
					m[i][j] = 'f';

		if(count == 0)
			break;
	}
	int treesLeft = 0;
	for(int i = 0; i < r; i++)
			for(int j = 0; j < c; j++)
				if(m[i][j] == '+')
					treesLeft++;

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
	for(float x = 0; x < 1.0001; x = x + deltaP)
	{
		float avg = 0;
		float total = 0;
		for(int y = 0; y < trialNum; y++)
			total += fire(x);
		avg = total / trialNum;
		
		//printf("%f %f\n", x, avg);
		//printf("%f\n", avg);
	}
}
