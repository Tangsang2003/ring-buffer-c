/**
 * @file ring_buffer.h
 * @author Tangsang Chongbang
 * @brief Circular buffer implementation
 * @version 0.1
 * @date 2026-03-09
 */

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Structure representing a ring buffer
 * 
 */
typedef struct {
    uint8_t *buffer; // Pointer to the data buffer
    size_t head;     // Write index
    size_t tail;     // Read index
    size_t size;     // size of the buffer
    size_t count;    // Number of elements in the buffer
    bool is_full;    // Indicates whether the buffer is full or not, haven't
                     // used this in our implementation
} ring_buffer_t;

/**
 * @brief Initialize the ring buffer
 * 
 * @param    rb                  ptr to ring buffer structure to initialize
 * @param    buf                 ptr to allocated memory for the buffer 
 *                               (size of rb->size bytes)
 * @param    size                size of the buffer in bytes
 * @return true on success
 * @return false on invalid parameters
 */
bool ring_buffer_init(ring_buffer_t *rb, uint8_t *buf, size_t size);

/**
 * @brief reset the ring buffer to empty state
 * 
 * @param    rb                  ptr to ring buffer
 */
void ring_buffer_reset(ring_buffer_t *rb);

/**
 * @brief check if ring buffer is empty
 * 
 * @param    rb                  ptr to ring buffer
 * @return true if ring buff is empty
 * @return false if ring buff is not empty
 */
bool rb_is_empty(const ring_buffer_t *rb);

/**
 * @brief check if ring buffer is full
 * 
 * @param    rb                  ptr to ring buff
 * @return true if ring buff is full 
 * @return false if ring buff is not full
 */
bool rb_is_full(const ring_buffer_t *rb);

/**
 * @brief get num of bytes currently stored in the ring buf
 * 
 * @param    rb                  ptr to ring buf
 * @return   size_t              num of bytes available
 */
size_t rb_count(const ring_buffer_t *rb);

/**
 * @brief get available space in ring buf
 * 
 * @param    rb                  ptr to ring buf
 * @return   size_t              available bytes in ring buf to write
 */
size_t rb_available(const ring_buffer_t *rb);

/**
 * @brief get total capacity of buf
 * 
 * @param    rb                  ptr to ring buf
 * @return   size_t              total size of buf in bytes
 */
size_t rb_capacity(const ring_buffer_t *rb);

/**
 * @brief read data from ring buffer
 * 
 * @param    rb                  ptr to ring buf
 * @param    data                ptr to memory/buffer to store the read data
 * @param    size                max num of bytes to read
 * @return   size_t              num of bytes actually read
 */
size_t rb_read(ring_buffer_t *rb, uint8_t *data, size_t size);

/**
 * @brief write data to ring buf
 * 
 * @param    rb                  ptr to ring buff
 * @param    data                ptr to memory/buf from where to read data to
 *                               write / ptr to data to write
 * @param    size                num of bytes to write
 * @return   size_t              num of bytes actually written (maybe less if
 *                               available size is less / buf is full)
 */
size_t rb_write(ring_buffer_t *rb, uint8_t *data, size_t size);

/**
 * @brief read data from ring buf without deleting it
 * 
 * @param    rb                  ptr to ring buf
 * @param    size                num of bytes to peek
 * @param    data                ptr to buf to store peeked data
 * @return   size_t              num of bytes actually peeked
 */
size_t rb_peek(const ring_buffer_t *rb, size_t size, uint8_t *data);

/**
 * @brief skip data in ring buf without reading
 * 
 * @param    rb                  ptr to ring buf
 * @param    len                 num of bytes to skip
 * @return   size_t              num of bytes actually skipped
 */
size_t rb_skip(ring_buffer_t *rb, size_t len);

/**
 * @brief insert a single byte to the ring buf
 * 
 * @param    rb                  ptr to ring buf
 * @param    data                byte to be written
 * @return   true                if write success
 * @return   false               if buff full
 */
bool rb_put(ring_buffer_t *rb, uint8_t byte);

/**
 * @brief read a single byte from ring buf
 * 
 * @param    rb                  ptr to ring buf
 * @param    byte                ptr to byte where the read byte to be written
 * @return   true                if the byte read
 * @return   false               if buf empty
 */
bool rb_get(ring_buffer_t *rb, uint8_t *byte);

/**
 * @brief get ptr to linear contiguous read buff (if available)
 * useful in DMA or zero-copy operations
 * 
 * @param    rb                  ptr to ring buf
 * @param    ptr                 ptr to store buf ptr
 * @return   size_t              num of contiguous bytes available to read
 */
size_t rb_get_read_ptr(const ring_buffer_t *rb, uint8_t **ptr);

/**
 * @brief get ptr to linear contiguous write buff (if available)
 * useful in DMA or zero-copy operations
 * 
 * @param    rb                  ptr to ring buf
 * @param    ptr                 ptr to store buf ptr
 * @return   size_t              number of contiguous bytes available to write
 */
size_t rb_get_write_ptr(const ring_buffer_t *rb, uint8_t **ptr);

/**
 * @brief advance read ptr after external read (use with get_read_ptr)
 * 
 * @param    rb                  ptr to ring buf
 * @param    len                 num of bytes to advance
 */
void rb_advance_read(ring_buffer_t *rb, size_t len);

/**
 * @brief advance write ptr after external write (use with get_write_ptr)
 * 
 * @param    rb                  ptr to ring buf
 * @param    len                 num of bytes to advance
 */
void rb_advance_write(ring_buffer_t *rb, size_t len);

#ifdef __cplusplus
}
#endif

#endif