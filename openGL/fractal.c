// 
// to compile type... make 
// uses file named... Makefile
// 
// a.out: fireGL.c
// 	gcc fireGL.c -lGL -lGLU -lglut
// 
// tab character '\t' before gcc
// 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
// 
#include <GL/glut.h>
// 
#define N     600
#define TREE    1 
#define FIRE    2 
#define SPARK   3 
#define BURNT   8 
#define EMPTY   0 
// 
char   t[N][N]     ; 
int    pause = 0   ; 
int    step        ; 
double prob = 0.60 ; 
double factor = 1;
double minX = -2;
double minY = -1.5; 
//

void inFrac(int max, int x, int y, double factor)
{
	double xWidth = 4 * factor;
	double yWidth = 3 * factor;
	
	double a = ((((double) x) / ((double) N)) * (xWidth)) + minX;
	double b = ((((double) y) / ((double) N)) * (yWidth)) + minY;
	
	//z = Re+Im*i
	double Re = 0; //real
	double Im = 0; //imaginary
	int steps=0;
	
	#pragma omp parallel for private(steps)
	while(steps < max)
	{
		if(pow(Re, 2) + pow(Im, 2) > 4)
			break;
		steps++;
		
		//z^2 = (Re+ Im * i)^2
		double newRe = (pow(Re, 2) - pow(Im, 2)) + a;
		double newIm = (2 * Re * Im) + b;
		Re = newRe;
		Im = newIm;
	}

	double gradient = 1 - ((double) steps) / ((double) max);
	glColor3f(gradient, 0, 0);
	
	if(steps == max)
		glColor3f(0, 0, 0);
	
	glBegin(GL_POINTS);
	glVertex2f(x,y);
	glEnd();
}

void outFrac(int max, int xCent, int yCent, double factor, double start)
{
	double xOldWidth = 4 * factor * start; //start is 1 if original, 2 if zoomed
	double yOldWidth = 3 * factor * start; //start is 1 if original, 2 if zoomed
	
	double centerX = ((((double) xCent) / ((double) N)) * xOldWidth) + minX;
	double centerY = ((((double) (N-yCent)) / ((double) N)) * yOldWidth) + minY; //N-yCent to reverse it
	
	minX = centerX - (2* factor); //new minX
	minY = centerY - (1.5*factor); //new minY
	
	//printf("pixX: %d, centerX: %f, minX: %f, Factor: %f\n", xCent, centerX, minX, factor);
	//printf("pixY: %d, centerY: %f, minY: %f, Factor: %f\n", yCent, centerY, minY, factor);

	for( int x = 0 ; x < N ; x++ )
	{
		for( int y = 0 ; y < N ; y++ )
		{ 
			inFrac(20, x, y, factor);
		}
	}
}
 
void drawString( char* s )
{
   int k = 0 ;
   //
   while( s[k] != '\0' )
   {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,s[k]);
      //
      ++k ;
   }
}
/*
   GLUT_BITMAP_8_BY_13
   GLUT_BITMAP_9_BY_15
   GLUT_BITMAP_TIMES_ROMAN_10
   GLUT_BITMAP_TIMES_ROMAN_24
   GLUT_BITMAP_HELVETICA_10
   GLUT_BITMAP_HELVETICA_12
   GLUT_BITMAP_HELVETICA_18
*/

void displayfunc()
{
   //
   char stepstr[20]; // overkill... 20 digits
   //
   glClear(GL_COLOR_BUFFER_BIT); // white
   //
   int start = N / 2;
   outFrac(20, start, start, 1, 1.0);

   glutSwapBuffers(); // single buffering... call glFlush();
}
void reshapefunc(int wscr,int hscr)
{
   glViewport(0,0,(GLsizei)N,(GLsizei)N);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0,1.0*N,0.0,1.0*N); // always a square
   glMatrixMode(GL_MODELVIEW);
}
void mousefunc(int button,int state,int xscr,int yscr)
{
   if(button==GLUT_LEFT_BUTTON)
   {
      if(state==GLUT_DOWN)
      {
        char stepstr[20]; // overkill... 20 digits
		glClear(GL_COLOR_BUFFER_BIT); // white
		
		factor = factor / 2.0;
		printf("Factor %f\n", factor);
        printf("Pressed Left Mouse \n");
		outFrac(20, xscr, yscr, factor, 2.0);
		
		glutSwapBuffers(); // single buffering... call glFlush();
      }
   }
   else if(button==GLUT_RIGHT_BUTTON)
   {
      if(state==GLUT_DOWN)
      {
         srand( time(NULL) ) ;
      }
   }
}
void keyfunc(unsigned char key,int xscr,int yscr)
{
   if( key == ' ' )
   {
      pause = !pause ;
   }
   else if( key == 'q' )
   {
      exit( 0 ) ;
   }
}

int main(int argc,char* argv[])
{  
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(N,N);
   glutInitWindowPosition(100,50);
   glutCreateWindow("");
   glClearColor(1.0,1.0,1.0,0.0);
   glShadeModel(GL_SMOOTH);
   //

   glutDisplayFunc(displayfunc);
   glutReshapeFunc(reshapefunc);
   glutMouseFunc(mousefunc);
   glutKeyboardFunc(keyfunc);
   //
   glutMainLoop();
   //
   return 0;
}
// 
// end of file
// 
