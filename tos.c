#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
tos(int n, char *str)
{
	int p, i;
	char *s;
	
	s = (char *)malloc(strlen(str)+1);
	
	p = 1;
	i = -1;
	
	while(n > 0){
		int aux, tenp;
		tenp = p * 10;
		aux = n % tenp; 
		n /= tenp;
		s[++i] = aux + '0';
	}
	
	/* str = reverse(s) */
	while(i >= 0) 
		*str++ = s[i--];
}

int
main(int argc, char *argv[])
{
	char s[9] = {};

	if(argc < 2)
		exit(1);

	tos(atoi(argv[1]), s);
	printf("%s [%lu chars]\n", s, strlen(s));

	return 0;
}
