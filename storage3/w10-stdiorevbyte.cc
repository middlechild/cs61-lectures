#include "iobench.hh"

int main() {
    FILE* f = stdout;
    if (isatty(fileno(f))) {
        f = fopen(DATAFILE, "w");
    }
    if (!f) {
        perror("fopen");
        exit(1);
    }

    size_t size = 51200000;
    const char* buf = "6";
    double start = tstamp();

    size_t pos = size;
    size_t n = 0;
    while (pos > 0) {
        pos -= 1;
        if (fseek(f, pos, SEEK_SET) == -1) {
            perror("fseek");
            exit(1);
        }
        size_t r = fwrite(buf, 1, 1, f);
        if (r != 1) {
            perror("fwrite");
            exit(1);
        }
        n += r;
        if (n % PRINT_FREQUENCY == 0) {
            report(n, tstamp() - start);
        }
    }

    fclose(f);
    report(n, tstamp() - start);
    fprintf(stderr, "\n");
}
