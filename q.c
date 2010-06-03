/* draw a square of l*l units
 * Iruata Souza
 * Public Domain
 */
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
	uint l, i, c;
	
	if (argc < 2) {
		fprintf(stderr, "usage: %s size [unit]\n", argv[0]);
		exit(1);
	}

	l = atoi(argv[1]);
	if (l == 0)
		exit(1);
	l--;

	if (argc == 3)
		c = *argv[2];
	else
		c = '*';

	for (i = 0; i <= l; i++) {
		uint n = 0;

	/*	if (i == 0 || i == l)
			while (n++ <= l) 
				fputc(c, stdout);
		else  
			while (n <= l) {
				fputc((n == 0 || n == l)?c:' ', stdout);
				n++;
			} */
		while (n <= l) {
			fputc(c, stdout);
			n++;
		}
		printf("\n"); 

	}
	return 0;
}	
