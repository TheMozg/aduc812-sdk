#include <stdio.h>
#include <stdlib.h>
#include "fifo.h"

/*
fifo_node_t* fifo_get_prev_node( fifo_t* fifo, fifo_node_t* node ) {
  fifo_node_t* tmp = fifo->front;
  
  while( tmp->next != node ) {
    tmp = tmp->next;
  }

  return tmp;
}*/

void enqueue( fifo_t* fifo, unsigned char val ) {
  fifo_node_t* node = malloc( sizeof(fifo_node_t) );

  node->val = val;
  node->next = NULL;  

  if( fifo->front == NULL ) {
    fifo->front = node;
  } else {
    fifo->back->next = node;
  }

  fifo->back = node;

}

unsigned char dequeue( fifo_t* fifo ) {
  unsigned char ret;

  fifo_node_t* front = fifo->front;

  if( front == NULL ) return -1;
                        
  ret = front->val;
  fifo->front = front->next;

  free( front );
  return ret;
}

int fifo_get_size( fifo_t* fifo ) {
  // Not implemented
  return 0;
}
