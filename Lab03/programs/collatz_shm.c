#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHM_SIZE 65536

int main(int argc, char *argv[]) {
    char *end;
    errno = 0;
    unsigned long long n = argc == 2 ? strtoull(argv[1], &end, 10) : 0;
    if (argc != 2 || errno || *argv[1] == '\0' || *end != '\0' || n == 0) {
        fprintf(stderr, "Usage: %s <positive integer>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char name[64];
    snprintf(name, sizeof(name), "/collatz_%ld", (long)getpid());
    int fd = shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd == -1 || ftruncate(fd, SHM_SIZE) == -1) {
        perror("shared memory setup");
        return EXIT_FAILURE;
    }
    char *memory = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (memory == MAP_FAILED) {
        perror("mmap");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (pid == 0) {
        size_t used = 0;
        while (1) {
            int written = snprintf(memory + used, SHM_SIZE - used, "%llu%s", n, n == 1 ? "\n" : ", ");
            if (written < 0 || (size_t)written >= SHM_SIZE - used) _exit(EXIT_FAILURE);
            used += (size_t)written;
            if (n == 1) break;
            if (n % 2 == 0) n /= 2;
            else {
                if (n > (ULLONG_MAX - 1) / 3) _exit(EXIT_FAILURE);
                n = 3 * n + 1;
            }
        }
        _exit(EXIT_SUCCESS);
    }

    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) fputs(memory, stdout);
    munmap(memory, SHM_SIZE);
    close(fd);
    shm_unlink(name);
    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAILURE;
}

