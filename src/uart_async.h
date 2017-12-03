#pragma once

void uart_init_async();
void uart_write_async( unsigned char c );
unsigned char uart_read_async();