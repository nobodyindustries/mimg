#include "../mimg.h"

int main() {
    stbi_uc *values, median;

    // TEST 1
    values = calloc(6, sizeof (stbi_uc));
    values[0] = 1;
    values[1] = 0;
    values[2] = 3;
    values[3] = 4;
    values[4] = 5;
    values[5] = 2;
    median = mimg_mediani(values, 6);
    assert(median == 3);
    printf("TEST 1: %d\n", median);
    free(values);

    // TEST 2
    values = calloc(5, sizeof (stbi_uc));
    values[0] = 1;
    values[1] = 0;
    values[2] = 3;
    values[3] = 4;
    values[4] = 5;
    median = mimg_mediani(values, 5);
    assert(median == 3);
    printf("TEST 2: %d\n", median);
    free(values);

    // TEST 3
    values = calloc(7, sizeof (stbi_uc));
    values[0] = 1;
    values[1] = 0;
    values[2] = 0;
    values[3] = 4;
    values[4] = 4;
    values[5] = 4;
    values[6] = 4;
    median = mimg_mediani(values, 7);
    assert(median == 4);
    printf("TEST 3: %d\n", median);
    free(values);

    // TEST 4
    values = calloc(7, sizeof (stbi_uc));
    values[0] = 1;
    values[1] = 0;
    values[2] = 0;
    values[3] = 100;
    values[4] = 100;
    values[5] = 100;
    values[6] = 100;
    median = mimg_mediani(values, 3);
    assert(median == 0);
    printf("TEST 4: %d\n", median);
    free(values);

    return 0;
}