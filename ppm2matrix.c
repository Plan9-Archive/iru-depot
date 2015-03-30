/* Print PPM image as an ascii matrix */

#include <u.h>
#include <libc.h>
#include <bio.h>

typedef struct Image Image;
struct Image
{
	int		w;
	int		h;
	char	**data;
};

enum {
	BI2BY = 8,		/* bits in a byte */
};

void*
emallocz(ulong n)
{
	void *a;

	a = malloc(n);
	if(a == nil)
		sysfatal("out of memory, %r");
	memset(a, 0, n);
	return a;
}

Image*
newimage(int w, int h)
{
	int i;
	char **data;
	Image *ip;

	data = emallocz(h*sizeof(char*));
	for(i = 0; i < h; i++)
		data[i] = emallocz(w);
	
	ip = emallocz(sizeof(Image));
	ip->w = w;
	ip->h = h;
	ip->data = data;
	return ip;
}

Image*
ppm2matrix(Biobuf *bp)
{
	Image *ip;
	char *buf, *dim[2];
	int i, j, c, skipbits;

	/* read version */
	buf = Brdline(bp, '\n');
	if(buf == nil)
		return nil;
	buf[Blinelen(bp)-1] = '\0';			/* remove trailing newline */

	/* read initial comments */
	for(;;){
		buf = Brdline(bp, '\n');
		if(buf == nil){
			print("EOF\n");
			break;
		}
		buf[Blinelen(bp)-1] = '\0';
		if(buf[0] != '#')
			break;
	}

	if(tokenize(buf, dim, 2) < 2)
		sysfatal("error in image dimensions");

	ip = newimage(atoi(dim[0]), atoi(dim[1]));
	skipbits = BI2BY - ip->w;

	for(i = 0; i < ip->h; i++){
		/* read a whole byte collecting only the relevant bits */
		c = Bgetc(bp);
		if(c == Beof)
			break;

		c >>= skipbits;
		/* Bits come reversed from what we would expect in an ASCII PBM */
		for(j = ip->w-1; j >= 0; j--){
			ip->data[i][j] = c&0x1;
			c >>= 1;
		}
	}
	return ip;
}

void
main(int argc, char *argv[])
{
	Biobuf *bp;
	Image *ip;
	int i, j;

	if(argc < 2){
		fprint(2, "usage: %s pbmfile\n", argv0);
		exits("usage");
	}

	bp = Bopen(argv[1], OREAD);
	if(bp == nil)
		sysfatal("Bopen %r");

	ip = ppm2matrix(bp);
	for(i = 0; i < ip->h; i++){
		for(j = 0; j < ip->w; j++)
			print("%d ", ip->data[i][j]);
		print("\n");
	}
	Bterm(bp);
}
