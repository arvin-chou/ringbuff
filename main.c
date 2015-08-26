#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ringbuff.h"

typedef struct circular_buffer
{
    void *buffer;     // data buffer
    void *buffer_end; // end of data buffer
    size_t capacity;  // maximum number of items in the buffer
    size_t count;     // number of items in the buffer
    size_t sz;        // size of each item in the buffer
    int overflow;    // overflow capacity
    void *head;       // pointer to head
    void *tail;       // pointer to tail
} circular_buffer;

void cb_init(circular_buffer *cb, size_t capacity, size_t sz)
{
    cb->buffer = malloc(capacity * sz);
    if(cb->buffer == NULL)
    {
        // handle error
        assert("XX");
    }
    cb->buffer_end = (char *)cb->buffer + capacity * sz;
    cb->capacity = capacity;
    cb->count = 0;
    cb->overflow = 0;
    cb->sz = sz;
    cb->head = cb->buffer;
    cb->tail = cb->buffer;
}

void cb_free(circular_buffer *cb)
{
    free(cb->buffer);
    // clear out other fields too, just to be safe
}

void cb_push_back(circular_buffer *cb, const void *item)
{
    //fprintf(stderr, "cb->count(%d)\n", cb->count);
    if(cb->count >= cb->capacity)
    {
        fprintf(stderr, "cb->count == cb->capacity, we remove oldest one\n");
        //cb->oldest = (cb->oldest == cb->capacity) ? cb->oldest++ : 0;
        //cb_pop_front(cb, NULL);
        cb->count = cb->count % cb->capacity;
        cb->overflow = 1;
        cb->tail = cb->buffer + cb->sz * (cb->count+1);
    }

    memcpy(cb->head, item, cb->sz);
    cb->head = (char*)cb->head + cb->sz;

    if(cb->head == cb->buffer_end)
        cb->head = cb->buffer;

    cb->count++;
}

/**
 * @breif peak one block data
 */
void cb_pop_front(circular_buffer *cb, void *item)
{
    if(cb->count == 0 && cb->overflow == 0)
    {
        fprintf(stderr, "cb->count == 0\n");
        return;
    }

    memcpy(item, cb->tail, cb->sz);

    cb->tail = (char*)cb->tail + cb->sz;
    if(cb->tail == cb->buffer_end)
        cb->tail = cb->buffer;
    cb->count--;
}

void _cb_clone(circular_buffer *dcb, circular_buffer *scb)
{
    dcb->buffer = scb->buffer ;
    dcb->buffer_end = scb->buffer_end ;
    dcb->capacity = scb->capacity ;
    dcb->count = scb->count ;
    dcb->overflow = scb->overflow ;
    dcb->sz = scb->sz ;
    dcb->head = scb->head ;
    dcb->tail = scb->tail ;
}

void _cb_print_all(circular_buffer *cb)
{
    int i;
    char buf[cb->sz];
    int capacity = 0 == cb->overflow ? cb->count : cb->capacity;

    for(i=0; i<capacity; i++)
    {
        cb_pop_front(cb, buf);
        //snprintf(buf, cb->sz, "%s", cb->buffer + (cb->sz * cb->count) + cb->sz * i);
        //printf(stderr, "data(%d) =%s\n", i, buf)
        fprintf(stderr, "data(%d) =%s\n", i, buf);
    }
}

/**
 * @breif print all data in order
 */
void cb_print_all(circular_buffer *cb)
{
    circular_buffer clone_cb, *pclone_cb;

    _cb_clone(&clone_cb, cb);
    _cb_print_all(&clone_cb);
}

/**
 * @breif print all data in order and remove
 */
void cb_print_all_remove(circular_buffer *cb)
{
    _cb_print_all(cb);
    cb->overflow = 0;
}



int main(int argc, char* argv[])
{
    RingBuffer* buf = RingBuffer_create(20);
    char *data = malloc(20), *result = malloc(20);
    int length;

    //fprintf(stderr, "---buffer at : %p\n", buf);
    //sprintf(data, "test\n");
    //length = 5;
    ////data[0] = 't';
    ////data[1] = 'e';
    ////length = 2;
    //length = RingBuffer_write(buf, data, length);
    ////RingBuffer_read(buf, result, length);
    ////fprintf(stderr, "---print from ringbuff: %s\n", result);
    ////*(data+length) = '\0';
    //fprintf(stderr, "---print from ringbuff(all): %s\n", (buf->buffer));

    sprintf(data, "12345678\n");
    length = 9;
    length = RingBuffer_write(buf, data, length);
    //RingBuffer_read(buf, result, length);
    //fprintf(stderr, "---print from ringbuff: %s\n", result);
    fprintf(stderr, "---print from ringbuff(all): %s\n", (buf->buffer));

    sprintf(data, "Compiler sees\n");
    length = 14;
    length = RingBuffer_write(buf, data, length);
    //RingBuffer_read(buf, result, length);
    fprintf(stderr, "---print from ringbuff: %s\n", result);


    fprintf(stderr, "---buffer at : %p\n", buf);
    fprintf(stderr, "---print from ringbuff(all): %s\n", (buf->buffer));

    RingBuffer_destroy(buf);

    circular_buffer cb;
    size_t sz = 10;
    size_t capacity = 5;
    char* item = malloc(sz);
    cb_init(&cb, capacity, sz);
    snprintf(item, sz, "1 The simplest solution would be to keep track\n");
    cb_push_back(&cb, item);

    snprintf(item, sz, "2 Circular buffer in JavaScript");
    cb_push_back(&cb, item);

    //snprintf(item, sz, "10 What are the uses of circular buffer?");
    //cb_push_back(&cb, item);

#if 1
    snprintf(item, sz, "3 How good is the memory mapped Circular Buffer on Wikipedia?");
    cb_push_back(&cb, item);

    snprintf(item, sz, "4 end pointer problem of a circular buffer");
    cb_push_back(&cb, item);
    snprintf(item, sz, "5 end pointer problem of a circular buffer");
    cb_push_back(&cb, item);
    snprintf(item, sz, "6 end pointer problem of a circular buffer");
    cb_push_back(&cb, item);
    snprintf(item, sz, "7 end pointer problem of a circular buffer");
    cb_push_back(&cb, item);
    snprintf(item, sz, "8 end pointer problem of a circular buffer");
    cb_push_back(&cb, item);
#endif

    int i;
    cb_print_all(&cb);
    fprintf(stderr, "add one: 9 \n");
    snprintf(item, sz, "9 end pointer problem of a circular buffer");
    cb_push_back(&cb, item);

    cb_print_all(&cb);

    fprintf(stderr, "before call cb_print_all_remove\n");
    cb_print_all_remove(&cb);
    fprintf(stderr, "remove it\n");
    cb_print_all_remove(&cb);
    fprintf(stderr, "end\n");

    return 0;
}
