#include "delays.h"
#include <stdio.h>  // For printf (used in uart_puts)
#include <unistd.h> // For usleep (used in wait_msec)
#include <stdlib.h>
#include "rprintf.h"
#include "serial.h"

// Sends a string over UART (simulated here with printf)
void uart_puts(const char *str) {
    esp_printf(my_putc, "%s", str);
}

// Waits for a specified number of milliseconds
void wait_msec(int msec) {
    volatile int count;
    for(int i = 0; i < msec; i++) {
	for(count = 0; count < 1000; count++);
    }
}

// Waits for a specified number of CPU cycles (placeholder implementation)
void wait_cycles(int cycles) {
    // Simple placeholder delay loop based on cycles
    for (int i = 0; i < cycles * 1000; i++) {
        asm volatile ("nop");
    }
}

