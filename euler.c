#include <stdio.h>
#include <string.h>
#include <math.h>

double
euler(double (*Df)(double, double), double xi, double yi, double h)
{
	return yi + h*Df(xi, yi);
}

double
parb(double x, double y)
{
	return exp(x);
}

int
main(void)
{
	double x, y, yi, h;

	x = 0;
	y = 1;
	h = 0.05;
	
	do{
		yi = y;
		y = euler(parb, x, yi, h);
		x += h;
		printf("df(%g)\t\t%g\t\tdiff:%g\n", x, y, y-yi);
	}while(fabs(y-yi) > 1e-9);
	return 0;
}
	