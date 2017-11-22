#include <stdio.h>
#include <stdlib.h>
#include "fifo.h"

void fifo_init( fifo_t* fifo ) {
  fifo->front = NULL;
  fifo->back = NULL;
}

void enqueue( fifo_t* fifo, unsigned char val ) {
  //if( val == NULL ) return;

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
  int i = 0;
  fifo_node_t* tmp = fifo->front;
  if( tmp == NULL ) return 0;

  do {
    i++;
    tmp = tmp->next;
  } while( tmp->next != NULL );
  return i;
}
