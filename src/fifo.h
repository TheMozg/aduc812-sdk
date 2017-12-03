#pragma once

typedef struct fifo_node_t {
    unsigned char val;
    struct fifo_node_t* next;
} fifo_node_t;

typedef struct {
    fifo_node_t* front;
    fifo_node_t* back;
} fifo_t;

void fifo_init( fifo_t* fifo );

void fifo_enqueue( fifo_t* fifo, unsigned char val );

unsigned char fifo_dequeue( fifo_t* fifo );

unsigned char fifo_peek( fifo_t* fifo );

unsigned int fifo_get_size( fifo_t* fifo );
