/* Convert number to its representation on a 5 position LED screen */

#include <u.h>
#include <libc.h>

/* 
 * There is 5 visual positions for each number. The number of bits in each position is 1 2 1 2 1. 
 */

/* Positions from top (n1) to bottom (n5) are numbered from least significant to most significant bit */
#define tobits(n1, n2, n3, n4, n5)  ((n1) | ((n2) << 1) | ((n3) << 3) | ((n4) << 4) | ((n5) << 6))

char digits[] = {
	tobits(1,3,0,3,1),
	tobits(0,1,0,1,0),
	tobits(1,1,1,2,1),
	tobits(1,1,1,1,1),
	tobits(0,3,1,1,0),
	tobits(1,2,1,1,1),
	tobits(1,2,1,3,1),
	tobits(1,1,0,1,0),
	tobits(1,3,1,3,1),
	tobits(1,3,1,1,1)
};

int
n2bits(int n)
{
	int x, shift;

	x = 0;
	shift = 0;

	for(;;){
		x |= digits[n%10] << shift;
		n /= 10;
		if(n == 0)
			break;
		shift += 8;
	}
	return x;
}

void
main(int argc, char *argv[])
{
	if(argc < 2)
		sysfatal("usage: num n");
	print("%x\n", n2bits(atoi(argv[1])));
}
