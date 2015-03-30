#include <u.h>
#include <libc.h>
#include <draw.h>
#include <memdraw.h>

uchar*
nnzoom(uchar *src, int ow, int oh, int nw, int nh, int nchan)
{
	uchar *dst, *new;
	double xrat, yrat;
	int i, j;

	new = malloc(nw*nh*nchan);
	if(new == nil)
		sysfatal("malloc: %r");
	dst = new;

	xrat = (double)ow/nw;
	yrat = (double)oh/nh;

	for(j=0; j < nh; j++){
		int srcy = (int)(j * yrat);
		uchar *srcrow = src + srcy * ow * nchan;
		
		for(i=0; i < nw; i++){
			int offx = (int)(i * xrat) * nchan;
			
			/* copy complete pixel, not generic yet */
			/*memcpy(dst, srcrow+offx, nchan);
			dst += nchan;*/
			*dst++ = srcrow[offx + 0];
			*dst++ = srcrow[offx + 1];
			*dst++ = srcrow[offx + 2];
			*dst++ = srcrow[offx + 3];
		}
	}
	return new;
}

Memimage*
resample(int xsize, int ysize, Memimage *m)
{
	int j, nchan;
	Memimage *new;
	uchar *nscan;
	ulong size;

	new = allocmemimage(Rect(0, 0, xsize, ysize), m->chan);
	if(new == nil)
		sysfatal("allocmemimage: %r");

	nchan = m->depth/8;
	size = bytesperline(m->r, m->depth)*Dy(m->r);

	nscan = nnzoom(byteaddr(m, m->r.min), Dx(m->r), Dy(m->r), xsize, ysize, nchan);
	j = loadmemimage(new, new->r, nscan, size);
	if(j == -1)
		sysfatal("loadimage: %r");
	return new;
}

void
main(int argc, char *argv[])
{
	Memimage *old, *new;
	int fd;
	vlong sec;

	if(argc < 2)
		fd = 0;
	else
		if((fd = open(argv[1], OREAD)) < 0)
			sysfatal("open %s: %r", argv[1]);

	memimageinit();

	old = readmemimage(fd);
	if(old == nil)
		sysfatal("readmemimage: %r");
	close(fd);

	sec = nsec();
 	new = resample(Dx(old->r)*0.5, Dy(old->r)*0.5, old);
	sec = nsec() - sec;

	if((fd = create("img", OWRITE, 0644)) < 0)
		sysfatal("create img: %r");
	writememimage(fd, new);
	close(fd);
	print("spent %lf seconds on resampling\n", (double)sec*1e-9);
}
