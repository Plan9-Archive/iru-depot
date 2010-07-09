#include <u.h>
#include <libc.h>

enum {
	MAX = 8192
};

typedef struct Matrix Matrix;
struct Matrix
{
	double **a;
	int nc;
	int nr;
};

int dflag;

void
printmatrix(Matrix *mp)
{
	int i, j;

	for(i=0; i < mp->nr; i++){
		for(j=0; j < mp->nc; j++)
			print("%g\t\t", mp->a[i][j]);
		print("\n");
	}
}

Matrix*
matrixalloc(int nr, int nc)
{
	int i;
	Matrix *mp;
	double **a;

	mp = malloc(sizeof(Matrix));
	if(!mp)
		return nil;

	a = malloc(nr*sizeof(double*));
	if(!a)
		return nil;

	for(i=0; i < nr; i++){
		a[i] = malloc(nc*sizeof(double));
		if(!a[i])
			return nil;
	}

	mp->a = a;
	mp->nr = nr;
	mp->nc = nc;
	return mp;
}

int
nilrow(double *row, int nc)
{
	int i;

	for(i=0; i < nc; i++)
		if(row[i])
			return 0;
	return 1;
}

void
swaprow(double *r, double *s, int nc)
{
	int i;
	double t;

	for(i=0; i < nc; i++){
		t = r[i];
		r[i] = s[i];
		s[i] = t;
	}
}

/* 
 * move nil rows to the bottom of the matrix. 
 * returns the number of empty rows
 */
int
cleanmatrix(double **a, int nr, int nc)
{
	int i, ne;

	ne = 0;
	for(i=0; i < nr; i++){
		if(nilrow(a[i], nc)){
			ne++;
			/* take care to not access past the last row */
			swaprow(a[i], a[i + (i+1)%nr], nc);
		}
	}
	return ne;
}

/* 
 * reduce a matrix to row echelon form
 * returns number of valid (non-nil) rows in the matrix
 */
int
echelon(Matrix *mp)
{
	int i, j, t, k, nr, nc;
	double **a, x;

	a = mp->a;
	nr = mp->nr;
	nc = mp->nc;

	for(i = j = 0; i < nr && j < nc; j++){
		/* find next pivot */
		for(k = i; k < nr; k++){
			if(a[k][j])
				break;
		}

		/* no pivot for this column */
		if(k >= nr)
			continue;

		/* sort pivot to it's place */
		swaprow(a[k], a[i], nc);

		/* since we sorted, i is the pivot row */
		for(k++; k < nr; k++){
			x = a[k][j]/a[i][j];	/* row multiplication factor */

			/* reduce lines */
			for(t=0; t < nc; t++)
				a[k][t] -= x*a[i][t];
		}

		i++;
		nr -= cleanmatrix(a, nr, nc);
		if(dflag){
			printmatrix(mp);
			print("\n");
		}
	}

	return nr;
}

/* reduce a matrix in row echelon form to row canonical form */
void
canonical(Matrix *mp)
{
	int i, j, k, t, nc, nr;
	double **a, x;

	a = mp->a;
	nr = mp->nr;
	nc = mp->nc;

	for(i = nr-1; i >= 0; i--){
		/* find pivot */
		for(j=0; a[i][j] == 0 && j < nc; j++)
			;

		/* normalize row */
		x = a[i][j];
		for(t=0; t < nc; t++)
			a[i][t] /= x;

		for(k = i-1; k >= 0; k--){
			x = a[k][j]/a[i][j];
	
			/* reduce lines */
			for(t=0; t < nc; t++)
				a[k][t] -= x*a[i][t];
		}
	}
}

int
wc(char *arg, int c, int beg, int end)
{
	char *p, *ep;
	int n;

	p = arg+beg;
	ep = p+end;
	n = 0;

	while(p < ep){
		if((*p & 0xff) == c)
			n++;
		p++;
	}
	return n;
}

void
synerr(char *fmt, ...)
{
	va_list arg;
	char buf[MAX];

	va_start(arg, fmt);
	vseprint(buf, buf+sizeof(buf), fmt, arg);
	va_end(arg);
	fprint(2, "syntax error: %s\n", buf);
	exits("syntax error");
}

int
spaces(char *s)
{
	char *p;
	int c;

	p = s;
	c = 0;
	while(*p && *p == ' ' || *p == '\t' || *p == '\n'){
		c++;
		p++;
	}
	return c;
}

/* 
 * the syntax is
 * [ a11, a12, ..., a1n ] ... [ bn1, bn2, ..., b2n ]
 * whitespace is irrelevant
 */
Matrix*
parse(char *arg)
{
	char *p;
	int nr, nc, i, j;
	double **a;
	Matrix *mp;

	nr = wc(arg, ']', 0, strlen(arg));
	p = strchr(arg, ']');
	if(!p)
		synerr("expecting ']', not found");
	nc = wc(arg, ',', 0, p-arg) + 1;

	mp = matrixalloc(nr, nc);
	if(!mp)
		return nil;
	a = mp->a;

	/* trailing whitespace become nul */
	p = arg+strlen(arg)-1;
	while(*p && *p == ' ' || *p == '\t' || *p == '\n')
		*p-- = 0;

	/* skip leading whitespace */
	p = arg + spaces(arg);

	/* parse a whole row */
	for(i=0; i < nr && *p; i++){
		p += spaces(p);
	
		if(*p != '[')
			synerr("expecting '[', found %c (0x%x)", *p, *p);
		p++;

		for(j=0; j < nc && *p && *p != ']'; j++){
			p += spaces(p);

			a[i][j] = strtod(p, &p);
			p += spaces(p);

			if(*p == ']'){
				p++;
				break;
			}
			if(*p != ',')
				synerr("expecting ',' or ']', found %c (0x%x)", *p, *p);
			p++;
		}
	}
	return mp;
}

void
usage(void)
{
	fprint(2, "usage: gj [-d] matrix\n");
	exits("usage");
}

void
main(int argc, char *argv[])
{
	char *p, buf[MAX];
	long n;
	Matrix *mp;

	ARGBEGIN {
	case 'd':
		dflag = 1;
		break;
	default:
		usage();
	} ARGEND

	if(argc > 2)
		p = argv[1];
	else{
		memset(buf, 0, sizeof(buf));
		n = read(0, buf, sizeof(buf)-1);
		if(n < 0)
			sysfatal("main: read matrix %r");
		p = buf;
	}

	mp = parse(p);
	printmatrix(mp);
	print("\n");

	mp->nr = echelon(mp);

	if(dflag){
		printmatrix(mp);
		print("\n");
	}
	canonical(mp);
	printmatrix(mp);
}
