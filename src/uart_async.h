#pragma once

void uart_init_async();
void uart_write_async( char c );
void uart_write_str_async( char * str );
char uart_read_async();
