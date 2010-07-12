#include <u.h>
#include <libc.h>

typedef struct Voldesc Voldesc;
typedef struct Drec Drec;

enum
{
	Vboot,
	Vpri,
	Vsup,
	Vpart,
	Vterm = 255,

	Blocksz = 2048,
};

char *typetab[] = {
	[Vboot]	"boot",
	[Vpri]	"primary",
	[Vsup]	"suplementary",
	[Vpart]	"partition",
};
	
struct Voldesc
{
	char type;
	char id[5];
	char version;
	char data[2041];
};

struct Drec
{
	uchar	reclen;
	uchar	attrlen;
	uchar	addr[8];
	uchar	size[8];
	uchar	date[6];
	uchar	tzone;		/* flags in high sierra */
	uchar	flags;		/* ? in high sierra */
	uchar	unitsize;	/* ? in high sierra */
	uchar	gapsize;	/* ? in high sierra */
	uchar	vseqno[4];		/* ? in high sierra */
	uchar	namelen;
	uchar	name[1];
};

static long
l32(void *arg)
{
	uchar *p = arg;
	return ((((((long)p[3]<<8)|p[2])<<8)|p[1])<<8)|p[0];
}

void
main(int argc, char *argv[])
{
	Voldesc v;
	int fd;
	long n;
	uchar buf[Blocksz], *p;
	char *s;
	Drec *root, *dp;

	if(argc < 2){
		fprint(2, "usage: iso cd\n");
		exits("usage");
	}

	if((fd = open(argv[1], OREAD)) < 0)
		sysfatal("open %r");

	/* skip reserved data */
	seek(fd, 0xF*Blocksz, 0);

	for(;;){
		n = read(fd, &v, sizeof(Voldesc));
		if(n < 0)
			sysfatal("read %r");
		if(n == 0)
			break;
		
		if(v.type == Vpri)
			break;
	}

	root = malloc(v.data[149]);
	if(!root)
		sysfatal("malloc %r");

	print("%d\n", v.data[149]);
	memcpy(root, &v.data[149], v.data[149]);
	seek(fd, l32(root->addr)*Blocksz, 0);
	n = read(fd, buf, l32(root->size));
	if(n <= 0)
		sysfatal("read %r");

	print("root\n");
	p = buf + buf[0];
	print("buf[0] %d p[0] %d\n", buf[0], p[0]);
	p += p[0];
	for(; p[0]; p += p[0]){
		dp = (Drec*)p;
		s = malloc(dp->namelen+1);
		if(!s)
			sysfatal("malloc %r");
		memmove(s, dp->name, dp->namelen);
		s[dp->namelen] = 0;

		/* directory */
		if(dp->flags & 2)
			print("Directory ");
		else
			print("File ");
		print("%s\n", s);
		if(strncmp(s, "386", dp->namelen) == 0)
			break;
		free(s);
	}
	free(s);

	seek(fd, l32(dp->addr)*Blocksz, 0);
	n = read(fd, buf, l32(dp->size));
	if(n <= 0)
		sysfatal("read %r");

	p = buf + buf[0];
	print("buf[0] %d p[0] %d\n", buf[0], p[0]);
	p += p[0];
	for(; p[0]; p += p[0]){
		dp = (Drec*)p;
		s = malloc(dp->namelen+1);
		if(!s)
			sysfatal("malloc %r");
		memmove(s, dp->name, dp->namelen);
		s[dp->namelen] = 0;

		/* directory */
		if(dp->flags & 2)
			print("Directory ");
		else
			print("File ");
		print("%s\n", s);
		free(s);
	}
	close(fd);
}
