/*
 * Numeric integration approximating the area by rectangles.
 *
 */

#include <stdio.h>
#include <math.h>

double f(double x){ return x*x*x + 2*x*x + 3; }

int
main(void)
{
	double a, b, T, Tk, sum, dx, eps;
	int k, i;

	printf("a b eps: ");
	scanf("%lf %lf %lf", &a, &b, &eps);

	k = 1;
	Tk = 0;
	do{
		T = Tk;
		Tk = 0;
		dx = (b-a)/k;

		for(i = 0; i < k; i++)
			Tk = Tk + f(a + (i+1/2.)*dx)*dx;
		k++;
	}while(fabs(Tk - T) > eps);

	printf("%d %.20g\n", k-1, Tk);
	return 0;
}
