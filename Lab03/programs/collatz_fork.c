#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int parse_positive(const char *text, unsigned long long *value) {
    char *end;
    errno = 0;
    unsigned long long parsed = strtoull(text, &end, 10);
    if (errno || *text == '\0' || *end != '\0' || parsed == 0) return 0;
    *value = parsed;
    return 1;
}

int main(int argc, char *argv[]) {
    unsigned long long n;
    if (argc != 2 || !parse_positive(argv[1], &n)) {
        fprintf(stderr, "Usage: %s <positive integer>\n", argv[0]);
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (pid == 0) {
        while (1) {
            printf("%llu%s", n, n == 1 ? "\n" : ", ");
            if (n == 1) break;
            if (n % 2 == 0) n /= 2;
            else {
                if (n > (ULLONG_MAX - 1) / 3) {
                    fprintf(stderr, "overflow\n");
                    _exit(EXIT_FAILURE);
                }
                n = 3 * n + 1;
            }
        }
        fflush(stdout);
        _exit(EXIT_SUCCESS);
    }
    waitpid(pid, NULL, 0);
    return EXIT_SUCCESS;
}

