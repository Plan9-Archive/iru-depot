/* Pascal/Yang Hui's Triangle
 * Iruata Souza (muzgo)
 * Public Domain
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

int 
fact(uint x) 
{
	if (x)
		return(x * fact(x-1));
	return 1; 
} 

int
bin(uint n, uint k)
{
	return(fact(n) / (fact(k) * (fact(n-k))));
}

int
main(int argc, char *argv[])
{
	int n, i, j;
	
	if (argc < 2)
		errx(1, "%s n", argv[0]); 
	
	n = atoi(argv[1]);

	if (n > 12)
		errx(1, "n too big!");

	for (i = 0; i <= n; i++)
		for (j = 0; j <= i; j++) 
 			printf("%d\t%c", bin(i,j), (j == i)?'\n':' '); 

	return 0;
}
