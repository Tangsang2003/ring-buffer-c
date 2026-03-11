#include <stdio.h>
#include <assert.h>
#include "ring_buffer.h"
#include <string.h>

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
    printf("\n<< TEST: Basic init >>\n");
    uint8_t buff[100];
    ring_buffer_t rb;

    TEST_ASSERT(ring_buffer_init(&rb, buff, 100) == true, 
    "init should succeed");
    TEST_ASSERT(rb_is_empty(&rb) == true, "should be empty after init");
    TEST_ASSERT(rb_is_full(&rb) == false, "shouldn't be full after init");
    TEST_ASSERT(rb_count(&rb) == 0, "count should be 0");
    TEST_ASSERT(rb_capacity(&rb) == 100, "capacity should  be 100");
    TEST_ASSERT(rb_available(&rb) == 100, "available space should be 100");

    return 0;
}

int single_byte_test(void) {
    printf("\n <<TEST: single byte operations >>\n");

    uint8_t buf[5];
    ring_buffer_t rb;
    uint8_t read_byte; 

    ring_buffer_init(&rb, buf, 5);

    TEST_ASSERT(rb_put(&rb, 0x69) == true, "put successful");
    TEST_ASSERT(rb_count(&rb) == 1, "count should be 1");
    TEST_ASSERT(rb_get(&rb, &read_byte) == true, "get successful");
    TEST_ASSERT(read_byte == 0x69, "read data is correct");
    TEST_ASSERT(rb_is_empty(&rb) == true, "rb is empty after get");

    for (int i = 0; i < 5; i++) {
        char msg[50];
        sprintf(msg, "put op.%d successful", i + 1);
        TEST_ASSERT(rb_put(&rb,  (uint8_t)i) == true, msg);
    }
    TEST_ASSERT(rb_is_full(&rb) == true, "should be full");
    TEST_ASSERT(rb_put(&rb, 0x42) == false, "put after full should fail");

    for (int i = 0; i < 5; i++) {
        char msg[25];
        sprintf(msg, "get op.%d successful", i + 1);
        TEST_ASSERT(rb_get(&rb, &read_byte) == true, msg);
        TEST_ASSERT(read_byte == (uint8_t)i, "FIFO");
    }
    TEST_ASSERT(rb_is_empty(&rb) == true, "should be empty");
    TEST_ASSERT(rb_get(&rb, &read_byte) == false, "get on empty should fail");

    return 0;
}

int test_bulk_operations(void) {
    printf("\n<< TEST: Bulk operations >>\n");

    uint8_t buf[100];
    ring_buffer_t rb;
    ring_buffer_init(&rb, buf, 100);

    uint8_t write_data[] = "Tangsang was here and wrote this to test bulk ops.";
    uint8_t read_data[100] = {0};

    size_t written_bytes = rb_write(&rb, &write_data[0], 
        strlen((char*)write_data) + 1);
    printf("wrote %zu bytes\n", written_bytes);

    TEST_ASSERT(written_bytes == strlen((char*)write_data) + 1, 
    "should  write all data");
    TEST_ASSERT(rb_count(&rb) == strlen((char*)write_data) + 1, 
    "count should match written length");

    // Read data back
    size_t read_bytes = rb_read(&rb, &read_data[0], sizeof(read_data));
    TEST_ASSERT(read_bytes == written_bytes, 
        "read and write bytes should be same");
    TEST_ASSERT(strcmp((char*)read_data, (char*)write_data) == 0, 
    "read and write data should match");
    
    return 0;
}

int test_peek_and_skip(void) {

    printf("\n<< TEST: peek and skip >>\n");
    printf("-- PEEK --\n");
    uint8_t buf[8];
    uint8_t data[5] = {1, 2, 3, 4, 5};
    uint8_t peek_buff[8];
    ring_buffer_t rb;

    ring_buffer_init(&rb, buf, 8);
    rb_write(&rb, data, 5);

    size_t peeked = rb_peek(&rb, 3, peek_buff);

    TEST_ASSERT(peeked == 3, "should peek 3 bytes");
    TEST_ASSERT(peek_buff[0] == 1 && peek_buff[1] == 2 && peek_buff[2] == 3, 
    "peeked data is correct");
    TEST_ASSERT(rb_count(&rb) == 5, "count should be unchanged after peek");

    printf("-- SKIP --\n");

    size_t skip = rb_skip(&rb, 2);
    TEST_ASSERT(skip == 2, "should skip 2 bytes");
    TEST_ASSERT(rb.count == 3, "should have 3 byte left");

    uint8_t read_buf[5];
    size_t read_bytes = rb_read(&rb, read_buf, 3);
    TEST_ASSERT(read_bytes == 3, "should read 3 bytes");
    TEST_ASSERT(read_buf[0] == 3 && read_buf[1] == 4 &&read_buf[2] == 5, "read data correct");

    return 0;
}

