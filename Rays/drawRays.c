//
// Torbert, 8 February 2016
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <ctype.h>
#include <time.h>
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



int n = 5000;
int numSpheres = 4 + 5000;
sphere sphereList[5004];

color grid1 = {205, 113, 63};
color grid2 = {160, 32, 240};
color defaultC = {135, 206, 250};
triple light = { 0.00 , 1.25 , -0.50 } ; // the light

double zDifference = 0; //moves eye and pixel board further from the spheres (-5 is good)
double size = .1;

color fileM[707][600];

//calculates magnitude of a triple
double calcMagnitude(triple p)
{
	return sqrt(pow(p.x, 2) + pow(p.y, 2) + pow(p.z, 2)); 
}

//finds normalized ray between two triples
triple calcRay(triple e, triple p)
{
	triple temp = {p.x - e.x, p.y - e.y, p.z - e.z};
	
	double rmag = calcMagnitude(temp);
	
	triple r = {temp.x/rmag, temp.y/rmag, temp.z/rmag} ;
	return r;
}

//creates sphere from individual numbers
sphere constructSphere(double x, double y, double z, int r, int g, int b, double rad)
{
	triple p = {x, y, z};
	color c = {r, g, b};
	sphere s = {p, c, rad};
	return s;
}

//finds minimum of quadratic formula
double minQuad(triple vars)
{
	double a = vars.x;
	double b = vars.y;
	double c = vars.z;
	
	double quad1 = (-b + sqrt(pow(b, 2) - (4*a*c))) / (2*a);
	double quad2 = (-b - sqrt(pow(b, 2) - (4*a*c))) / (2*a);
						
	if(quad1 < quad2) 
		return quad1;
	else
		return quad2;
}

//returns normal vector
triple calcNormal(triple p, triple c, double rad)
{
	double nx = (p.x - c.x) / rad;
	double ny = (p.y - c.y) / rad;
	double nz = (p.z - c.z) / rad;
	
	triple n = {nx, ny, nz};
	return n;
}

//returns dot product of triple a and b
double dotProduct(triple a, triple b)
{
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

//draws color c and point (x,y)
void drawPixel(int x, int y, color c)
{
	rgb[y][x][0] = c.r;
	rgb[y][x][1] = c.g;
	rgb[y][x][2] = c.b;
}

//multiplies triple by a factor
triple multiplyVect(triple a, double mult)
{
	triple t = {a.x * mult, a.y * mult, a.z * mult};
	return t;
}

//adds tripples together
triple addVect(triple a, triple b)
{
	triple t = {a.x + b.x, a.y + b.y, a.z + b.z};
	return t;
}

//finds a, b, and c for quadratic formula, returns as triple
triple quadraticVariables(triple e, triple r, sphere s)
{
	double dx = e.x - s.p.x;
	double dy = e.y - s.p.y;
	double dz = e.z - s.p.z;
	
	double a = 1;
	double b = 2*(dx * r.x + dy * r.y + dz * r.z);
	double c = pow(dx, 2) + pow(dy, 2) + pow(dz, 2) - pow(s.rad, 2);
	triple vars = {a, b, c};
	return vars;
}

int calcInput(FILE* input)
{
	char tempList[10];
	int index = 0;
	char c = getc(input);
	if(isspace(c))
		c = getc(input); //if initial space, move over one
	while(!isspace(c))
	{
		tempList[index] = c;
		index++; 
		c = getc(input);
	}
					
	int sum = 0;
	for(int x = 0; x < index; x++)
	{
		int i = tempList[x] - '0';
		sum += i * (pow(10, (index - x - 1)));
	}
	
	return sum;
}

color findColorReflect(triple e, triple r, double colorFactor, int width, int length)
{
	int indexList[5004];
	int sphereCount = 0;
	int lastDex = 0;
			
	for(int sDex = 0; sDex < numSpheres; sDex++)
	{	
		//printf("sDex: %d, numSpheres: %d\n", sDex, numSpheres);
		sphere sph = sphereList[sDex];
		
		double dx = e.x - sph.p.x;
		double dy = e.y - sph.p.y;
		double dz = e.z - sph.p.z;
		
		double a = 1;
		double b = 2*(dx * r.x + dy * r.y + dz * r.z);
		double c = pow(dx, 2) + pow(dy, 2) + pow(dz, 2) - pow(sph.rad, 2);
		
		/*printf("pow: %f\n", pow(b, 2));
		printf("mult: %f\n", (4 * a * c));
		printf("subtract: %f\n", pow(b, 2) - (4 * a * c));*/
		if(pow(b, 2) - (4 * a * c) > 0) //discriminate
		{
			sphereCount++;
			lastDex = sDex;
			indexList[sDex] = 1; //marks sphere as hit
		}
		
		else
			indexList[sDex] = 0;
	}
	
	if(sphereCount == 0) //if it hit no spheres, display sky
	{
		return defaultC;
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
				
				triple vars = quadraticVariables(e, r, sph);
							
				double quad = minQuad(vars);
							
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
					
			triple s = {e.x + (minT * r.x), e.y + (minT * r.y), e.z + (minT * r.z)}; //sphere point
			triple rL = calcRay(s, light); //ray between point on sphere to light
			triple n = calcNormal(s, sph.p, sph.rad); //surface normal
					
			double factor = .001;
					
			triple noiseFree = {s.x + (n.x * factor), s.y + (n.y * factor), s.z + (n.z * factor)}; //gets ride of noise
			s = noiseFree;
					
			int indexList2[5004];
			int sphereCount2 = 0; 
					
			for(int sDex = 0; sDex < numSpheres; sDex++)
			{
				sphere sph2 = sphereList[sDex];
				
				triple vars = quadraticVariables(s, rL, sph2);
							
				if(pow(vars.y, 2) - (4 * vars.x * vars.z) > 0) //discriminate
						if(minQuad(vars) > 0) //makes sure quadratic formula is positive
							sphereCount2++;
			}
					
			color sphereColor = sph.c;
		
			if(minS == 0) //if it is the floor
			{
				//s is the point on the sphere, y can be ignored (constant)
				int tempX = s.x / size;
				int tempZ = s.z / size;
				int index = tempX + tempZ;
				if(index % 2 == 0) //even
					sphereColor = grid1;
				else
					sphereColor = grid2;	
			}
			
			if(minS == 1) //makes the first sphere into earth
			{
				double dx = s.x - sph.p.x;
				double dy = s.y - sph.p.y;
				double dz = s.z - sph.p.z;
				
				//arc cos, rows
				//printf("rad %f, dy %f \n", sph.rad, dy);
				double latit = acos(dy/sph.rad) * (180.0 / M_PI); //in degrees
				if(isnan(latit))
					latit = 0;
				
				//arc tan, takes into account that dx might be 0, cols
				double longit = atan2(dz, dx) * (180.0 / M_PI); //in degrees
				if(longit < 0)
					longit += 360; //makes it positive;
				
				//printf("latitude: %f, longitude %f \n", latit, longit);

				int rowNum = (latit / 180.0) * length;
				int colNum = (longit / 360.0) * width;
				//printf("rowNum %d colNum %d \n", rowNum, colNum);
				
				sphereColor = fileM[rowNum][colNum];
				colorFactor = 1; //stops the reflect from applying if it hits Earth
			}
			
			color reflect;
			if(colorFactor < 1) //only does this if reflection turned on
			{
				double mult = -2 * dotProduct(r, n); //r is original ray, n is normal
				triple trip = multiplyVect(n, mult);
				trip = addVect(r, trip); 
				
				reflect = findColorReflect(s, trip, colorFactor, width, length); //s being point on sphere, finds reflecting colors
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
				
				color tempC = {(colorFactor * ((sphereColor.r / 2) + ((sphereColor.r / 2) * cosVal))) + ((1-colorFactor) * reflect.r), 
					(colorFactor * ((sphereColor.g / 2) + ((sphereColor.g / 2) * cosVal))) + ((1-colorFactor) * reflect.g), 
					(colorFactor * ((sphereColor.b / 2) + ((sphereColor.b / 2) * cosVal))) + ((1-colorFactor) * reflect.b)};
					
				return tempC;
			}
					
			else //hit another sphere
			{
				color tempC = {(colorFactor * sphereColor.r / 2) + ((1-colorFactor) * reflect.r), 
					(colorFactor * sphereColor.g / 2) + ((1-colorFactor) * reflect.g), 
					(colorFactor * sphereColor.b / 2) + ((1-colorFactor) * reflect.b)};

				return tempC;
			}
		}
		
		else //no minimum, draw default color
			return defaultC;
	}
}


