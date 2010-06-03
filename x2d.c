#include <ctype.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* x2d :  
 * 
 * 0 x 1 f 0
 *     | | |___ 0	*16	^0	= 0
 *     | |_____ 15	*16	^1	= 240  
 *     |_______ 1	*16	^2	= 256
 *     					= 496			
 */

int
main(int argc, char *argv[])
{
	int             n, size;
	n = 0;
	
	if (argc < 2)
		errx(1, "%s hex", argv[0]);
	
	size = strlen(argv[1]) - 1;

	while (*argv[1]) {
		/* XXX: don't check for invalid chars
		 * 16^n = 2^4n 
		 */
		switch (toupper(*argv[1])) {
		case 'A':
			n += 10 * (1 << (4*size));
			break;
		case 'B':
			n += 11 * (1 << (4*size));
			break;
		case 'C':
			n += 12 * (1 << (4*size));
			break;
		case 'D':
			n += 13 * (1 << (4*size));
			break;
		case 'E':
			n += 14 * (1 << (4*size));
			break;
		case 'F':
			n += 15 * (1 << (4*size));
			break;
		default:
			n += ((*argv[1]) - '0') * (1 << (4*size));
		}

		argv[1]++;
		size--;
	}
	printf("%d\n", n);
	return 0;
}
