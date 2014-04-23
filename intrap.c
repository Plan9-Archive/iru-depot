/*
 * Numeric integration by the trapezoid rule.
 * Practical reference: 
 * http://www.if.ufrj.br/~sandra/MetComp/2012-2/Aula8.pdf
 *
 */
#include <stdio.h>
#include <math.h>

double f(double x){ return x*x*x; }

int
main(void)
{
	double a, b, T, Tk, sum, dx_k, eps;
	int k, j;

	printf("a b eps: ");
	scanf("%lf %lf %lf", &a, &b, &eps);

	k = 1;
	Tk = (f(a) + f(b))*(b-a)/2;

	do{
		T = Tk;
		sum = 0;
		dx_k = (b-a)/pow(2,k);

		for(j = 1; j <= pow(2,k)-1; j = j+2)
			sum = sum + f(a + j*dx_k);
		Tk = T/2. + dx_k*sum;
		k = k+1;
	}while(fabs(Tk - T) > eps);

	printf("%d %g\n", k-1, Tk);
	return 0;
}

		