#include <stdio.h>
#include <gnu/libc-version.h>

int main() {
    const char *version = gnu_get_libc_version();
    if (version == NULL) {
        printf("Failed to get glibc version\n");
        return -1;
    }
    
    printf("GNU libc version: %s\n", version);
    return 0;
}