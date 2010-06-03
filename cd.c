/* check cd drive status */
#include <stdio.h>
#include <fcntl.h>

int
main(int argc, char *argv[])
{
	char buf[16];
	int fd, error;

	if (argc < 2)
		exit(1);

	fd = open(argv[1], O_RDWR);
	if (fd < 0)
		printf("drive empty\n");
	else
		printf("drive not empty\n");
	
	error = read(fd, buf, 15);
	buf[15]='\0';
	printf("buf %s err %d\n", buf, error);
	return 0;
}
