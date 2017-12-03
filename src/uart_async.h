#pragma once

void uart_init_async();
void uart_write_async( unsigned char c );
void uart_write_str_async( unsigned char * str );
unsigned char uart_read_async();