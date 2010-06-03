#include <err.h>
#include <stdio.h>
#include <string.h>

#define spc(x) { \
	int i;\
	for(i=0; i<x; i++)\
		printf(" ");\
}

int
main(int argc, char *argv[])
{
	int i, j, sz, n;

	if (argc < 2)
		errx(1, "%s str", argv[0]);

	sz = strlen(argv[1]);
	printf("85 / sz = %d\n", 85 / (sz + sz));

	for (;;) {
		for (n = 0; n < 85 / (sz + sz); n += sz) {
			/* print sequence */
			for (i = 0; i < sz; i++) {
				spc(n);
				/* print word */
				for (j = 0; j < sz; j++) {
					printf("%c", (i == j) ? xtoupper(argv[1][j]) : argv[1][j]);
				}
			}
		}
	}

	return 0;
}
