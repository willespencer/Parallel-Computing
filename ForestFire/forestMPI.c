#include <stdio.h>
// 
#include "mpi.h"
// 
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

double fire(float p)
{
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
				if(m[i][j] == '?') // temp character
					m[i][j] = 'f';

		if(count == 0)
			break;
	}
	int treesLeft = 0;
	for(int i = 0; i < r; i++)
			for(int j = 0; j < c; j++)
				if(m[i][j] == '+')
					treesLeft++;
	
	double returnVal = (double)steps / (double)c;
	return returnVal;
}

int main( int argc , char* argv[] )
{
   //
   // MPI variables
   //
   int        rank    ;
   int        size    ;
   MPI_Status status  ;
   int        tag = 0 ;
   //
   // other variables
   //
   int        k , j  ;
   double     prob , nbt ;
   double array[2];
   //
   // boilerplate
   //
   MPI_Init(      &argc          , &argv ) ;
   MPI_Comm_size( MPI_COMM_WORLD , &size ) ; // same
   MPI_Comm_rank( MPI_COMM_WORLD , &rank ) ; // different
   //
   // manager has rank = 0
   //
   
   if( rank == 0 )
   {
      printf( "\n" ) ;
      int rseed;
	  //rseed = time( NULL ) ;
	  rseed = 0;
	  srand( rseed ) ;
      
      //
      prob = 0;
      //
      for( j = 1 ; j < size ; j++ )
      {
         MPI_Send( &prob , 1 , MPI_DOUBLE , j , tag , MPI_COMM_WORLD ) ;
         
         prob += deltaP;
      }
      //
      while(1)
      {
		MPI_Recv(array, 2, MPI_DOUBLE , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
			 

		//
		j = status.MPI_SOURCE ;
		//
		double tempP = array[0];
		nbt = array[1];
			 
		//printf( "%d %d %f %f \n" , j , size , tempP, nbt ) ;
			 
		prob += deltaP;
		
		if(prob > 1.0)
			break;
			 
		MPI_Send( &prob , 1 , MPI_DOUBLE , j , tag , MPI_COMM_WORLD ) ;
		  
		
	  }
      //
      for( j = 1 ; j < size ; j++ ) //to break all the workers
         MPI_Send( &prob , 1 , MPI_DOUBLE , j , tag , MPI_COMM_WORLD ) ;
      
      printf( "MANAGER FINISHED\n" );
   }
   //
   // workers have rank > 0
   //
   else
   {
	  while(1)
	  {

		  MPI_Recv( &prob , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status ) ;
		  
		  if(prob > 1.0)
		  {
			  break;
		  }
		  

		  float steps = 0;
		  float total = 0;
		  for(int y = 0; y < trialNum; y++)
			total += fire(prob);
		  steps = total / trialNum;		  
		  
		  array[0] = prob;
		  array[1] = steps;
		  
		  MPI_Send( array, 2 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD ) ;
		  

	  }
   }
   //
   // boilerplate
   //
   MPI_Finalize() ;
   //
   return 0;
}
// 
// end of file
// 
