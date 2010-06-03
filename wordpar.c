/* draw a parallelogram using the word argument */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char *argv[])
{
	int i, j, sz, v;

	if (argc < 2)
		errx(1, "%s word", argv[0]);

	sz = strlen(argv[1]);

	for (i = 0, j = sz ; i < (2 * sz) + 1 && j >= 0; i += 2, j--) {
		for (v = 0; v < sz * j; v++)
			printf(" ");
		printf("%s", argv[1]);
		for (v = 0; v < sz * i; v++)
			printf(" ");
		printf("%s", argv[1]);
		printf("\n");
	}

	for (i = 1, j = 2; i <= sz ; i++, j += 2) {
		for (v = 0; v < i * sz; v++)
			printf(" ");
		printf("%s", argv[1]);

		for (v = 0; v < sz * sz + ((sz - j) * sz); v++)
			printf(" ");
		printf("%s", argv[1]);

		printf("\n");
	}

	printf("\n");

	return 0;
}
