CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
POSIX_LIBS = -lrt

TARGETS = zombie time_pipe time_shm pid_manager collatz_fork collatz_shm reverse_case_pipes filecopy

all: $(TARGETS)

zombie: zombie.c
	$(CC) $(CFLAGS) -o $@ $<

time_pipe: time_pipe.c
	$(CC) $(CFLAGS) -o $@ $<

time_shm: time_shm.c
	$(CC) $(CFLAGS) -o $@ $< $(POSIX_LIBS)

pid_manager: pid_manager.c
	$(CC) $(CFLAGS) -o $@ $<

collatz_fork: collatz_fork.c
	$(CC) $(CFLAGS) -o $@ $<

collatz_shm: collatz_shm.c
	$(CC) $(CFLAGS) -o $@ $< $(POSIX_LIBS)

reverse_case_pipes: reverse_case_pipes.c
	$(CC) $(CFLAGS) -o $@ $<

filecopy: filecopy.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS) *.class copy.txt

.PHONY: all clean

