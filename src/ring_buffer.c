#include "ring_buffer.h"
#include <assert.h>

bool ring_buffer_init(ring_buffer_t *rb, uint8_t *buf, size_t size) {
    if (rb == NULL || buf == NULL || size == 0) {
        return false;
    }
    rb->buffer = buf;
    rb->size = size;
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    rb->is_full = false;

    return true;
}

void ring_buffer_reset(ring_buffer_t *rb) {
    if (rb == NULL) {
       return;
    }

    rb->head = 0;
    rb-> tail = 0;
    rb->count = 0;
    rb->is_full = false;
}

bool rb_is_empty(const ring_buffer_t *rb) {
    assert(rb != NULL);
    if (rb->count == 0) {
        return true;
    }
    else {
        return false;
    }
}

bool rb_is_full(const ring_buffer_t *rb) {
    assert(rb != NULL);
    return (rb->count == rb->size);
}

size_t rb_count(const ring_buffer_t *rb) {
    assert(rb != NULL);
    return (rb->count);
}

size_t rb_capacity(const ring_buffer_t *rb) {
    assert(rb != NULL);
    return (rb->size);
}
