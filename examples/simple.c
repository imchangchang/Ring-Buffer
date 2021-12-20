#include <assert.h>
#include <stdio.h>
#include "../ringbuffer.h"

#define TEST_BUFFER_SIZE 256

static int test_buffer[TEST_BUFFER_SIZE];

typedef int test_t;

int main (void)
{
    int  cnt;
    test_t buf;

    /* Create and initialize ring buffer */
    ring_buffer_t ring_buffer;
    ring_buffer_init (&ring_buffer,
                      test_buffer,
                      sizeof(test_buffer[0]),
                      sizeof(test_buffer)/sizeof(test_buffer[0]));

    /* Add elements to buffer; one at a time */
    for (int e = 0; e < TEST_BUFFER_SIZE - 1; e++) {
        ring_buffer_queue (&ring_buffer, (void*) &e);
    }
    /* Verify size */
    assert(ring_buffer_num_items(&ring_buffer) == TEST_BUFFER_SIZE - 1);

    /* Peek third element */
    const test_t * peek_data = (const test_t*)ring_buffer_peek (&ring_buffer, 3);
    /* Assert byte returned */
    assert(peek_data != NULL);
    /* Assert contents */
    assert(*peek_data == 3);

    /* Dequeue all elements */
    for (cnt = 0; ring_buffer_dequeue (&ring_buffer, &buf) > 0; cnt++){
        /* Do something with buf... */
        assert(buf == cnt);
        printf ("Read: %d\n", buf);
    }
    printf ("\n===============\n");

    /* Add array */
    test_t test_buf_arr[20];
    ring_buffer_queue_arr (&ring_buffer, test_buf_arr, 20);

    /* Is buffer empty? */
    assert(!ring_buffer_is_empty (&ring_buffer));

    /* Dequeue all elements */
    while (ring_buffer_dequeue (&ring_buffer, &buf) > 0)
    {
        /* Print contents */
        printf ("Read: %d\n", buf);
    }
    ring_buffer_queue_arr (&ring_buffer, test_buf_arr, 20);
    assert(!ring_buffer_is_empty (&ring_buffer));
    ring_buffer_dequeue_arr(&ring_buffer, test_buf_arr, 20);
    assert(ring_buffer_is_empty (&ring_buffer));

    printf ("\n===============\n");

    /* Overfill buffer */
    for (int i = 0; i < 1000; i++){
        ring_buffer_queue (&ring_buffer, &i);
    }

    /* Is buffer full? */
    if (ring_buffer_is_full (&ring_buffer)) {
        cnt = ring_buffer_num_items (&ring_buffer);
        printf ("Buffer is full and contains %d bytes\n", cnt);
    }

    /* Dequeue all elements */
    while (ring_buffer_dequeue (&ring_buffer, &buf) > 0)
    {
        /* Print contents */
        printf ("Read: %d\n", buf);
    }
    return 0;
}
