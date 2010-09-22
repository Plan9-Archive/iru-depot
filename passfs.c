#include <u.h>
#include <libc.h>
#include <thread.h>
#include <fcall.h>
#include <9p.h>

typedef struct Aux Aux;
struct Aux
{
	char *name;
	int fd;

	ulong diroff;
	Dir *dir;
	int ndir;
};

int ctlfd;

void
usage(void)
{
	fprint(2, "usage: passfs [-D] [-s service] [-l listener] mtpt\n");
	exits("usage");
}

void
freefid(Fid *f)
{
	Aux *a;
	int i;

	a = f->aux;
	if(!a)
		return;

	for(i=0; i < a->ndir; i++)
		free(&a->dir[i]);
	free(a->name);
	free(a);
	f->aux = nil;
}


Qid*
path2qid(char *path, Qid *q)
{
	Dir *d;

	if(path == nil || q == nil)
		return nil;

	d = dirstat(path);
	if(d == nil)
		*q = (Qid){0, 0, QTFILE};
	else {
		*q = d->qid;
		free(d);
	}
	return q;
}

Aux*
newaux(char *path)
{
	Aux *aux;

	aux = emalloc9p(sizeof(Aux));
	aux->name = estrdup9p(path);
	aux->fd = -1;
	aux->ndir = 0;
	aux->diroff = 0;
	return aux;
}

static void
fsattach(Req *r)
{
	Fid *f;

	if(r->ifcall.aname && r->ifcall.aname[0]){
		respond(r, "invalid attach specifier");
		return;
	}

	f = r->fid;
	f->aux = newaux(".");
	path2qid(".", &f->qid);

	r->ofcall.qid = f->qid;
	respond(r, nil);
}

static char*
fswalk1(Fid *fid, char *name, Qid *qid)
{
	Aux *a;
	Dir *d;
	char npath[1024];

	a = fid->aux;
	snprint(npath, sizeof npath, "%s/%s", a->name, name);
	
	/* aux contains the parent fid info, free them */
	free(a->name);
	free(a);
	fid->aux = nil;

	d = dirstat(npath);
	if(!d)
		return "file not found";

	fid->aux = newaux(npath);
	path2qid(npath, &fid->qid);
	*qid = fid->qid;
	
	return nil;
}

static char*
fsclone(Fid *of, Fid *nf)
{
	Aux *a;

	a = of->aux;
	if(a)
		nf->aux = newaux(a->name);
	return nil;
}

static void
fsopen(Req *r)
{
	Aux *aux;
	int fd;
	
	aux = r->fid->aux;
	fd = open(aux->name, OREAD);
	if(fd < 0){
		respond(r, "could not open");
		return;
	}
	aux->fd = fd;
	respond(r, nil);
	fprint(ctlfd, "open name:%s\n", aux->name);
}

static int
dirgen(int, Dir *rd, void *v)
{	
	Dir *d;
	Aux *aux;
	
	aux = ((Fid*)v)->aux;
	d = dirstat(aux->name);
	if(!d)
		return -1;

	memset(rd, 0, sizeof *rd);
	rd->qid = d->qid;
	rd->mode = d->mode;
	rd->atime = d->atime;
	rd->mtime = d->mtime;
	rd->length = d->length;
	rd->name = estrdup9p(d->name);
	rd->uid = estrdup9p(d->uid);
	rd->gid = estrdup9p(d->gid);
	rd->muid = estrdup9p(d->muid);
	return 0;
}

static void
fsdirread(Req *r)
{
	Fid *f;
	Aux *a;
	long n, i;
	char *rdata;

	f = r->fid;
	a = f->aux;
	rdata = r->ofcall.data;

	if(r->ifcall.offset == 0){
		if(a->dir)
			free(a->dir);
		a->dir = nil;
		if(a->diroff != 0)
			seek(a->fd, 0, 0);
		a->ndir = dirreadall(a->fd, &a->dir);
		a->diroff = 0;
		if(a->ndir < 0){
			respond(r, "could not read dir");
			return;
		}
	}

	/* copy in as many directory entries as possible */
	for(n = 0; a->diroff < a->ndir; n += i){
		i = convD2M(&a->dir[a->diroff], (uchar*)rdata+n, r->ifcall.count - n);
		if(i <= BIT16SZ)
			break;
		a->diroff++;
	}

	r->ofcall.count = n;
	respond(r, nil);
}		

static void
fsread(Req *r)
{
	long n;
	Aux *a;

	if(r->fid->qid.type & QTDIR){
		fsdirread(r);
		return;
	}

	a = r->fid->aux;
	n = pread(a->fd, r->ofcall.data, r->ifcall.count, r->ifcall.offset);
	if(n < 0){
		respond(r, "read error");
		return;
	}
	
	r->ofcall.count = n;
	respond(r, nil);
}

static void
fsstat(Req *r)
{
	Fid *f;

	f = r->fid;
	if(dirgen(0, &r->d, f) == -1){
		respond(r, "could not retrieve stat");
		return;
	}
	respond(r, nil);
}

static void
fsdestroyfid(Fid *f)
{
	Aux *a;

	a = f->aux;
	free(a->dir);
	free(a->name);
}

Srv fs = {
.attach=		fsattach,
.walk1=			fswalk1,
.clone=			fsclone,
.open=			fsopen,
.read=			fsread,
.stat=			fsstat,
.destroyfid=	fsdestroyfid,
};

void
main(int argc, char *argv[])
{
	char *mtpt, *svc, *ctl, buf[64];
	int p[2], cp[2];

	svc = nil;
	ctl = nil;

	ARGBEGIN{
	case 'D':
		chatty9p++;
		break;
	case 's':
		svc = EARGF(usage());
		break;
	case 'l':
		ctl = EARGF(usage());
		break;
	default:
		usage();
	}ARGEND

	if(argc < 1)
		usage();

	mtpt = argv[0];

	if(pipe(p) < 0)
		sysfatal("pipe failed %r");
	fs.infd = p[1];
	fs.outfd = p[1];
	fs.srvfd = p[0];

	if(ctl){
		snprint(buf, sizeof buf, "/srv/%s", ctl);
		if(pipe(cp) < 0)
			sysfatal("pipe failed %r");

		ctlfd = create(buf, OWRITE|ORCLOSE, DMEXCL|0600);
		if(ctlfd < 0)
			sysfatal("could not post control file %s, %r", ctl);
		fprint(ctlfd, "%d", cp[0]);
		close(cp[0]);
		ctlfd = cp[1];
	}

	if(svc)
		if(postfd(svc, fs.srvfd) < 0)
			sysfatal("could not post service %s, %r", svc);
	
	switch(rfork(RFFDG|RFPROC|RFNAMEG|RFNOTEG)){
	case -1:
		sysfatal("fork: %r");
	case 0:
		chdir(mtpt);
		close(p[0]);
		srv(&fs);
		break;
	default:
		if(mount(fs.srvfd, -1, mtpt, MREPL|MCREATE, "") < 0)
			sysfatal("mount failed: %r");
	}
	exits(nil);
}
