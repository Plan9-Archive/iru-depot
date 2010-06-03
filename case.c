#include <stdio.h>

/* simple case convertion functions */

char
xtoupper(const char c)
{
	if(c < 'a' || c > 'z')
		return c;
	return(c - 32);
}

char
xtolower(const char c)
{
	if(c < 'A' || c > 'Z')
		return c;
	return(c + 32);
}

/*int
main(void)
{
	char c;

	for(c='a'; c <= 'z'; c++)
		printf("%c ", xtoupper(c));
	printf("\n");

	for(c='A'; c <= 'Z'; c++)
		printf("%c ", xtolower(c));
	printf("\n");

	return 0;
}*/
