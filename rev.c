/* store a reversed list */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct list list;
struct list {
	char *name;
	list *next;
};

int
main(void)
{
	list *l, *lp;
	char buf[24][10];
	int n, i;
	
	l = malloc(sizeof(list));
	l->next = NULL;
	for(i=0; i<10; i++){
		list *aux;

		n = read(0, buf[i], 24);
		if(n <= 0)
			break;
		buf[i][n]=0;

		aux = malloc(sizeof(list));
		aux->name = buf[i];
		aux->next = l;
		l = aux;
	}
	
	for(lp = l;lp->next; lp = lp->next)
		printf("%s", lp->name);
	return 0;
}
