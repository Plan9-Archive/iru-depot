/* Simulate random walks */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double
randin(double inf, double sup)
{
	return inf + (sup-inf)*((double)rand() / (double)(RAND_MAX + 1.0));
}

double
walk1(int Npassos)
{
	double x;
	int i;

	x = 0;
	for(i = 0; i < Npassos; i++){
		if(randin(0,1) < 0.5)
			x = x + 1;
		else
			x = x - 1;
	}
	return x;
}

double
walks(double Npassos, int Nwalk)
{
	int i;
	double xm, x;
	xm = 0;

	for(i = 0; i < Nwalk; i++){
		x = walk1(Npassos);
		xm = xm + x;
		xm = xm + x*x;
	}
	return xm/Nwalk;
}

void
simul(double minP, double maxP)
{
	int Npassos;

	for(Npassos = minP; Npassos <= maxP; Npassos++)
		printf("%d %g\n", Npassos, walks(Npassos, 1000));
}

int
main(void)
{
	int Npassos;

	srand(time(NULL));
	simul(1, 100);
	return 0;
}
