#include <inttypes.h>
#include <stdbool.h>

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

typedef struct ring_buffer_t ring_buffer_t;

struct ring_buffer_t
{
    void * element_buffer;
    size_t element_size;
    size_t element_num;
    size_t tail_index;
    size_t head_index;
};

void ring_buffer_init (ring_buffer_t *buffer, void *ebuffer, size_t esize, size_t e_num);
void ring_buffer_queue (ring_buffer_t *buffer, void *edata);
void ring_buffer_queue_arr (ring_buffer_t *buffer, const void *edata, size_t e_num);
bool ring_buffer_dequeue (ring_buffer_t *buffer, void *edata);
size_t ring_buffer_dequeue_arr (ring_buffer_t *buffer, void *edata, size_t e_num);
const void *ring_buffer_peek (ring_buffer_t *buffer, size_t index);

inline bool ring_buffer_is_empty (ring_buffer_t *buffer){
    return (buffer->head_index == buffer->tail_index);
}

inline bool ring_buffer_is_full (ring_buffer_t *buffer){
    return ((buffer->head_index - buffer->tail_index) &
            (buffer->element_num-1)) == (buffer->element_num-1);
}

inline size_t ring_buffer_num_items (ring_buffer_t *buffer){
    return ((buffer->head_index - buffer->tail_index) & (buffer->element_num-1));
}

#endif /* RINGBUFFER_H */
