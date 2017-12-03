#include <stdio.h>
#include <stdlib.h>
#include "fifo.h"

void fifo_init( fifo_t* fifo ) {
    fifo->front = NULL;
    fifo->back = NULL;
}

void fifo_enqueue( fifo_t* fifo, unsigned char val ) {
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

unsigned char fifo_dequeue( fifo_t* fifo ) {
    unsigned char ret;

    fifo_node_t* front = fifo->front;

    if( front == NULL ) return '!';
                          
    ret = front->val;
    fifo->front = front->next;

    free( front );
    return ret;
}

unsigned char fifo_peek( fifo_t* fifo ) {
    if( fifo->front != NULL ) {
        return fifo->front->val;
    }                    
    return '!';
}

unsigned int fifo_get_size( fifo_t* fifo ) {
    unsigned int i = 0;
    fifo_node_t* tmp = fifo->front;
    if( tmp == NULL ) return 0;

    i++;
    while( tmp->next != NULL ) {
        i++;
        tmp = tmp->next;
    }

    return i;
}