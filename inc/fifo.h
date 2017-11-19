#pragma once

typedef struct fifo_node {
  unsigned char val;
  struct fifo_node* next;
} fifo_node_t;

typedef struct fifo {
  fifo_node_t* front;
  fifo_node_t* back;
} fifo_t;

void enqueue( fifo_t* fifo, unsigned char val );

unsigned char dequeue( fifo_t* fifo );

int fifo_get_size( fifo_t* fifo );
