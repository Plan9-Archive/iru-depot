#include <u.h>
#include <libc.h>
#include <bio.h>

enum {
	Lfalse,
	Ltrue,
	Lor,
	Land,
	Lnot,
};

typedef struct List List;

struct List
{
	int		x;
	List	*next;
};

Biobuf *bio;
List *outl, *opstk;

List*
lalloc(int x)
{
	List *s;

	s = malloc(sizeof(List));
	if(s == nil)
		sysfatal("out of memory, %r");

	s->x = x;
	s->next = nil;
	return s;
}

void
plist(List *l)
{
	List *lp;

	for(lp = l; lp; lp = lp->next)
		print("%d ", lp->x);
	print("\n");
}

void
empty(void)
{
	print("empty stack!\n");
}

void
push(List **s, int x)
{
	List *sv;

	sv = lalloc(x);
	sv->next = *s;
	*s = sv;
}

List*
pop(List **s)
{
	List *sp;

	if(s == nil)
		return nil;

	sp = *s;
	if(sp != nil)
		*s = sp->next;
	return sp;
}

void
inserttail(List **l, int x)
{
	List *e, *lp;

	e = lalloc(x);
	for(lp = *l; lp && lp->next; lp = lp->next)
		;
	if(lp)
		lp->next = e;
	else
		*l = e;
}

List*
removehead(List **l)
{
	List *h;

	if(*l == nil)
		return nil;

	h = *l;
	*l = h->next;
	return h;
}	

void
popall(List **s)
{
	while(pop(s))
		;
}

void
add(void)
{
	List *h;

	h = removehead(&outl);
	print("%d ", h->x);
	free(h);

	h = removehead(&outl);
	print("%d ", h->x);
	free(h);

	print("+");
}
	

int
readnumber(void)
{
	char buf[256], *p;
	int c;

	memset(buf, 0, sizeof(buf));
	p = buf;

	for(;;){
		c = Bgetc(bio);
		print("c '%c' %d\n", c, c);
		if(c == Beof){
			print("BEOF\n");
			return Beof;
		}
		c &= 0xFF;

		if(!isdigit(c)){
			print("not a digit\n");
			Bungetc(bio);
			break;
		}

		if(p-buf >= sizeof(buf)){
			print("bof\n");
			break;
		}
		*p++ = c;
	}
	return atoi(buf);
}

void
lex(void)
{
	int c, x;

	for(;;){
		c = Bgetc(bio);
		if(c == Beof)
			return;

		while(isspace(c))
			c = Bgetc(bio);

		c &= 0xFF;
		if(isdigit(c)){
			Bungetc(bio);
			print("read %d\n", x = readnumber());
			inserttail(&outl, x);
		}

		switch(c){
		case '+':
		case '-':
			push(&opstk, c);
			break;
		case 'S':
			print("numbers\n");
			plist(outl);
			print("ops\n");
			plist(opstk);
			break;
		}
	}
}

void
dumpop(int op)
{
	int i;
	List *h;

	for(i = 0; i < optab[op].nargs; i++){
		h = removehead(&outl);
		print("%d ", h->x);
		free(h);
	}
	print("%c", optab[op].c);
}

int
main(void)
{
	List *e;
	int i;

	bio = Bfdopen(0, OREAD);
	if(bio == nil)
		sysfatal("could not allocate I/O buffer, %r");

	lex();
	while(e = pop(&opstk)){

			(optab[i].c == e->x){
				dumpop(i);
				break;
			}
		}
		print("\n");
	}
	
	return 0;
}
