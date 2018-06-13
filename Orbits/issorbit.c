#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define R 6371000 //m
#define RM 1738000 //m
#define G 6.67408e-11
#define M 5.97219e24 //kg
#define MOONM 7.34767309e22
#define PI 3.14159265

double x[1000000];
double y[1000000];
double vx[1000000];
double vy[1000000];
double x2[1000000];//2 signifies apollo
double y2[1000000];
double vx2[1000000];
double vy2[1000000];

void moon(double mins, double dt, double theta)
{
	int totalTime = (mins * 60 * (1/dt));
	
	double initialVal = sqrt(G*M/384400000);
	//moon
	//y[0] = 0;
	//x[0] = 384400000; // Moon distance in m
	x[0] = 383876712.8;
	y[0] = 20051178.65;
	vx[0] = 0; //circular
	vy[0] = initialVal; //m/s
	
	//apollo should be 20 not 35
	double alt = 200000000; //altitude 
	double initialV = 1500;
	double val = PI / 180.0;
	x2[0] = alt * cos(val * theta);
	y2[0] = alt * sin(val * theta);
	vx2[0] = initialV * cos(val * theta);
	vy2[0] = initialV * sin(val * theta); 
	
	printf("Moon x\tMoon y\tApollo x\tApollo y\tt\tvmag\tradM\n");
	for(int i = 1; i < totalTime; i++)
	{
		//MOON
		x[i] = x[i-1] + vx[i-1] * dt;
		y[i] = y[i-1] + vy[i-1] * dt;
		
		double rad = sqrt(pow(x[i], 2) + pow(y[i], 2));
		double a = -1 * (G*M)/ pow(rad, 2.0); 
		
		double ax = a * (x[i] / rad);
		double ay = a * (y[i] / rad);
		
		vx[i] = vx[i-1] + ax * dt; //changing speed of the moon
		vy[i] = vy[i-1] + ay * dt;
		
		//APOLLO
		
		x2[i] = x2[i-1] + vx2[i-1] * dt;
		y2[i] = y2[i-1] + vy2[i-1] * dt;
		
		double radE = sqrt(pow(x2[i], 2) + pow(y2[i], 2)); //dist from Earth
		double xDist = x[i-1] - x2[i-1]; //x distance between moon and apollo
		double yDist = y[i-1] - y2[i-1];
		double radM = sqrt(pow(xDist, 2) + pow(yDist, 2)); //dist between M and apollo
		
		double aE = (G*M)/ pow(radE, 2.0); //accel due to Earth
		double aM = (G*MOONM)/ pow(radM, 2.0); //accel due to Moon
	
		double ax2 = ((-1*aE * (x2[i] / radE)) + (aM * (xDist / radM)));
		double ay2 = ((-1*aE * (y2[i] / radE)) + (aM * (yDist / radM))); 
		
		vx2[i] = vx2[i-1] + ax2 * dt; //changing speed of apollo
		vy2[i] = vy2[i-1] + ay2 * dt;
		
		double vmag = sqrt(pow(vx2[i], 2) + pow(vy2[i], 2));
		double time = dt * i;
		
		//if(radM < RM) //it hits the moon;
		//{
			//printf("HIT MOON \n");
			//printf("Theta %f\n", theta);
			//break;
		//}
		
		if(i % 100 == 0)
			printf("%f\t%f\t%f\t%f\t%f\t%f\t%f\n", x[i], y[i], x2[i], y2[i], time, vmag, radM);
	}
}


void euler(double mins, double dt)
{
	int length = mins * 60 * (1/dt);
	double x[length];
	double y[length];
	double vx[length];
	double vy[length];
	
	double initialVal = sqrt(G*M/R);
	x[0] = 0;
	y[0] = R + 400000;
	//vx[0] = 1.2 * initialVal; //elliptical
	vx[0] = initialVal; //circular
	//vx[0] = 1.5 * 7700; //hyperbolic
	vy[0] = 0;
	
	printf("x\ty\tt\tvx\tvy\tvmag\tr\n");
	for(int i = 1; i < length; i++)
	{
		x[i] = x[i-1] + vx[i-1] * dt;
		y[i] = y[i-1] + vy[i-1] * dt;
		
		double rad = sqrt(pow(x[i], 2) + pow(y[i], 2));
		double a = -1 * (G*M)/ pow(rad, 2.0);
		
		double ax = a * (x[i] / rad);
		double ay = a * (y[i] / rad);
		
		vx[i] = vx[i-1] + ax * dt;
		vy[i] = vy[i-1] + ay * dt; //seems kinda inconsistent idk
		
		double vmag = sqrt(pow(vx[i], 2) + pow(vy[i], 2));
		double time = dt * i;
		printf("%f\t%f\t%f\t%f\t%f\t%f\t%f\n", x[i], y[i], time, vx[i], vy[i], vmag, rad);
	}
}

int main( int argc , char* argv[] )
{
	double dt = 1;
	double mins = 3.5 * 24 * 60; //moon
	//double mins = 90; //circle
	//double mins = (60 * 12); //ellipse
	//double mins = (60 * 2.5); //hyperbola
	//for(double theta = 0; theta < 90; theta = theta + .1)
		//moon(mins, dt, theta);
	//double theta = 36.11;
	double theta = 30.8;
	moon(mins, dt, theta);
	
	return 0;
}
