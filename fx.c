#include <u.h>
#include <libc.h>
#include <draw.h>
#include <thread.h>
#include <mouse.h>

Mousectl *mousectl;

enum {
	STACK = 32768,
};

void
eresized(int new)
{
	Rectangle box;

	if(new && getwindow(display, Refnone) < 0)
		sysfatal("reattach to window failed: %r");

	draw(screen, screen->r, display->white, nil, ZP);

	box = screen->r;
	box.min.x += Dx(screen->r)/2;
	box.min.y += Dy(screen->r)/2;
	box.max.x = box.min.x+25;
	box.max.y = box.min.y+25;

	flushimage(display, 1);
}

void
mousethread(void*)
{
	Mouse m;
	enum { MResize, MMouse, MEnd };

	threadsetname("mousethread");
	Alt a[] = {
				/* c				v					op */
	[MResize]	{mousectl->resizec, nil, 				CHANRCV},
	[MMouse]	{mousectl->c,		&mousectl->Mouse,	CHANRCV},
	[MEnd]		{nil,				nil,				CHANEND},
	};

	for(;;){
		flushimage(display, 1);
		switch(alt(a)){
		case MResize:
			if(getwindow(display, Refnone) < 0)
				sysfatal("could not reattach to window: %r");
			break;
		case MMouse:
			m = mousectl->Mouse;
			Point p;
			char buf[64];
			p.x = screen->r.min.x + 10;
			p.y = screen->r.min.y + 10;
			sprint(buf, "mouse %P", m.xy);
			draw(screen, screen->r, display->white, nil, ZP);
			string(screen, p, display->black, ZP, font, buf);
			break;
		}
	}
}
	
void
threadmain(int argc, char *argv[])
{
	int p[2];

	if(argc < 2){
		fprint(2, "usage: fx images\n");
		exits("usage");
	}

	if(initdraw(nil, nil, nil) < 0)
		sysfatal("can't initialize screen: %r");

	mousectl = initmouse(nil, screen);
	if(mousectl == nil)
		sysfatal("can't initialize mouse: %r");
	draw(screen, screen->r, display->white, nil, ZP);

	threadcreate(mousethread, nil, STACK);

	
	if(pipe(p) < 0)
		sysfatal("pipe failed: %r");
	
	if(fork() == 0){
		close(1);
		dup(p[1], 1);
		execl("/bin/jpg", "jpg", "-9t", argv[1], 0);
	//	execl("/bin/resample", "resample", "-x50%", argv[1], 0);
	}

	char buf[16];
	memset(buf, 0, sizeof buf);
	read(p[0], buf, sizeof buf);
	int i;
	for(i=0; i < sizeof buf; i++)
		print("%02ux ", buf[i]&0xFF);
}
