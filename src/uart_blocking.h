#pragma once

void uart_init_blocking();
void uart_write_blocking( char c );
char uart_read_blocking();
