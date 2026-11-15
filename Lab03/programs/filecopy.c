#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

static int write_all(int fd, const char *buffer, ssize_t count) {
    ssize_t offset = 0;
    while (offset < count) {
        ssize_t written = write(fd, buffer + offset, (size_t)(count - offset));
        if (written <= 0) return -1;
        offset += written;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int input = open(argv[1], O_RDONLY);
    if (input == -1) {
        perror("open source");
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
        close(fd[1]);
        close(input);
        int output = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output == -1) {
            perror("open destination");
            _exit(EXIT_FAILURE);
        }
        char buffer[BUFFER_SIZE];
        ssize_t count;
        while ((count = read(fd[0], buffer, sizeof(buffer))) > 0) {
            if (write_all(output, buffer, count) == -1) _exit(EXIT_FAILURE);
        }
        close(output);
        close(fd[0]);
        _exit(count < 0 ? EXIT_FAILURE : EXIT_SUCCESS);
    }

    close(fd[0]);
    char buffer[BUFFER_SIZE];
    ssize_t count;
    int failed = 0;
    while ((count = read(input, buffer, sizeof(buffer))) > 0) {
        if (write_all(fd[1], buffer, count) == -1) {
            failed = 1;
            break;
        }
    }
    if (count < 0) failed = 1;
    close(input);
    close(fd[1]);
    int status;
    waitpid(pid, &status, 0);
    return failed || !WIFEXITED(status) || WEXITSTATUS(status) != 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}

