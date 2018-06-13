//
// Torbert, 8 February 2016
//
#include <stdio.h>
#include <math.h>
#include <limits.h>
//
#define M 640
#define N 480

int rgb[N][M][3] ; // red-green-blue for each pixel

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

int numSpheres = 4;
sphere sphereList[4];

color grid1 = {205, 113, 63};
color grid2 = {160, 32, 240};
color defaultC = {135, 206, 250};

double zDifference = 0; //moves eye and pixel board further from the spheres (-5 is good)
double size = .1;

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

void drawPixel(int x, int y, color c)
{
	rgb[y][x][0] = c.r;
	rgb[y][x][1] = c.g;
	rgb[y][x][2] = c.b;
}

triple multiplyVect(triple a, double mult)
{
	triple t = {a.x * mult, a.y * mult, a.z * mult};
	return t;
}

triple addVect(triple a, triple b)
{
	triple t = {a.x + b.x, a.y + b.y, a.z + b.z};
	return t;
}

color findColorReflect(triple e, triple r)
{
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
		
		//make floor calculation
		triple s = {e.x + (minT * r.x), e.y + (minT * r.y), e.z + (minT * r.z)}; //sphere point
		
		if(minS == 0) //if it is the floor
		{
			//s is the point on the sphere, y can be ignored (constant)
			int tempX = s.x / size;
			int tempY = s.z / size;
			int index = tempX + tempY;
			if(index % 2 == 0) //even
				return grid1;
			else
				return grid2;	
		}
		
		return sphereList[minS].c;
	}
}


int main(void){
	triple e = { 0.50 , 0.50 , -1.00 + zDifference } ; // the eye
	triple light = { 0.00 , 1.25 , -0.50 } ; // the light
		
	//int numSpheres = 4;
	sphereList[0] = constructSphere(.5, -20000, .5, 205, 113, 63, 20000.25); //floor (Peru)
	sphereList[1] = constructSphere(.5, .8, .5, 0, 0, 255, .25); //blue (y changed from .5 to .8)
	sphereList[2] = constructSphere(1, .5, 1, 0, 255, 0, .25); //green
	sphereList[3] = constructSphere(0, .75, 1.25, 255, 0, 0, .5); //red
	//sphere sphereList[4] = {s1, s2, s3, s4};
	//
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
			double pz = zDifference;
			
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
			
			if(sphereCount == 0) //hit no sphere
			{
				drawPixel(x, y, defaultC);
			}

			else //hit at least one sphere
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
						
						else //draw default color
						{
							drawPixel(x, y, defaultC);
						}
					}
				}
				
				//draw ray from point on sphere (x = ex + (T * rx)) to the light source
				//see if any spheres in the way
				
				if(minS < numSpheres) //found minimum
				{
					sphere sph = sphereList[minS]; //hit sphere
					
					triple s = {e.x + (minT * r.x), e.y + (minT * r.y), e.z + (minT * r.z)}; //sphere point
					triple rL = calcRay(s, light); //ray between point on sphere to light
					triple n = calcNormal(s, sph.p, sph.rad); //surface normal
					
					double factor = .001;
					
					triple noiseFree = {s.x + (n.x * factor), s.y + (n.y * factor), s.z + (n.z * factor)}; //gets ride of noise
					s = noiseFree;
					
					int indexList2[4];
					int sphereCount2 = 0; 
					
					for(int sDex = 0; sDex < numSpheres; sDex++)
					{
						sphere sph2 = sphereList[sDex];
							
						double dx = s.x - sph2.p.x;
						double dy = s.y - sph2.p.y;
						double dz = s.z - sph2.p.z;
							
						double a = pow(rL.x, 2) + pow(rL.y, 2) + pow(rL.z, 2);
						double b = 2*(dx * rL.x + dy * rL.y + dz * rL.z);
						double c = pow(dx, 2) + pow(dy, 2) + pow(dz, 2) - pow(sph2.rad, 2);
							
						if(pow(b, 2) - (4 * a * c) > 0) //discriminate
								if(minQuad(a, b, c) > 0) //makes sure quadratic formula is positive
									sphereCount2++;
					}
					
					color c = sph.c;
					double colorFactor = .5;
					color reflect;
					
					if(minS == 0) //if it is the floor
					{
						//s is the point on the sphere, y can be ignored (constant)
						int tempX = s.x / size;
						int tempY = s.z / size;
						int index = tempX + tempY;
						if(index % 2 == 0) //even
							c = grid1;
						else
							c = grid2;	
					}
					
					if(minS == 1) //if blue sphere (temporary reflection)
					{
						double mult = -2 * dotProduct(r, n); //r is original ray, n is normal
						triple trip = multiplyVect(n, mult);
						trip = addVect(r, trip); 
						
						reflect = findColorReflect(s, trip); //s being point on sphere
						//MIGHT NEED TO TAKE BACKGROUND COLOR INTO ACCOUNT
					}
					
					else
					{
						colorFactor = 1; //resets color factor so reflection not taken into account
					}
					
					if(sphereCount2 == 0) //did not hit sphere
					{
						//Lambert's Law to calculate how much shadow there should be
						double mag1 = calcMagnitude(rL);
						double mag2 = calcMagnitude(n);
						double dot = dotProduct(n, rL);
						double cosVal = dot / (mag1 * mag2);
						
						if(cosVal < 0) //no negative numbers
							cosVal = 0;
							
						color tempC = {(colorFactor * ((c.r / 2) + ((c.r / 2) * cosVal))) + ((1-colorFactor) * reflect.r), 
							(colorFactor * ((c.g / 2) + ((c.g / 2) * cosVal))) + ((1-colorFactor) * reflect.g), 
							(colorFactor * ((c.b / 2) + ((c.b / 2) * cosVal))) + ((1-colorFactor) * reflect.b)};
						drawPixel(x, y, tempC);
					}
					
					else //hit another sphere
					{
						color tempC = {(colorFactor * c.r) + ((1-colorFactor) * reflect.r), (colorFactor * c.g) + ((1-colorFactor) * reflect.g), 
							(colorFactor * c.b) + ((1-colorFactor) * reflect.b)};
						drawPixel(x, y, tempC);
					}
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
