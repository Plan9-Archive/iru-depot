/* List of strings */

#include <stdio.h>
#include <stdlib.h>

typedef struct SList SList;
struct SList {
	unsigned long	len;
	char			*str;
	SList			*link;
};

SList*
elem(char *s, unsigned long len)
{
	SList *lp;

	lp = malloc(sizeof(SList));
	lp->str = s;
	lp->len = len;
	lp->link = NULL;
	return lp;
}

void
pelem(SList *lp)
{
	printf("%lu : %.*s -> %lu", (unsigned long)lp, (int)lp->len, lp->str, (unsigned long)lp->link);
}

/* Print list */
void
plist(SList *lp)
{
	SList *p;

	for(p = lp; p; p = p->link){
		pelem(p);
		putchar('\n');
	}
}

void
addtail(SList *lp, SList *ep)
{
	SList *p;

	for(p = lp; p->link; p = p->link)
		;
	p->link = ep;
}

SList*
test(void)
{
	SList *lp;

	lp = elem("first", 5);
	addtail(lp, elem("second", 6));
	addtail(lp, elem("third", 5));
	return lp;
}

int
main(void)
{
	plist(test());
	return 0;
}

	
