// shell getconf
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("No. of clock ticks per soc: %ld\n", sysconf(_SC_CLK_TCK));
    return 0;
}
