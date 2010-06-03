/* string to integer convertion */
#include <stdio.h>
#include <math.h>

/* XXX this is very inefficient code */
int
main(int argc, char *argv[])
{
	unsigned long long n;
	int i, len;
	char c;

	if(argc < 2)
		exit(1);

	n=0;
	len = strlen(argv[1]);

	for(i=len; i > 0; i--){
		char c = argv[1][i-1];
		n +=  (c - 48) * pow(10, len-i);
	}

	printf("%d\n", n);

	return 0;
}
