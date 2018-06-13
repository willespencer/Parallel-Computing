//
// Torbert, 4 Sept 2015
//
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
//
typedef struct Node
{
	char ch;
	int frequency;
	struct Node* left;
	struct Node* right;
} TreeNode ;

void binaryRecur(TreeNode* t, char goal, char code[], FILE* fout)
{
	if(t -> ch != 0 && t -> ch == goal)
		fprintf(fout,"%s", code);
	if(t -> left != NULL)
	{
		char code2[strlen(code)];
		strcpy(code2, code);
		strcat(code2, "0");
		binaryRecur(t -> left, goal, code2, fout);
	}
	if(t -> right != NULL)
	{	
		char code2[strlen(code)];
		strcpy(code2, code);
		strcat(code2, "1");
		binaryRecur(t -> right, goal, code2, fout);
	}

}

void printBinary(TreeNode* t, char msg[], FILE* fout)
{
	for(int x = 0; x < strlen(msg); x++)
	{
		TreeNode* currNode = t;
		char code[] = "";
		binaryRecur(currNode, msg[x], code, fout);
	}
	fprintf(fout, "\n");
}

void treeRecur(TreeNode* t, char code[], FILE* fout)
{
	if(t -> ch != 0)
		fprintf(fout, "%c%s\n", t -> ch,code);
	if(t -> left != NULL)
	{
		char code2[strlen(code)];
		strcpy(code2, code);
		strcat(code2, "0");
		treeRecur(t -> left, code2, fout);
	}
	if(t -> right != NULL)
	{	
		char code2[strlen(code)];
		strcpy(code2, code);
		strcat(code2, "1");
		treeRecur(t -> right, code2, fout);
	}
}

int encode()
{
	char msg[2000000] = {0};
	int msglength = 0;
	char ch;
	int numbytes;

	FILE* fMob = fopen("mobydick.txt", "r");
	while(1)
	{
		numbytes = fread(&ch, sizeof(char), 1, fMob);
		if(numbytes == 0) break;
		msg[msglength] = ch;
		msglength++;
		//printf("%d\n", msglength);
	}
	fclose(fMob);
	printf("read\n");
	//int msglength = 8;
	//char msg[] = "LOSSLESS";
	FILE* fout = fopen( "decodeME.txt" , "w");

	TreeNode* freq[256] = {0};
	int index = 0;
	for(int x = 0; x < msglength; x++)
	{
		char character = msg[x];
		for(int y = 0; y < 256; y++)
		{
 
			if(freq[y] == 0)
			{
				TreeNode* t = NULL;
				t = (TreeNode*)malloc(sizeof(TreeNode) );
				t -> ch  = character;
				t -> frequency = 1;
				t -> left = NULL;
				t -> right = NULL;
				freq[index] = t;
				index++;
				break;			
			}
			else
			{
				TreeNode* t = freq[y];
				if(t -> ch == character)
				{
					t -> frequency = t -> frequency +1;
					break;
				}	
			}	
		}
	}
	
	int length = index;
	fprintf(fout, "%d\n", length); //first line of file

	while(length > 1)
	{
		int min1dex;
		int min2dex;
		
		//makes min1dex the minimum of first two
		if(freq[0] -> frequency > freq[1] -> frequency)
		{
			min1dex = 1;
			min2dex = 0;
		}
		else
		{
			min1dex = 0;
			min2dex = 1;
		}

		for(int x = 2; x < length; x++)
		{
			if(freq[x] -> frequency < freq[min1dex] -> frequency)
			{
				min2dex = min1dex; //because min1 < min2
				min1dex = x;
			}
			else if(freq[x] -> frequency < freq[min2dex] -> frequency)
				min2dex = x;
		}
		//for swapping to work, min1 must be the first index
		if(min1dex < min2dex)
		{
			int temp = min1dex;
			min1dex = min2dex;
			min2dex = temp;
		}

		TreeNode* m1 = freq[min1dex];
		TreeNode* m2 = freq[min2dex];
		freq[min1dex] = freq[length-1];
		freq[min2dex] = freq[length-2];
		freq[length-1] = 0;
			
		TreeNode* t = NULL;
		t = (TreeNode*)malloc(sizeof(TreeNode) );
		t -> ch  = 0;
		t -> frequency = m1 -> frequency + m2 -> frequency;
		t -> left = m1;
		t -> right = m2;
		freq[length-2] = t;		
	
		length--;
	}
	TreeNode* head = freq[0];
	char code[] = "";
	
	treeRecur(head, code, fout);
	printBinary(head, msg, fout);	
	fclose(fout);
}

int decode()
{
	FILE* fin = fopen( "decodeME.txt" , "r" ) ;
	//
	char ch ;
	//
	int count = 0 ;
	//
	int numbytes ;
	//
	int totalCodes;
	int firstCode;
	char treeArray[2000000] = {0};
	int freqArray[256] = {0};
	
	int firstArray[3] = {0};
	int firstIndex = 0;
	while(1)
	{
		numbytes = fread(&ch, sizeof(char), 1, fin);
		if(ch == '\n') break;
		firstArray[firstIndex] = (ch - '0');
		firstIndex++;
	} 

	if(firstIndex == 1) totalCodes = firstArray[0];
	else if(firstIndex == 2) totalCodes = firstArray[0] * 10 + firstArray[1];
	else totalCodes = firstArray[0] * 100 + firstArray[1] * 10 + firstArray[2];
	
	while (totalCodes > 0)
	{
                numbytes  = fread( &ch , sizeof(char) , 1 , fin ) ;
		int index = 1;
		while(1)
		{
			char num;
			numbytes =  fread( &num , sizeof(char) , 1 , fin ) ;
			if(num == '\n')	
			{
				treeArray[index] = ch;
				break;
			}
			if(num == '0') index = index * 2 ;
			else if (num == '1') index = index * 2 + 1 ;
		}
		totalCodes -= 1;
	}

	char num;	
	int index = 1;
	int bitCount = 0;
	int charCount = 0;
	while(1)
	{
		while(treeArray[index] == 0)
		{
			numbytes = fread( &num, sizeof(char), 1, fin);
			bitCount +=1;
			if(num == '\n') break;
			if(num == '0') index = index * 2;
			else if(num == '1') index = index * 2 + 1;
		}
		if(num == '\n') break;
		charCount += 1;
		freqArray[(int)treeArray[index]] +=1;
		printf( "%c", treeArray[index]);
		index = 1;
	}
	
	double ratio = (double) ((8 * charCount) - bitCount)/ (8 * charCount);
	printf("\n%f\n", ratio);

	double shannonSum = 0;
	for(int x = 0; x < 256; x++)
	{
		if(freqArray[x] != 0)
		{
			double prob = freqArray[x] * (1.0 / charCount);
			shannonSum += freqArray[x] * (-1 * (log(prob) / log(2)));
		}
	}
	printf("%f\n", shannonSum);
	//
	fclose( fin );
	//
	return 0 ;
}

int main(){
	encode();
	printf("decode\n");
	decode();
	return 0;
}
//
// end of file
//
