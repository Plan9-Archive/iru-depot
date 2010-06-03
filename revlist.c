/* list reversing */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct List List;
struct List
{
	int d;
	List *link;
};

void*
emalloc(long n)
{
	void *v;
	v = malloc(n);
	if(!v){
		fprintf(stderr, "out of memory\n");
		exit(1);
	}
	return v;
}

List*
alloclist(int d)
{
	List *l;
	
	l = emalloc(sizeof(List));
	l->link = NULL;
	l->d = d;
	return l;
}

void
printlist(List *l)
{
	List *p;
	
	for(p=l; p; p = p->link)
		printf("%d ", p->d);
	printf("\n");
}


void
addtail(List *l, List *elem)
{
	List *p;
	
	for(p=l; p->link; p = p->link)
		;
	p->link = elem;
}

void
reverse(List **l)
{
	List *new, *p;
	
	new = NULL;
	while(*l){
		/* delete head from old list */
		p = *l;
		*l = (*l)->link;
		
		/* insert element in new list's head */
		p->link = new;
		new = p;
	}
	*l = new;
}


int
main(int argc, char *argv[])
{
	List *l;
	long n;
	
	if(argc < 2){
		fprintf(stderr, "usage: revlist listsize\n");
		exit(1);
	}
	
	n = atoi(argv[1]);
	srand(time(NULL));
	l = alloclist(n*rand());
	n--;
	while(n){
		addtail(l, alloclist(n*rand()));
		n--;
	}
	printf("original: "), printlist(l);
	reverse(&l);
	printf("reversed: "), printlist(l); 
	return 0;
}
