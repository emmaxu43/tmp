CC = gcc
CFLAGS = -Wall -Werror

test-subseq: maxSeq.o test-subseq.o
	$(CC) $(CFLAGS) -o test-subseq maxSeq.o test-subseq.o

maxSeq.o: maxSeq.c
	$(CC) $(CFLAGS) -c maxSeq.c

test-subseq.o: test-subseq.c
	$(CC) $(CFLAGS) -c test-subseq.c

