/* watch for file events using BSD kqueue */
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h> 

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>

int
main(int argc, char *argv[])
{
	int f, kq, nev;
	struct kevent change;
	struct kevent event;

	if (argc < 2)
		exit(1);

	kq = kqueue();
	if (kq == -1)
		errx(1, "kqueue");

	f = open(argv[1], O_RDWR);
	if (f == -1)
		perror("open");

	EV_SET(&change, f, EVFILT_VNODE,
			EV_ADD | EV_ENABLE | EV_ONESHOT,
			NOTE_DELETE | NOTE_EXTEND | NOTE_WRITE | NOTE_ATTRIB,
			0, 0);

	for (;;) {
		nev = kevent(kq, &change, 1, &event, 1, NULL);
		if (nev == -1)
			perror("kevent");
		else if (nev > 0) {
			if (event.fflags & NOTE_DELETE) {
				printf("File deleted\n");
				break;
			}
			if (event.fflags & NOTE_EXTEND ||
				event.fflags & NOTE_WRITE)
				printf("File modified\n");
			if (event.fflags & NOTE_ATTRIB)
				printf("File attributes modified\n");
		}
	}

	close(kq);
	close(f);
	return 0;
}


