#include <stdio.h>
#include "rprintf.h"

#define MU_IO 0x3F215040  // For Pi 3, use 0xFE215004 for Pi 4

void my_putc(int data) {
    // Cast the address to a pointer and assign the character data
    *(volatile unsigned int *)MU_IO = data;
}

