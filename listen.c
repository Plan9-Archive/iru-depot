/* simple network listener based on Plan 9's listen(1)
 * Iruata Souza - iru.muzgo!gmail.com
 * public domain
 */
 
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void usage(char *);

enum {
	NAMELEN		= 64,	/* reasonable upper limit for name elements */
	srv_port	= 1234,
};

typedef struct Service	Service;
struct Service
{
	char	remote[3*NAMELEN];	/* address of remote system */
	char	prog[5*NAMELEN+1];	/* program to execute */
	char	**args;
};

void
newcall(int fd, Service *s)
{
	switch(rfork(RFFDG|RFPROC|RFNOWAIT)) {
		case -1: 
			err(1, "rfork");
		case 0:
			dup2(fd, 0);
			dup2(fd, 1);
			dup2(fd, 2);
			close(fd);
			
			execv(s->prog, s->args);
	}
}

void
dolisten(Service *s, int ctl, int port)
{
	int data;
 	struct sockaddr_in ch, r;
	socklen_t n;

	n = 1;
	bzero(&ch, sizeof(ch));
	ch.sin_family = PF_INET;
	ch.sin_addr.s_addr = htonl(INADDR_ANY);
	ch.sin_port = htons(port ? port : srv_port);

	ctl = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(ctl < 0)
		err(1, "socket");
	setsockopt(ctl, SOL_SOCKET, SO_REUSEADDR, (char *) &n, sizeof(n));

	if((bind(ctl, (struct sockaddr *) &ch, sizeof(ch))) < 0)
		err(1, "bind");

	if((listen(ctl, 5)) < 0)
		err(1, "listen");

	n = sizeof(r);

	for(;;){
		data = accept(ctl, 0, 0);
		if(data < 0)
			err(1, "accept");
	//	getpeername(data, (struct sockaddr *)&r, &n);
	//	fmt_ip(s->remote, (union sckaddr_sto *)&r); 
		newcall(data, s);
	}
}	
		
int
main(int argc, char *argv[])
{
	Service s;
      	
	if(argc < 3)
		usage(argv[0]);
	strlcpy(s.prog, argv[2], 5*NAMELEN);
	s.args = argv+2;
	dolisten(&s, 0, atoi(argv[1]));
	return 0;
}

void
usage(char *name)
{
	fprintf(stderr, "usage: %s port cmd\n", name);
	exit(1);
} 
