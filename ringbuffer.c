#include "ringbuffer.h"
#include <string.h>

#define RING_BUFFER_MASK (buffer->element_num-1)

void ring_buffer_init (ring_buffer_t *buffer, void *ebuffer, size_t esize, size_t e_num){
    buffer->element_buffer = ebuffer;
    buffer->element_size = esize;
    buffer->element_num = e_num;
    buffer->tail_index = 0;
    buffer->head_index = 0;

    //element num must be a power of two
    if ((buffer->element_num & (buffer->element_num - 1)) != 0) {
        buffer->element_num--;
    }
}

void ring_buffer_queue (ring_buffer_t *buffer, void *edata){
    /* Is buffer full? */
    if (ring_buffer_is_full (buffer)) {
        /* Is going to overwrite the oldest byte */
        /* Increase tail index */
        buffer->tail_index = ((buffer->tail_index + 1) & RING_BUFFER_MASK);
    }

    /* Place data in buffer */
    memcpy ((uint8_t*) buffer->element_buffer + buffer->head_index * buffer->element_size,
            edata,
            buffer->element_size);
    buffer->head_index = ((buffer->head_index + 1) & RING_BUFFER_MASK);
}

void ring_buffer_queue_arr (ring_buffer_t *buffer, const void *edata, size_t e_num){
    /* Add bytes; one by one */
    for (size_t i = 0; i < e_num; i++) {
        ring_buffer_queue (buffer, (uint8_t*) edata + i * buffer->element_size);
    }
}

bool ring_buffer_dequeue (ring_buffer_t *buffer, void *edata){
    if (ring_buffer_is_empty (buffer)) {
        return false;
    }
    memcpy (edata,
            (uint8_t*) buffer->element_buffer + buffer->tail_index * buffer->element_size,
            buffer->element_size);
    buffer->tail_index = ((buffer->tail_index + 1) & RING_BUFFER_MASK);
    return true;
}

size_t ring_buffer_dequeue_arr (ring_buffer_t *buffer, void *edata, size_t e_num){
    if (ring_buffer_is_empty (buffer)) {
        return 0;
    }

    uint8_t *data_ptr = edata;
    size_t cnt = 0;
    while ((cnt < e_num) && ring_buffer_dequeue (buffer, data_ptr)){
        cnt++;
        data_ptr += buffer->element_size;
    }
    return cnt;
}

const void* ring_buffer_peek (ring_buffer_t *buffer, size_t index)
{
    if (index >= ring_buffer_num_items (buffer)) {
        /* No items at index */
        return NULL;
    }

    /* Add index to pointer */
    size_t data_index = ((buffer->tail_index + index) & RING_BUFFER_MASK);
    return (const void*) ((uint8_t*) buffer->element_buffer + data_index * buffer->element_size);
}

extern inline bool ring_buffer_is_empty (ring_buffer_t *buffer);
extern inline bool ring_buffer_is_full (ring_buffer_t *buffer);
extern inline size_t ring_buffer_num_items (ring_buffer_t *buffer);

