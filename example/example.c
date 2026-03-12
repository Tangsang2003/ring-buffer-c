#include <stdio.h>
#include "ring_buffer.h"
#include <string.h>

int main(void) {
    ring_buffer_t rb;
    uint8_t buf[64];

    ring_buffer_init(&rb, buf, sizeof(buf));

    printf("\n<< Ring Buffer Example >>\n");
    printf("Capacity: %zu\n", rb_capacity(&rb));

    // Simulate receiving data (e.g. from UART)
    const char *memory[] = {
        "this", 
        "is", 
        "awesome", 
        "six seven",
        "loveIsGood",
        "Spiderman"
    };

    // Write messages
    printf("\n<< Writing messages >>\n");
    for (int i = 0; i < 6; i++) {
        size_t written = rb_write(&rb, (uint8_t*)memory[i], 
        strlen(memory[i]));
        printf("wrote %zu bytes  of \"%s\"\n", written, memory[i]);
    }
    printf("--------------------\n");
    printf("RB status: %zu of %zu bytes used\n", 
    rb_count(&rb), rb_capacity(&rb));

    printf("\n<< Reading from buffer >>\n");
    uint8_t read[32];
    while (!rb_is_empty(&rb)) {
        size_t size_available = rb_get_read_ptr(&rb, &(uint8_t*){read});
        printf("%zu contiguous bytes available\n", size_available);

        uint8_t chunk[10];
        size_t total_read = rb_read(&rb, chunk, sizeof(chunk) - 1);
        chunk[total_read] = '\0';
        printf("read: %s\n", chunk);
    }
    printf("--------------------\n");
    printf("Buff empty: %s\n", rb_is_empty(&rb) ? "yes": "no");

    return 0;
}