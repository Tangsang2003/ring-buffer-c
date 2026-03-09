#include <stdio.h>
#include <assert.h>
#include "ring_buffer.h"

#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        printf("FAIL: %s (line %d)\n", msg, __LINE__); \
        return 1; \
    } \
    else { \
        printf("PASS: %s\n", msg); \
    } \
} while (0);

int test_basic_init(void) {
    printf("\n << TEST: Basic init >>\n");
    uint8_t buff[100];
    ring_buffer_t rb;

    TEST_ASSERT(ring_buffer_init(&rb, buff, 100) == true, 
    "init should succeed");

    return 0;
}

