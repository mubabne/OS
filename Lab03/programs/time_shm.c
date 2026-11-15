#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    char shm_name[64];
    snprintf(shm_name, sizeof(shm_name), "/time_shm_%ld", (long)getpid());
    int fd = shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd == -1 || ftruncate(fd, sizeof(struct timeval)) == -1) {
        perror("shared memory setup");
        return EXIT_FAILURE;
    }
    struct timeval *start = mmap(NULL, sizeof(*start), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (start == MAP_FAILED) {
        perror("mmap");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (pid == 0) {
        gettimeofday(start, NULL);
        execvp(argv[1], &argv[1]);
        perror("execvp");
        _exit(errno == ENOENT ? 127 : 126);
    }

    int status;
    waitpid(pid, &status, 0);
    struct timeval end;
    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start->tv_sec) + (end.tv_usec - start->tv_usec) / 1000000.0;
    printf("Elapsed time: %.6f seconds\n", elapsed);

    munmap(start, sizeof(*start));
    close(fd);
    shm_unlink(shm_name);
    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAILURE;
}

