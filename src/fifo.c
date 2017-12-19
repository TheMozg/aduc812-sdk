#include "fifo.h"

void fifo_init( char* fifo ) {
    unsigned int i;
    for (i = 0; i < FIFO_SIZE; i++) {
        fifo[i]='\0';
    }
}

void fifo_enqueue( char* fifo, char c ) {
    unsigned int count = 0;
    while (*fifo) {
        count++;
        fifo++;
    }
    if (count < FIFO_SIZE - 1) {
        *fifo = c;
    }
}

char fifo_dequeue( char* fifo ) {
    unsigned int i;
    char c = fifo[0];
    for (i = 0; i < FIFO_SIZE - 1; i++) {
        fifo[i] = fifo[i+1];
    }
    return c;
}

char fifo_peek( char* fifo ) {
    return fifo[0];
}

unsigned int fifo_get_size( char* fifo ) {
    unsigned int count = 0;
    while (*fifo) {
        count++;
        fifo++;
    }
    return count;
}
