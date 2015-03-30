#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct List List;
struct List {
	char	c;
	List	*next;
};

void*
emalloc(size_t size)
{
	void *v;
	v = malloc(size);
	if(v == NULL)
		exit(1);
	return v;
}

List*
c2elem(char c)
{
	List *lp;

	lp = emalloc(sizeof(List));
	lp->c = c;
	lp->next = NULL;
	return lp;
}

void
printlist(List *lp)
{
	List *p;

	for(p = lp; p; p = p->next)
		printf("%c ", p->c);
	puts("");
}

List*
tailinsert(List *head, List *elem)
{
	List *p;

	if(head == NULL)
		return elem;

	for(p = head; p->next; p = p->next)
		;
	p->next = elem;
	return head;
}

List*
rmelem(List *head, List *elem)
{
	List *cur, *prev;

	if(elem == head)
		return head->next;

	prev = head;
	for(cur = head->next; cur;){
		if(cur == elem){
			prev->next = cur->next;
			break;
		}
		prev = cur;
		cur = cur->next;
	}
	return head;
}

int
isvowel(char c)
{
	c = tolower(c);
	if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
		return 1;
	return 0;
}

int
main(int argc, char *argv[])
{
	List *head, *vowels, *p;
	int n, i;
	char c;

	if(argc < 2){
		fprintf(stderr, "usage: sl string\n");
		exit(1);
	}

	head = c2elem(argv[1][0]);
	n = strlen(argv[1]);
	for(i = 1; i < n; i++)
		tailinsert(head, c2elem(argv[1][i]));

	vowels = NULL;
	for(p = head; p;){
		c = p->c;
		if(isvowel(c)){
			head = rmelem(head, p);
			p = head->next;
			vowels = tailinsert(vowels, c2elem(c));
		}else
			p = p->next;			
	}

	
	head = tailinsert(vowels, head);
	printlist(head);
	return 0;
}
