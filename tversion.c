#include <stdio.h>
#include <stdint.h>

int
main(void)
{
	char msg[] = {0x13, 0x00, 0x00, 0x00, 
				100, 
				0x00, 0x00, 
				0x00, 0x20, 0x00, 0x00, 
				0x06, 0x00, '9', 'P', '2', '0', '0', '0'
	};

	char buf[8192];

	write(1, msg, sizeof(msg)/sizeof(msg[0]));

//	read(0, buf, 4);
//	printf("%x %x %x %x\n", buf[0], buf[1], buf[2], buf[3]);
	return 0;
}
