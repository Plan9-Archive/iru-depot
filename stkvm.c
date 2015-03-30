/* Stack vm */
#include <stdio.h>
#include <stdint.h>

enum {
	MAXPSTK = 64,
	MAXRSTK	= 64,
};


typedef struct Mach Mach;
typedef void (*Inst)(Mach*);

struct Mach {
	uint64_t	*psp;
	uint64_t	*rsp;
	uint64_t	w;
	uint64_t	*ip;

	uint64_t	pstk[MAXPSTK];
	uint64_t	rstk[MAXRSTK];
};


void
printpstk(Mach *mp)
{
	uint64_t *p;

	for(p = mp->psp-1; p >= &mp->pstk[0]; p--)
		printf("%ld ", (int64_t)*p);
	printf("ok\n");
}

void
push(Mach *mp)
{
	*mp->psp = *(uint64_t *)mp->ip++;
	mp->psp++;
}

void
drop(Mach *mp)
{
	mp->psp--;
}

void
dup(Mach *mp)
{
	*mp->psp = *(mp->psp - 1);
	mp->psp++;
}

void
minit(Mach *mp)
{
	mp->ip = 0;
	mp->psp = &mp->pstk[0];
	mp->rsp = &mp->rstk[0];
	mp->w = 0;
}

void
run(Mach *mp, uint64_t *prog)
{
	void (*fn)(Mach*);

	mp->ip = prog;
	while((uint64_t)*mp->ip != 0xcafebabe){
		fn = (void (*)(Mach*))*mp->ip++;
		fn(mp);
	}
}

int
main(void)
{
	uint64_t prog[] = {push, 1, push, 2, push, 3, dup, drop, printpstk, 0xcafebabe};
	Mach m;

	minit(&m);
	run(&m, prog);
	return 0;
}
