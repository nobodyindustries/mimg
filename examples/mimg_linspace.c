#include "../mimg.h"
#include <stdio.h>

void print_array(const char *name, stbi_uc *values, size_t length) {
    printf("%s - [", name);
    for(int i = 0; i < length; i ++) {
        printf("%d", values[i]);
        if(i != length - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main() {
    stbi_uc *values;

    values = calloc(6, sizeof (stbi_uc));
    mimg_linspace(values, 6, 0, 10);
    print_array("TEST 1", values, 6);
    free(values);

    values = calloc(6, sizeof (stbi_uc));
    mimg_linspace(values, 6, 2, 12);
    print_array("TEST 2", values, 6);
    free(values);

    values = calloc(12, sizeof (stbi_uc));
    mimg_linspace(values, 12, 0, 255);
    print_array("TEST 3", values, 12);
    free(values);

    values = calloc(255, sizeof (stbi_uc));
    mimg_linspace(values, 255, 0, 255);
    print_array("TEST 4", values, 255);
    free(values);

    values = calloc(27, sizeof (stbi_uc));
    mimg_linspace(values, 27, 0, 31);
    print_array("TEST 5", values, 27);
    free(values);

    return 0;
}