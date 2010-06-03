#include <stdio.h>
#include <stdlib.h>

int
find(ushort elem, ushort *v, uint size)
{
	ushort *p;
	p = v;
	while(p != v+size)
		if(*p++ == elem)
			return 1; 
	return 0;
}

int
main(void)
{
	ushort x[15], *p, d;
	srand(time(NULL));	
	p = x;
	d = sizeof(x)/sizeof(*x); 
	while(p != x+d){
		ushort n;
		do{
			n = rand()%25;
		}while(find(n, x, d));
		*p = n;
		printf("%u\n", *p++);
	}
	return 0;
} 

