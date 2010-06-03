/* old and dirty stack implementation */
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long ulong;
typedef unsigned char uchar;
typedef struct Stack Stack;

enum SType {
	Tchar,
	Tint,
	Tfloat,
	Tdouble,
};

struct Stack {
	void *data;
	ulong size;
	enum SType type;
	Stack *next;
};

void
dumpelem(char *name, Stack *sp)
{
	union {
		char *cp;
		int *dp;
		float *fp;
		double *lfp;
	} tp;

	if(name)
		printf("%s ", name);
	printf("[%p ", sp);
	switch(sp->type){
	case Tchar:
		tp.cp = sp->data;
		printf("%c", *tp.cp);
		break;
	case Tint:
		tp.dp = sp->data;
		printf("%d", *tp.dp);
		break;
	default:
		printf("%s", (char*)sp->data);
	}
	printf(" %lu -> %p]\n", sp->size, sp->next);
}

void
dumpstack(Stack *stk)
{
	Stack *sp;
	
	for(sp = stk; sp; sp = sp->next)
		dumpelem(0, sp);
	printf("-- end of dump --\n");
}

Stack*
salloc(void *data, ulong n, enum SType type)
{
	Stack *sp;

	sp = malloc(sizeof(Stack));
	sp->data = data;
	sp->size = n;
	sp->type = type;
	sp->next = NULL;
	return sp;
}

void
push(void *data, ulong n, enum SType type, Stack **stk)
{
	Stack *sp;

	sp = salloc(data, n, type);
	sp->next = *stk;
	*stk = sp;
}

Stack*
pop(Stack **stk)
{
	Stack *sp;
	
	if(*stk == NULL)
		return NULL;
	
	sp = *stk;
	*stk = (*stk)->next;
	return sp;
}

void
test(void)
{
	Stack *stk;
	int *d;

	d = malloc(sizeof(int));
	*d = 518;
	stk = salloc(0, 0, -1);
	dumpstack(stk);
	push("iru", 4, -1, &stk);
	dumpstack(stk);
	push(d, sizeof(int), Tint, &stk);
	dumpstack(stk);
	free(pop(&stk));
	dumpstack(stk);
	return 0;
}


