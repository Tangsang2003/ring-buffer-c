#include "ring_buffer.h"
#include <assert.h>
#include <string.h>

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

size_t rb_available(const ring_buffer_t *rb) {
    assert(rb != NULL);
    return (rb->size - rb->count);
}

size_t rb_read(ring_buffer_t *rb, uint8_t *data, size_t bytes) {
    assert(rb != NULL);
    assert(data != NULL);

    if (rb->count == 0 || bytes == 0) {
        return 0;
    }

    size_t to_read = (bytes < rb->count ? bytes : rb->count);
    size_t first_part = rb->size - rb->tail;

    if (first_part > to_read) {
        first_part = to_read;
    }
    memcpy(data, &rb->buffer[rb->tail], first_part);

    if (first_part < to_read) {
        size_t second_part = to_read - first_part;
        memcpy(&data[first_part], rb->buffer, second_part);
        rb->tail = second_part;
    }
    else {
        rb->tail = (rb->tail + to_read) % rb->size;
    }

    rb->count -= to_read;
    return to_read;
}

size_t rb_write(ring_buffer_t *rb, uint8_t *data, size_t size) {
    assert(rb != NULL);
    assert(data != NULL);

    if (rb->count == rb->size || size == 0) {
        return 0;
    }

    size_t to_write = (size < rb_available(rb) ? size : rb_available(rb));

    size_t first_part = rb->size - rb->head;
    if (first_part > to_write) {
        first_part = to_write;
    }
    memcpy(&rb->buffer[rb->head], data, first_part);

    if (first_part < to_write) {
        size_t second_part = to_write - first_part;
        memcpy(rb->buffer, &data[first_part], second_part);
        rb->head = second_part;
    }
    else {
        rb->head = (rb->head + to_write) % rb->size;
    }
    rb->count += to_write;

    return to_write;
}

size_t rb_peek(const ring_buffer_t *rb, size_t size,  uint8_t *data) {

    if (rb == NULL || data == NULL || rb->count == 0 || size == 0) {
        return 0;
    }

    size_t to_peek = (size < rb->count ? size : rb->count);

    size_t first_part, second_part;

    first_part = rb->size - rb->tail;
    if (first_part > to_peek) {
        first_part = to_peek;
    }
    memcpy(data, &rb->buffer[rb->tail], first_part);

    if (first_part < to_peek) {
        second_part = to_peek - first_part;
        memcpy(&data[first_part], rb->buffer, second_part);
    }

    return to_peek;
}

size_t rb_skip(ring_buffer_t *rb, size_t len){
    assert(rb != NULL);

    if (rb == NULL || len == 0 || rb->count == 0)
    {
        return 0;
    }

    size_t to_skip = len < rb->count ? len : rb->count;

    rb->tail = (rb->tail + to_skip) % rb->size;
    rb->count -= to_skip;

    return to_skip;
}

bool rb_put(ring_buffer_t *rb, uint8_t byte) {

    if (rb == NULL || rb->count == rb->size) {
        return false;
    }

    rb->buffer[rb->head] = byte;
    rb->head = (rb->head + 1) % rb->size;
    rb->count += 1;

    return true;
}

bool rb_get(ring_buffer_t *rb, uint8_t *byte) {

    if (rb == NULL || byte == NULL || rb-> count == 0) {
        return false;
    }
    *byte = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % rb->size;
    rb->count -= 1;

    return true;
}
