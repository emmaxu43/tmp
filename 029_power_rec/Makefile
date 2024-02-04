CC = gcc
CFLAGS = -Wall -g
OBJS = power.o test-power.o

all: test-power

test-power: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

power.o: power.c
	$(CC) $(CFLAGS) -c power.c

test-power.o: test-power.c
	$(CC) $(CFLAGS) -c test-power.c

clean:
	rm -f *.o test-power

