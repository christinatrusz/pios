#include "list.h"
//#include <stdef.h>
#include "rprintf.h"
#include "serial.h"
#include "mmu.h"
#include "fat.h"
#include "delays.h"
#include "sd.h"


extern struct table_descriptor_stage1 L1table[]; 

char glbl[128];

// Updated for sd.c
void wait_msec(int ms) {
    volatile int count; 
    for (int i = 0; i < ms; i++) {
        for (count = 0; count < 1000; count++);
    }
}

int get_current_el() {
    unsigned int el;
    asm("mrs %0, CurrentEL"
     : "=r"(el)
     :
     :);
    return el>>2;
}

void kernel_main() {

   // Homework 8: FAT Filesystem Driver

   // Initialize the FAT filesystem
    if (fatInit() != 0) {
        // Handle error in initialization
        rprintf("FAT Initialization Failed\n");
        return -1;
    }

    // Open a file 
    int file_descriptor = fatOpen("/BIN/BASH");
    if (file_descriptor < 0) {
        // Handle error in opening the file
        rprintf("Failed to open file: %d\n", file_descriptor);
        return -1;
    }

    // Read the file contents into the buffer
    int bytes_read = fatRead(file_descriptor, buffer, BUFFER_SIZE);
    if (bytes_read < 0) {
        // Handle error in reading the file
        rprintf("Failed to read file: %d\n", bytes_read);
        return -1;
    }

    extern int __bss_start, __bss_end;
    char *bssstart, *bssend;
    
    // Homework 7 page mapping 
    mapPages((void*)0x0, (void*)0x0);
    int result = loadPageTable(L1table);

    esp_printf( my_putc, "Current Execution Level is %d\r\n", get_current_el());

    bssstart = &__bss_start;
    bssend = &__bss_end;

    // Loop to clear bss from start to end
    for(char *p = bssstart; p < bssend; p++) {
	*p = 0; //Sets each byte to zero
    }

    while(1){
    }
}

