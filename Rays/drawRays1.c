//
// Torbert, 8 February 2016
//
#include <stdio.h>
#include <math.h>
#include <limits.h>
//
#define M 640
#define N 480
//

typedef struct
{
   double x ;
   double y ;
   double z ;
   //
} triple ;

typedef struct
{
	int r;
	int g;
	int b;
	
} color ;

typedef struct
{
	triple p;
	color c;
	double rad;
	
} sphere;

triple e = { 0.50 , 0.50 , -1.00 } ; // the eye
triple light = { 0.00 , 1.25 , -0.50 } ; // the light

double calcMagnitude(triple p)
{
	return sqrt(pow(p.x, 2) + pow(p.y, 2) + pow(p.z, 2)); 
}

triple calcRay(triple e, triple p)
{
	triple temp = {p.x - e.x, p.y - e.y, p.z - e.z};
	
	double rmag = calcMagnitude(temp);
	
	triple r = {temp.x/rmag, temp.y/rmag, temp.z/rmag} ;
	return r;
}

sphere constructSphere(double x, double y, double z, int r, int g, int b, double rad)
{
	triple p = {x, y, z};
	color c = {r, g, b};
	sphere s = {p, c, rad};
	return s;
}

double minQuad(double a, double b, double c)
{
	double quad1 = (-b + sqrt(pow(b, 2) - (4*a*c))) / (2*a);
	double quad2 = (-b - sqrt(pow(b, 2) - (4*a*c))) / (2*a);
						
	if(quad1 < quad2) //finds minimum of quadratic formula
		return quad1;
	else
		return quad2;
}

triple calcNormal(triple p, triple c, double rad)
{
	double nx = (p.x - c.x) / rad;
	double ny = (p.y - c.y) / rad;
	double nz = (p.z - c.z) / rad;
	
	triple n = {nx, ny, nz};
	return n;
}

double dotProduct(triple a, triple b)
{
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}


int main(void){
	int numSpheres = 4;
	sphere s1 = constructSphere(.5, -20000, .5, 205, 113, 63, 20000.25); //floor (Peru)
	sphere s2 = constructSphere(.5, .5, .5, 0, 0, 255, .25); //blue
	sphere s3 = constructSphere(1, .5, 1, 0, 255, 0, .25); //green
	sphere s4 = constructSphere(0, .75, 1.25, 255, 0, 0, .5); //red
	sphere sphereList[4] = {s1, s2, s3, s4};
	
	int rgb[N][M][3] ; // red-green-blue for each pixel
	//
	int y , x ;
	//
	FILE* fout ;
	//
	for( y = 0 ; y < N ; y++ )
	{
		for( x = 0 ; x < M ; x++)
		{	
			double px = (x + .5) / N; //+.5 to make it center
			double py = 1- ((y + .5) / N); //1- to flip y
			double pz = 0;
			
			triple p = {px, py, pz};
			triple r = calcRay(e, p);
			
			int indexList[4];
			int sphereCount = 0;
			int lastDex = 0;
			
			for(int sDex = 0; sDex < numSpheres; sDex++)
			{
				sphere sph = sphereList[sDex];
				
				double dx = e.x - sph.p.x;
				double dy = e.y - sph.p.y;
				double dz = e.z - sph.p.z;
				
				double a = 1;
				double b = 2*(dx * r.x + dy * r.y + dz * r.z);
				double c = pow(dx, 2) + pow(dy, 2) + pow(dz, 2) - pow(sph.rad, 2);
				
				if(pow(b, 2) - (4 * a * c) > 0) //discriminate
				{
					sphereCount++;
					lastDex = sDex;
					indexList[sDex] = 1; //marks sphere as hit
				}
				else
					indexList[sDex] = 0;
			}

			if(sphereCount > 0) //hit at least one sphere
			{
				double minT = INFINITY;
				int minS = numSpheres;
				
				for(int sDex = 0; sDex < numSpheres; sDex++) //finds minimum T of possible spheres
				{
					if(indexList[sDex] == 1) //if found that it hit above
					{
						sphere sph = sphereList[sDex];
						
						double dx = e.x - sph.p.x;
						double dy = e.y - sph.p.y;
						double dz = e.z - sph.p.z;
						
						double a = 1;
						double b = 2*(dx * r.x + dy * r.y + dz * r.z);
						double c = pow(dx, 2) + pow(dy, 2) + pow(dz, 2) - pow(sph.rad, 2);
						
						double quad = minQuad(a, b, c);
							
						if(quad >= 0 && quad < minT) //excludes negative numbers
						{
							minT = quad;
							minS = sDex;
						}
					}
				}
				
				if(minS < numSpheres) //found minimum
				{
					sphere sph = sphereList[minS]; //hit sphere
					
					rgb[y][x][0] = sph.c.r; 
					rgb[y][x][1] = sph.c.g;
					rgb[y][x][2] = sph.c.b;
				}
			}
			
		}
	}
	
	
	//
	//
	//
	fout = fopen( "output.ppm" , "w" ) ;
	//
	fprintf( fout , "P3\n" ) ;
	fprintf( fout , "%d %d\n" , M , N ) ;
	fprintf( fout , "255\n" ) ;
	//
	for( y = 0 ; y < N ; y++ )
	{
		for( x = 0 ; x < M ; x++)
		{
			fprintf( fout , "%d %d %d\n" ,
			rgb[y][x][0] , rgb[y][x][1] , rgb[y][x][2] ) ;
		}
	}
	fclose( fout ) ;
	//
	return 0 ;
}
//
// end of file
//
