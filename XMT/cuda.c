#include <stdio.h>
#include <math.h>

// called from host, run on device
__global__ void add_arrays_gpu(float *in1,float *in2,float *out)
{
	int idx=threadIdx.x; // flat model
	
	out[idx]=in1[idx]+in2[idx];
}

int checkSum(int array[], int numElements)
{
	int sum = 0;
	for(int x = 0; x < numElements; x++)
		sum += array[x];
	return sum;
}


int main(int argc,char* argv[])
{  
	
	// pointers to host memory
	int *a,*b,*c;

	// pointers to device memory
	int *a_d,*b_d,*c_d;
	int N=18;
	int i;

	// allocate arrays a, b and c on host
	a=(float*)malloc(N*sizeof(int));
	b=(float*)malloc(N*sizeof(int));
	c=(float*)malloc(N*sizeof(int));
	
	// allocate arrays a_d, b_d and c_d on device
	cudaMalloc((void**)&a_d,sizeof(int)*N);
	cudaMalloc((void**)&b_d,sizeof(int)*N);
	cudaMalloc((void**)&c_d,sizeof(int)*N);
	
	// copy input from host memory to device memory
	cudaMemcpy(a_d,a,sizeof(float)*N,cudaMemcpyHostToDevice);
	cudaMemcpy(b_d,b,sizeof(float)*N,cudaMemcpyHostToDevice);
	
	// execution configuration: How the threads are arranged, FLAT and LINEAR.
	dim3 dimGrid(1),dimBlock(N);
	add_arrays_gpu<<<dimGrid,dimBlock>>>(a_d, b_d, c_d);
	
	// copy result from device memory to host memory
	cudaMemcpy(c,c_d,sizeof(float)*N,cudaMemcpyDeviceToHost);
	
	a = {3, 1,  4, 1, 5, 9, 2, 6};
	//int array[] = {1, 1, 2, 3, 5, 2, 1, 2};
	int numElements = 8;
	
	int rows = (log(numElements) / log(2) ) + 1; //log base 2 of elements + 1
	i//nt m[rows][numElements];
	
	for(int x = 0; x < numElements; x++) //sets first row of M to array
		b[0][x] = a[x];
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
	
	free(a);
	free(b);
	free(c);
	cudaFree(a_d);
	cudaFree(b_d);
	cudaFree(c_d);
}