int test_zero_copy_operations(void) {
    printf("\n<<TEST: zero copy >>\n");

    ring_buffer_t rb;
    uint8_t *ptr;
    uint8_t buf[16];
    size_t len;

    ring_buffer_init(&rb, buf, 16);

    len = rb_get_write_ptr(&rb, &ptr);
    
    TEST_ASSERT(len == 16, 
    "num of contiguous bytes available to write should be 16");

    // DMA write simulate
    memcpy(ptr, "dmaWrite", 8);
    rb_advance_write(&rb, 8);
    TEST_ASSERT(rb_count(&rb) == 8, "count should be 8");

    // DMA read
    len = rb_get_read_ptr(&rb, &ptr);
    TEST_ASSERT(len == 8, 
    "num of contiguous bytes available to read should be 8");
    TEST_ASSERT(memcmp(ptr, "dmaWrite", 8) == 0, "read should match write");
    rb_advance_read(&rb, 5);
    TEST_ASSERT((&rb)->count, "should have 3 bytes remaining");

    return 0;
}

int test_wrap_around(void) {
    printf("\n<<TEST: wrap around >>\n");

    uint8_t buf[8];
    ring_buffer_t rb;
    ring_buffer_init(&rb, buf, 8);

    // [ 1, 2, 3, 4, 5, 6, 7, 8]
    for (int i = 0; i < 8; i++) {
        rb_put(&rb, (uint8_t)(i + 1));    
    }

    // Remove three items
    // [_, _, _, 4, 5, 6, 7, 8]
    uint8_t temp;
    for (int i = 0; i < 3; i++) {
        rb_get(&rb, &temp);
    }

    // Insert new items
    // [9, 10, 11, 4, 5, 6, 7, 8]
    for (int i = 9; i < 12; i++) {
        rb_put(&rb, (uint8_t)(i));
    }

    uint8_t expected[] = {4, 5, 6, 7, 8, 9, 10, 11}; // tail is at 4

    for (int i = 0; i < 8; i++) {
        rb_get(&rb, &temp);
        TEST_ASSERT(temp == expected[i], "wrap around correct");
    }
    return 0;
}

int test_edge_cases(void) {
    printf("\n<< TEST: edge cases >>\n");

    ring_buffer_t rb;
    uint8_t buf[5];
    uint8_t data[10];

    // Null pointers
    TEST_ASSERT(ring_buffer_init(NULL, buf, 5) == false, 
    "init with NULL rb should fail");
    TEST_ASSERT(ring_buffer_init(&rb, NULL, 5) == false, 
    "init with NULL buf should fail");
    TEST_ASSERT(ring_buffer_init(&rb, buf, 0) == false, 
    "init with size 0 should fail");

    ring_buffer_init(&rb, buf, 5);

    // Read on empty
    TEST_ASSERT(rb_read(&rb, data, 1) == 0, 
    "read on empty should return 0");
    TEST_ASSERT(rb_get(&rb, data) == false, "get on empty should be false");
    TEST_ASSERT(rb_skip(&rb, 1) == 0, "skip on empty should be 0");

    // Write larger than buffer
    uint8_t large_data[] = "larger than 8 bytes";
    size_t written = rb_write(&rb, large_data, sizeof(large_data));
    TEST_ASSERT(written == 5, "only 5 bytes must be written");
    TEST_ASSERT(rb_is_full(&rb), "buffer should be full");

    // Reset
    ring_buffer_reset(&rb);
    TEST_ASSERT(rb_count(&rb) == 0, "count should be 0 after reset");
    TEST_ASSERT(rb_is_empty(&rb) == true, "rb should be empty after reset");

    return 0;
}

int test_partial(void) {
    uint8_t buf[16];
    ring_buffer_t rb;
    uint8_t read_data[16];

    ring_buffer_init(&rb, buf, 16);

    uint8_t write_data[] = "LIFEisGood";

    rb_write(&rb, write_data, 6);

    // read more than available
    size_t read = rb_read(&rb, read_data, 16);
    TEST_ASSERT(read == 6, "only 6 bytes must be read");
    TEST_ASSERT(memcmp(read_data, "LIFEis", 6) == 0, "read data should be correct");

    // fill completely
    size_t written = rb_write(&rb, write_data, 10);
    written += rb_write(&rb, write_data, 10); // completely filled now

    TEST_ASSERT(written == 16, "should write 16 bytes to fill");

    written = rb_write(&rb, write_data, 10); // written should be 0 now since
                                             // completely filled
    TEST_ASSERT(written == 0, "0 bytes should be written after fill");

    return 0;
}

int main() {

    printf("\n<< -- Performing Tests on ring buffer operations -- >>\n");

    int failures = 0;
    failures += test_basic_init();
    failures += single_byte_test();
    failures += test_bulk_operations();
    failures += test_wrap_around();
    failures += test_peek_and_skip();
    failures += test_zero_copy_operations();
    failures += test_partial();
    failures += test_edge_cases();

    if (failures == 0) {
        printf("\n----- ALL Tests Passed :) -----\n");
    }

    else {
        printf("\n----- Oops! %d Test failed :( -----\n", failures);
    }

    return failures;
}
