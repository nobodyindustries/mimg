#include <stdint.h>
#include <stdio.h>

#include "../mimg.h"

#define N 10
#define MAX_TRIES 100000000

int main() {
    uint32_t count[N] = {0};
    uint32_t r;
    uint64_t sum = 0;
    for(int i = 0; i < MAX_TRIES; i++) {
         r = mimg_random_range(2, N);
         count[r]++;
    }
    for(int i = 0; i < N; i++) {
        printf("%d - %d\n", i, count[i]);
        sum += count[i];
    }
    printf("TOTAL: %lld\n", sum);
}