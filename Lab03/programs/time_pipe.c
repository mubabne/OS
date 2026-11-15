#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (pid == 0) {
        close(fd[0]);
        struct timeval start;
        gettimeofday(&start, NULL);
        if (write(fd[1], &start, sizeof(start)) != (ssize_t)sizeof(start)) {
            perror("write");
            _exit(EXIT_FAILURE);
        }
        close(fd[1]);
        execvp(argv[1], &argv[1]);
        perror("execvp");
        _exit(errno == ENOENT ? 127 : 126);
    }

    close(fd[1]);
    struct timeval start, end;
    if (read(fd[0], &start, sizeof(start)) != (ssize_t)sizeof(start)) {
        perror("read");
        return EXIT_FAILURE;
    }
    close(fd[0]);
    int status;
    waitpid(pid, &status, 0);
    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("Elapsed time: %.6f seconds\n", elapsed);
    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAILURE;
}

