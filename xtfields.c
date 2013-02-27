/*
 * Given a file and a list of field names, print the field values.
 * N.B. A record is a line of data. A field is a string delimited by a space, \t or \n.
 *
 * The file format is:
 * # descr of N: field0name field1name ... fieldNname
 * rec0field0val rec0field1val ... rec0fieldNval
 * ...
 * recMfield0val recMfield1val ... recMfieldNval
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <err.h>

enum {
	Maxname = 256,
};

typedef struct {
	FILE		*fp;				/* Pointer to FILE */
	uint32_t	recoff;				/* Offset of first record in fp */
	uint32_t	nrecs;				/* Number of records in fp */
	uint32_t	nfields;			/* Number of fields per record */
	char		**fields;			/* Table of the fields' names */
} Data;


uint32_t
countfields(FILE *fp)
{
	uint32_t nf;

	nf = 0;
	if(fseek(fp, 0, SEEK_SET) != 0)
		return -1;

	for(;;){
		switch(fgetc(fp)){
		case ' ':
			nf++;
			break;
		case '\n':
		case EOF:
			return nf+1;	/* fields = spaces+1 */
		default:
			break;
		}
	}
}

int
readword(FILE *fp, char *buf, int nbuf)
{
	int c, i;

	for(i = 0; i < nbuf; i++){
		c = fgetc(fp);
		if(c == ' ' || c == '\n' || c == EOF)
			break;
		else
			buf[i] = c;
	}
	buf[i] = '\0';
	return i;
}

int
fillfields(Data *dp)
{
	uint32_t nf, i, j;
	char **tbl;

	fscanf(dp->fp, "# descr of %u: ", &nf);

	tbl = malloc(nf*sizeof(char*));
	if(tbl == NULL)
		return 0;

	for(i = 0; i < nf; i++){
		tbl[i] = malloc(Maxname);
		if(tbl[i] == NULL){
			for(j = i-1; j >= 0; j--)
				free(tbl[j]);
			free(tbl);
			return 0;
		}
		readword(dp->fp, tbl[i], Maxname);
	}
	
	dp->recoff = ftell(dp->fp);
	dp->fields = tbl;
	dp->nfields = nf;
	return 1;
}

void
countrecs(Data *dp)
{
	dp->nrecs = 0;
	fseek(dp->fp, dp->recoff, SEEK_SET);

	for(;;){
		switch(fgetc(dp->fp)){
		case '\n':
			dp->nrecs++;
			break;
		case EOF:
			fseek(dp->fp, dp->recoff, SEEK_SET);
			return;
		default:
			break;
		}
	}
}

Data*
newdata(char *path)
{
	Data *dp;
	FILE *fp;

	fp = fopen(path, "r");
	if(fp == NULL)
		return NULL;

	if(fseek(fp, 0, SEEK_SET) != 0)
		return NULL;

	dp = malloc(sizeof(Data));
	if(dp == NULL)
		return NULL;
	dp->fp = fp;

	if(!fillfields(dp)){
		free(dp);
		return NULL;
	}
	countrecs(dp);
	return dp;
}

int
jumptorec(Data *dp, uint32_t rec)
{
	uint32_t i;

	if(rec > dp->nrecs)
		rec = dp->nrecs;

	for(i = 0; i < rec;){
		switch(fgetc(dp->fp)){
		case '\n':
			i++;
			break;
		case EOF:
			return 0;
		default:
			break;
		}
	}
	return 1;
}

int
jumptofield(Data *dp, uint32_t fld)
{
	uint32_t i;
	
	if(fld > dp->nfields)
		fld = dp->nfields;

	for(i = 0; i < fld;){
		switch(fgetc(dp->fp)){
		case ' ':
			i++;
			break;
		case EOF:
			return 0;
		default:
			break;
		}
	}
	return 1;
}

int
jumpto(Data *dp, uint32_t rec, uint32_t fld)
{
	fseek(dp->fp, dp->recoff, SEEK_SET);
	if(!jumptorec(dp, rec))
		return 0;
	if(!jumptofield(dp, fld))
		return 0;
	return 1;
}

int
findfield(Data *dp, char *s)
{
	uint32_t i;

	for(i = 0; i < dp->nfields; i++){
		if(strncasecmp(dp->fields[i], s, Maxname) == 0)
			return i;
	}
	return -1;
}

void
printfields(Data *dp, uint32_t *fld, uint32_t nfld)
{
	uint32_t i, j;
	char buf[Maxname];

	for(i = 0; i < dp->nrecs; i++){
		for(j = 0; j < nfld; j++){
			if(!jumpto(dp, i, fld[j]))
				err(1, "premature eof 1");
			readword(dp->fp, buf, sizeof(buf));
			printf("%s ", buf);
		}
		puts("");
	}
}
	
void
usage(void)
{
	fprintf(stderr, "usage: ExN file fields\n");
	exit(1);
}

int
main(int argc, char *argv[])
{
	uint32_t i, *fld, *p, j, nf;
	Data *dp;

	if(argc < 3 || (argv[1][0] == '-' && argv[1][1] == 'h'))
		usage();

	dp = newdata(argv[1]);
	if(dp == NULL)
		err(1, "could not allocate Data");

	fld = malloc((argc-2)*sizeof(uint32_t));
	if(fld == NULL)
		err(1, "out of memory for fld");
	memset(fld, -1, (argc-2)*sizeof(uint32_t));

	p = fld;
	nf = 0;
	for(i = 2; i < argc; i++){
		j = findfield(dp, argv[i]);
		if(j == -1){
			fprintf(stderr, "%s is not a field of %s\n", argv[i], argv[1]);
			continue;
		}
		*p = j;
		p++;
		nf++;
	}

	printfields(dp, fld, nf);
	return 0;
}

	