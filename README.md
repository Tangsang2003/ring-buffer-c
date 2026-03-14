# Ring Buffer in C

A ring buffer implementation in C for embedded applications.

## Features 
- No dynamic allocation (Zero heap usage)
- Automatic wrap-around
- Zero-copy operations
- Constant-time execution

## API reference

### Initialization and basic information

| Function | Description |
| -------- | -------- |
| `ring_buffer_init` | Initialize buffer with pre-allocated memory |
|`ring_buffer_reset`| Clear all data and empty the buffer |
| `rb_capacity` | Gives total capacity of buffer in bytes |
| `rb_is_empty` | Check if ring buffer is empty or not | 
| `rb_is_full` | Check if ring buffer is full or not | 
| `rb_count` | Gives total number of bytes currently occupied |
| `rb_available` | Gives total bytes available to be written |

### Data Operations
| Function | Description |
| -------- | ----------- |
| `rb_write` | Write multiple bytes to the buffer - returns actual number of bytes written | 
| `rb_read` | Read multiple bytes from the buffer - returns actual number of bytes read |
| `rb_peek` | Reads data from buffer without removing them | 
| `rb_skip` | Removes data from buffer without reading |
| `rb_put` | Writes single byte to the buffer |
| `rb_get` | Reads single byte from the buffer |

### Zero-copy operations
| Function | Description |
| -------- | ----------- |
| `rb_get_read_ptr` | Get pointer to contiguous write space | 
| `rb_get_read_ptr` | Get pointer to contiguous read space | 
| `rb_advance_write` | Advance write pointer after external write | 
| `rb_advance_read` | Advance read pointer after external read | 

## Build and Usage
### Build Targets
```bash 
make # build tests and example
make test # run unit tests
make example # run usage example
make clean # remove build artifacts and .gdbhistory
```
- See usage example [here.](./example/example.c)
- See tests and operations [here.](./tests/tests.c)
