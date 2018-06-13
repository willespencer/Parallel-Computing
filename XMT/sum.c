#include <stdio.h>
#include <math.h>
int checkSum(int array[], int numElements)
{
	int sum = 0;
	for(int x = 0; x < numElements; x++)
		sum += array[x];
	return sum;
}


int main(int argc,char* argv[])
{  
	int array[] = {3, 1,  4, 1, 5, 9, 2, 6};
	//int array[] = {1, 1, 2, 3, 5, 2, 1, 2};
	int numElements = 8;
	
	int rows = (log(numElements) / log(2) ) + 1; //log base 2 of elements + 1
	int m[rows][numElements];
	
	for(int x = 0; x < numElements; x++) //sets first row of M to array
		m[0][x] = array[x];
	printf("Arr: %d\n", m[0][5]);
	
	for(int r = 1; r < rows; r++)
	{
		for(int c = 0; c < pow(2, rows - r - 1); c++)
		{
			//add up the two elements "below" it
			m[r][c] = m[r-1][2 * c] + m[r-1][2 * c + 1];
			printf("Value: %d, %d, %d\n", m[r-1][2*c], m[r-1][2*c+1], m[r][c]);
		}
	}
	
	printf("Sum: %d\n", m[rows-1][0]);
	printf("Check: %d\n", checkSum(array, numElements));
	
	int c[rows][numElements];
			
	for(int h = rows - 1; h >= 0; h--)
	{
		for(int i = 0; i < numElements / pow(2, h); i++)
		{
			if(i == 0)
				c[h][i] = m[h][i]; //leftmost element = sum
			else if(i % 2 == 1)
				c[h][i] = c[h+1][i/2]; //right child = it's parent
			else
				c[h][i] = c[h+1][((i-1)/2)] + m[h][i];
		}
	}
		
	//output
	for(int r = 0; r < rows; r++)
	{
		for(int col = 0; col < pow(2, rows - r - 1); col++)
		{
			printf("(%d, %d): %d\n", r, col, c[r][col]);
		}
	}
}

