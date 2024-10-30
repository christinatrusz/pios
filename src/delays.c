#include "delays.h"
#include <stdio.h>  // For printf (used in uart_puts)
#include <unistd.h> // For usleep (used in wait_msec)

// Sends a string over UART (simulated here with printf)
void uart_puts(const char *str) {
    printf("%s", str);
}

// Waits for a specified number of milliseconds
void wait_msec(int msec) {
    usleep(msec * 1000);  // usleep expects microseconds, so multiply by 1000
}

// Waits for a specified number of CPU cycles (placeholder implementation)
void wait_cycles(int cycles) {
    // Simple placeholder delay loop based on cycles
    for (int i = 0; i < cycles * 1000; i++) {
        asm volatile ("nop");
    }
}

