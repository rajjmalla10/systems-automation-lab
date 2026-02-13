#include "proc_status.h"
#include <stdio.h>
#include <stdlib.h> 
int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <PID>\n", argv[0]);
        return 1;
    }

    pid_t pid = atoi(argv[1]);
    struct proc_info info;

    if(read_proc_full_info(pid, &info) != 0) {
        fprintf(stderr, "Failed to read full info for PID %d\n", pid);
        return 1;
    }

    printf("PID: %d\n", info.pid);
    printf("Name: %s\n", info.name);
    printf("utime: %ld\n", info.utime);
    printf("stime: %ld\n", info.stime);

    if(info.utime > 0 || info.stime > 0) {
        printf("CPU info is valid!\n");
    } else {
        printf("CPU info is NOT valid (utime=0, stime=0)\n");
    }

    return 0;
}
