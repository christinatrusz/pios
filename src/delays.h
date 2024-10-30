#ifndef DELAYS_H
#define DELAYS_H

// Function declarations for delay and UART utility functions
void uart_puts(const char *str);
void wait_msec(int msec);
void wait_cycles(int cycles);

#endif  // DELAYS_H

