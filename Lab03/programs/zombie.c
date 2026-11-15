#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (pid == 0) {
        printf("Child PID: %ld\n", (long)getpid());
        return EXIT_SUCCESS;
    }

    printf("Parent PID: %ld, zombie child: %ld\n", (long)getpid(), (long)pid);
    printf("The child remains a zombie for 10 seconds. Check with: ps -l\n");
    sleep(10);
    waitpid(pid, NULL, 0);
    puts("Child reaped.");
    return EXIT_SUCCESS;
}

