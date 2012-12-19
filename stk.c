/* old and dirty stack implementation */
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long ulong;
typedef unsigned char uchar;
typedef struct Stack Stack;

enum SType {
	Tnone,
	Tchar,
	Tint,
	Tfloat,
};

struct Stack {
	void *data;
	ulong size;
	enum SType type;
	Stack *next;
};

void
dumpelem(Stack *sp)
{
	int i;

	printf("[%p %lu ", sp, sp->size);
	switch(sp->type){
	case Tchar:
		printf("\'%c\'", *(char*)sp->data);
		break;
	case Tint:
		printf("%d", *(int*)sp->data);
		break;
	case Tfloat:
		printf("%f", *(float*)sp->data);
		break;
	case Tnone:
	default:
		for(i = 0; i < sp->size; i++)
			printf("%02x ", ((char*)sp->data)[i]);
	}
	printf(" -> %p]\n", sp->next);
}

void
dumpstack(Stack *stk)
{
	Stack *sp;
	
	for(sp = stk; sp; sp = sp->next)
		dumpelem(sp);
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

int
main(void)
{
	Stack *stk;
	int d;

	stk = salloc("iru", 4, Tnone);
	dumpstack(stk); puts("");

	d = 518;
	push(&d, sizeof(int), Tint, &stk);
	dumpstack(stk); puts("");

	free(pop(&stk));
	dumpstack(stk);
	return 0;
}



