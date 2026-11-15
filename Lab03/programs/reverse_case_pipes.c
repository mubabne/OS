#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2 || strlen(argv[1]) >= BUFFER_SIZE) {
        fprintf(stderr, "Usage: %s <message under %d bytes>\n", argv[0], BUFFER_SIZE);
        return EXIT_FAILURE;
    }

    int parent_to_child[2], child_to_parent[2];
    if (pipe(parent_to_child) == -1 || pipe(child_to_parent) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (pid == 0) {
        close(parent_to_child[1]);
        close(child_to_parent[0]);
        char buffer[BUFFER_SIZE];
        ssize_t count = read(parent_to_child[0], buffer, sizeof(buffer));
        if (count <= 0) _exit(EXIT_FAILURE);
        for (ssize_t i = 0; i < count; ++i) {
            unsigned char c = (unsigned char)buffer[i];
            buffer[i] = isupper(c) ? (char)tolower(c) : islower(c) ? (char)toupper(c) : (char)c;
        }
        if (write(child_to_parent[1], buffer, (size_t)count) != count) _exit(EXIT_FAILURE);
        close(parent_to_child[0]);
        close(child_to_parent[1]);
        _exit(EXIT_SUCCESS);
    }

    close(parent_to_child[0]);
    close(child_to_parent[1]);
    size_t length = strlen(argv[1]) + 1;
    write(parent_to_child[1], argv[1], length);
    close(parent_to_child[1]);
    char result[BUFFER_SIZE];
    ssize_t count = read(child_to_parent[0], result, sizeof(result));
    close(child_to_parent[0]);
    waitpid(pid, NULL, 0);
    if (count <= 0) return EXIT_FAILURE;
    printf("%s\n", result);
    return EXIT_SUCCESS;
}

