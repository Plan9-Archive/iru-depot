#include <sys/types.h>

#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <id3.h>

typedef struct flist flist;
struct flist
{
	char path[PATH_MAX];
	ID3 *info;
	flist *next;
};

/* can do generic compare with mask */
int
titlecmp(flist *f0, flist *f1)
{
	if(!f0->info->mask & TITLE_TAG || !f1->info->mask & TITLE_TAG)
		return 0;
	return !strncasecmp(f0->info->title, f1->info->title, f0->info->title_size);
}

int
artistcmp(flist *f0, flist *f1)
{
        if(!f0->info->mask & ARTIST_TAG || !f1->info->mask & ARTIST_TAG)
                return 0;
        return !strncasecmp(f0->info->artist, f1->info->artist, f0->info->artist_size);
}     

flist*
id3lookup(flist *f, flist *files)
{
	flist *flp;

	for(flp = files; flp->next; flp = flp->next)
		if(artistcmp(f, flp) && titlecmp(f, flp))
			return flp;
	return NULL;
}

flist*
id3dir(char *dirpath)
{
	DIR *dirp;
	struct dirent *dp;
	flist *files, *flp;
	char path[PATH_MAX];
	uint len;

	len = strlen(dirpath);
	files = malloc(sizeof(flist));
	if(!files)
		exit(1);
	flp = files;

dir:
	dirp = opendir(dirpath);
	if(!dirp)
		return NULL;

	while(dp = readdir(dirp)){
		struct stat st;
		flist *fmatch;
	
                /* skip '.' and '..' */
                if(dp->d_name[0] == '.')
                        if(dp->d_name[1] == '.' || dp->d_name[1] == '\0')
                                continue;

		/* construct absolute path */
		strlcpy(path, dirpath, PATH_MAX);
		//strcat(path, "/");
		strlcat(path, dp->d_name, PATH_MAX-len+1);
		printf("path=%s\n", path);

		if(stat(path, &st) != 0){
			printf("could not stat %s\n", path);
			continue;
		}
		strlcpy(flp->path, path, PATH_MAX);

		if(S_ISDIR(st.st_mode)){
			printf("diving into %s\n", path);
			dirpath = path;
			goto dir;
		}
	
		if(!(flp->info = create_ID3(flp->info))){
			printf("could not create ID3 for %s\n", flp->path);
			continue;
		}
		if(parse_file_ID3(flp->info, flp->path)){
			printf("could not parse ID3 for %s\n", flp->path);
			continue;
		}
		
		if((fmatch=id3lookup(flp, files)))
			printf("%s match with %s\n", flp->path, fmatch->path);
		
		flp->next = malloc(sizeof(flist));
		if(!flp->next){
			printf("out of memory\n");
			exit(1);
		}
		flp = flp->next;
	}
	flp->next = NULL;
	closedir(dirp);
	return files;
error:
	return NULL;
}

void
printfiles(flist *files)
{
	flist *flp;
	for(flp = files; flp->next; flp = flp->next){
		if(flp->info->mask & TITLE_TAG)
			printf("Title: %s\n", flp->info->title);
		if(flp->info->mask & ARTIST_TAG)
			printf("Artist: %s\n", flp->info->artist);
	}
}

int
main(int argc, char *argv[])
{
	flist *files;
	if(argc < 2)
		exit(1);
	
//	if(argv[1][strlen(argv[1])-1] == '/')
//		argv[1][strlen(argv[1])-1] = '/';
	files = id3dir(argv[1]);	
	return 0;
}

