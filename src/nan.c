#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static void print_bits(uint8_t *bytes, size_t byte_size) {
    for (int i = (int) byte_size; i >= 0; --i) {
        uint8_t byte = bytes[i];
        for (int j = 7; j >= 0; --j) {
            printf("%d", !!(byte & (1 << j)));
        }
        printf(" ");
    }
    printf("\n");
}

#define INSPECT_DOUBLE(name, value) \
    { \
        double name = (value); \
        printf("%lf\t=\t ", value); \
        print_bits((uint8_t *)&name, sizeof(name)); \
    }

int main(void) {
    INSPECT_DOUBLE(pi, 3.14);
    INSPECT_DOUBLE(one, 1.0);
    INSPECT_DOUBLE(minus, -1.0);
    INSPECT_DOUBLE(inf, 1.0 / 0.0);
    INSPECT_DOUBLE(minf, -1.0 / 0.0);
    INSPECT_DOUBLE(nan, 0.0 / 0.0);
    INSPECT_DOUBLE(minus_zero, -0.0);
    return 0;
}