#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sstream>

unsigned finish_checksum(const char*, unsigned) __attribute__((noinline));


unsigned checksum(const char* arg) {
    // initialize buffer
    char buf[100];
    memset(buf, 0, sizeof(buf));

    // copy string into a local buffer in lower case
    for (size_t i = 0; arg[i] != 0; ++i) {
        buf[i] = tolower((unsigned char) arg[i]);
    }

    // add 4-byte contiguous words
    unsigned u = 0;
    for (size_t i = 0; i < sizeof(buf); i += sizeof(unsigned)) {
        u += * (unsigned*) &buf[i];
    }
    return finish_checksum(buf, u);
}


unsigned finish_checksum(const char*, unsigned u) {
    return u;
}


int run_shell(const char* command) {
    fprintf(stderr, "Running %s\n", command);
    return system(command);
}


int main(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        printf("%8x %s\n", checksum(argv[i]), argv[i]);
    }
}
