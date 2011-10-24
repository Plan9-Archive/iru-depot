#ifdef __linux__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void
exits(char *err)
{
	fprintf(stderr, "%s\n", err);
	exit(1);
}

#define dup dup2
#define print printf
#define stop 1
#define nil NULL

#else /* plan 9 */
#include <u.h>
#include <libc.h>

#define stop 1000
#endif

main(int argc, char *argv[])
{
	long n;
	int p[2], pid, i;

	if(argc < 4)
		exits("usage: pipefeed string path args");
	if(pipe(p) < 0)
		exits("pipe");
		
	if((pid = fork()) == 0){
		close(p[1]);
		dup(p[0], 0);
		execl(argv[2], argv[3], (char*)nil);
		exits(argv[2]);
	}else if(pid < 0)
		exits("fork");

	for(;;){
		n = write(p[1], argv[1], strlen(argv[1]));
		print("parent %d: wrote %ld\n", getpid(), n);
		sleep(stop);
	}
}
