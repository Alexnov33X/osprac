#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
int main() {
    int fp[2];
    size_t size = 1;
    int counter = 0;

    if (pipe(fp) < 0) {
        printf("Cannot create pipe\n");
        exit(-1);
    }

    fcntl(fp[1], F_SETFL, fcntl(fp[1], F_GETFL) | O_NONBLOCK);

    while (size == 1) {
        size = write(fp[1], "a", 1);
        counter++;
    }
    printf("Pipe max size is %d\n", counter);
    return 0;
}