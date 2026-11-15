#include <stdio.h>
#include <stdlib.h>

#define MIN_PID 300
#define MAX_PID 5000
#define PID_COUNT (MAX_PID - MIN_PID + 1)

static unsigned char *used;

int allocate_map(void) {
    used = calloc(PID_COUNT, sizeof(*used));
    return used == NULL ? -1 : 1;
}

int allocate_pid(void) {
    if (used == NULL) return -1;
    for (int i = 0; i < PID_COUNT; ++i) {
        if (!used[i]) {
            used[i] = 1;
            return MIN_PID + i;
        }
    }
    return -1;
}

void release_pid(int pid) {
    if (used != NULL && pid >= MIN_PID && pid <= MAX_PID) {
        used[pid - MIN_PID] = 0;
    }
}

int main(void) {
    if (allocate_map() == -1) return EXIT_FAILURE;
    int first = allocate_pid();
    int second = allocate_pid();
    printf("Allocated: %d, %d\n", first, second);
    release_pid(first);
    printf("Reallocated: %d\n", allocate_pid());
    free(used);
    return EXIT_SUCCESS;
}

