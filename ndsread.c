/* read a Nintendo DS game header */
#include <sys/types.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

typedef unsigned char uchar;
typedef struct NDSheader NDSheader;
typedef struct NDSbanner NDSbanner;

struct NDSheader
{
	char	title[12];
	char	game[4];
	char	maker[2];
	uint8_t	unit;
	uint8_t device;
	uint8_t cardsize;
//	uint8_t	cardinfo[10];
	char	cardinfo[10];
	uint8_t	flags;

	struct{
		uint32_t	src;
		uint32_t	execaddr;
		uint32_t	copyto;
		uint32_t	binsize;
	}arm9;
	
	struct{
		uint32_t	src;
		uint32_t	execaddr;
		uint32_t	copyto;
		uint32_t	binsize;
	}arm7;

	uint32_t	fntaboff;	/* filename table offset */
	uint32_t	fntabsize;	/* filename table size */
	uint32_t	fatoffset;
	uint32_t	fatsize;	
	uint32_t	arm9oversrc;
	uint32_t	arm9oversize;
	uint32_t	arm7oversrc;
	uint32_t	arm7oversize;
	uint32_t	ctlregread;	/* control register flags for read */
	uint32_t	ctlreginit; /* control register flags for init */
	uint32_t	banneroff;	/* banner offset relative to file beginning */
	uint16_t	crc16;
	uint16_t	timeout;
	uint32_t	arm9unk;
	uint32_t	arm7unk;
	uint64_t	magic;
	uint32_t	romsize;
	uint32_t	headersize;
	char	unknown5[56];
	char	gbalogo[156];
	uint16_t	logocrc16;
	uint16_t	headercrc16;
	char	reserved[160];
};

struct NDSbanner
{
	uint16_t	version;
	uint16_t	crc16;
	char		reserved[28];
	char		tiledata[512];
	char		palette[32];

	/* every char in title is followed by a nul, and padded to 256 */
	struct{
		char	jp[256];
		char	en[256];
		char	fr[256];
		char	de[256];
		char	it[256];
		char	sp[256];
	}title;
};

void
fixtitles(NDSbanner *b)
{
	char *p;
	int i, len;

	for(p = (char*)&b->title, i = 1; i < sizeof(b->title); i++)
		p[i] = p[2*i];
}
		
int
main(int argc, char *argv[])
{
	NDSheader h;
	NDSbanner ban;
	int n, fd;

	if(argc < 2){
		fprintf(stderr, "usage: %s ROM\n", argv[0]);
		return -1;
	}

	fd = open(argv[1], O_RDONLY);
	if(fd < 0){
		fprintf(stderr, "could not open %s\n", argv[1]);
		return -1;
	}

	n = read(fd, &h, sizeof(NDSheader));
	if(n <= 0)
		return -1;
	n = lseek(fd, h.banneroff, SEEK_SET);
	if(n <= 0){
		fprintf(stderr, "could not seek to location %p\n", h.banneroff);
		return -1;
	}
	n = read(fd, &ban, sizeof(NDSbanner));
	if(n <= 0)
		return -1;

	fixtitles(&ban);
	printf("%s\n", ban.title.en);
	
	close(fd);
	return 0;
}
