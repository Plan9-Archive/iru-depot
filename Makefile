# public domain
# Iruata Souza

CC=cc
CFLAGS=-Wall

%*:%.c
	$(CC) $(CFLAGS) -o $@ $@.c
%*:%.s
	$(CC) $(CFLAGS) -o $@ $@.s

clean:
	rm -f *.o
	for f in $(file * | grep -i elf | awk '{print $1}' | sed 's/\://'); do rm -f $f; done
