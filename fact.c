#include <stdio.h>
#include <stdlib.h>

int
factiter(int n)
{
	int i, x; 

	x = 1;
	for(i = 1; i <= n; i++) 
		x = x*i;
	return x;
}

int
factrec(int n)
{
	if(n <= 1)
		return 1;
	return n*factrec(n-1);
}

int
factreciter(int i, int n, int result)
{
	if(i > n)
		return result;
	return factreciter(i+1, n, i*result);
}

int
main(int argc, char *argv[])
{
	int n;
	if(argc < 2){
		fprintf(stderr, "usage: fact n\n");
		exit(1);
	}

	n = atoi(argv[1]);
	printf("%d %d %d\n", factiter(n), factrec(n), factreciter(1, n, 1));
	return 0;
}