int main(void){
	srand(time(NULL));
	triple e = { 0.50 , 0.50 , -1.00 + zDifference } ; // the eye

	sphereList[0] = constructSphere(.5, -20000, .5, 205, 113, 63, 20000.25); //floor (Peru)
	sphereList[1] = constructSphere(.5, .8, .5, 0, 0, 255, .25); //blue (y changed from .5 to .8)
	sphereList[2] = constructSphere(1, .5, 1, 0, 255, 0, .25); //green
	sphereList[3] = constructSphere(0, .75, 1.25, 255, 0, 0, .5); //red
	
	FILE*  fin;
	double xh,yh,zh,r;
	int    j;
	//
	fin=fopen("helix.txt","r");
	//
	for(j=0;j<n;j++)
	{
		fscanf( fin , "%lf %lf %lf %lf" , &xh,&yh,&zh,&r ) ;
		//
		sphereList[j+4] = constructSphere(xh, yh, zh, 192, 192, 192, r);
	}
	//
	fclose(fin);
	//
	//
	int y , x ;
	//
	FILE* fout ;
	//
	
	FILE * input = fopen("mercator82.ppm", "r");
	for(int x = 0; x < 3; x++)
		getc(input); //first three chars, discared
					
	int width = calcInput(input);
	int length = calcInput(input);
					
	for(int x = 0; x < 4; x++)
		getc(input); //next 4 chars, discared
	
	//fill matrix of color values from PPM
	for(int r = 0; r < length; r++)
	{
		for(int c = 0; c < width; c++)
		{
			color temp = {calcInput(input), calcInput(input), calcInput(input)};
			fileM[r][c] = temp;
		}
	}
	
	fclose(input);
	
	for( y = 0 ; y < N ; y++ )
	{
		//line to induce parallel on openMP
		#pragma omp parallel for private(x)
		for( x = 0 ; x < M ; x++)
		{	
			double px = (x + .5) / N; //+.5 to make it center
			double py = 1- ((y + .5) / N); //1- to flip y
			double pz = zDifference;
			
			triple p = {px, py, pz};
			triple r = calcRay(e, p);
			
			double colorFactor = .5; //reflection factor, 1 = off
			color finalColor = findColorReflect(e, r, colorFactor, width, length);
			
			drawPixel(x, y, finalColor);
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
