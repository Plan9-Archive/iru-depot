#include <u.h>
#include <libc.h>
#include <draw.h>

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

Image*
resample(int xsize, int ysize, Image *m)
{
	int j, bpl, nchan;
	Image *new;
	uchar *oscan, *nscan;
	ulong size;

	new = allocimage(display, Rect(0, 0, xsize, ysize), m->chan, 1, DNofill);
	if(new == nil)
		sysfatal("allocimage: %r");

	bpl = bytesperline(m->r, m->depth);
	nchan = m->depth/8;

	/* we use a one-dimensional array for the image */
	size = bpl*Dy(m->r);
	oscan = malloc(size);
	if(oscan == nil)
		sysfatal("malloc: %r");

	j = unloadimage(m, m->r, oscan, size);
	if(j != size)
		sysfatal("unloadimage");

	nscan = nnzoom(oscan, Dx(m->r), Dy(m->r), xsize, ysize, nchan);
	j = loadimage(new, new->r, nscan, size);
	if(j == -1)
		sysfatal("loadimage: %r");
	return new;
}

void
main(int argc, char *argv[])
{
	Image *new, *old;
	int fd;
	vlong sec;

	if(argc < 2)
		fd = 0;
	else
		if((fd = open(argv[1], OREAD)) < 0)
			sysfatal("open %s: %r", argv[1]);

	if(initdraw(nil, nil, nil) < 0)
		sysfatal("initdraw: %r");

	old = readimage(display, fd, 0);
	if(old == nil)
		sysfatal("readimage: %r");
	close(fd);

	sec = nsec();
  	new = resample(Dx(old->r)*0.5, Dy(old->r)*0.5, old);
	sec = nsec() - sec;

	if((fd = create("img", OWRITE, 0644)) < 0)
		sysfatal("create img: %r");
	writeimage(fd, new, 0);
	close(fd);
	print("spent %lf seconds on resampling\n", (double)sec*1e-9);
}
