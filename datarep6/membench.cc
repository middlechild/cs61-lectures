#include "membench.hh"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <atomic>

static unsigned noperations;
static std::atomic<unsigned long> answer;

void* benchmark_thread(void* user_data) {
    answer += memnode_benchmark(noperations, (size_t) user_data);
    return nullptr;
}

int main(int argc, char** argv) {
    noperations = 10000000;
    if (argc >= 2) {
        noperations = strtoul(argv[1], nullptr, 0);
    }
    int nthreads = 1;
    if (argc >= 3) {
        nthreads = strtol(argv[2], nullptr, 0);
    }

    if (nthreads <= 1) {
        // Run the benchmark.
        answer = memnode_benchmark(noperations, 1);
    } else {
        // Run `nthreads` threads, each running the benchmark.
        pthread_t threads[100];
        assert(nthreads <= 100);
        for (int i = 0; i != nthreads; ++i) {
            uintptr_t arg = 2 * i + 1;
            pthread_create(&threads[i], nullptr,
                           benchmark_thread, (void*) arg);
        }
        for (int i = 0; i != nthreads; ++i) {
            pthread_join(threads[i], nullptr);
        }
    }

    printf("answer: %lu\n", answer.load());
}
