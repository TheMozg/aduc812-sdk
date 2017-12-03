#pragma once

void uart_init_blocking();
void uart_write_blocking( unsigned char c );
unsigned char uart_read_blocking();