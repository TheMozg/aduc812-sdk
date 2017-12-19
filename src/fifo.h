#pragma once

#define FIFO_SIZE 20

void fifo_init( char* fifo );

void fifo_enqueue( char* fifo, char c );

char fifo_dequeue( char* fifo );

char fifo_peek( char* fifo );

unsigned int fifo_get_size( char* fifo );
