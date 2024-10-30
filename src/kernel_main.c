#include "list.h"
#include <stddef.h>
#include <stdio.h>
#include "rprintf.h"
#include "serial.h"
#include "mmu.h"
#include "fat.h"
#include "delays.h"
#include "sd.h"

extern struct table_descriptor_stage1 L1table[]; 

char glbl[128];

int get_current_el() {
    unsigned int el;
    asm("mrs %0, CurrentEL"
     : "=r"(el)
     :
     :);
    return el>>2;
}

void *memcpy(void *dest, const void *src, size_t n) {
    char *d = dest;
    const char *s = src;
    while (n--) {
	*d++ = *s++;
    }
    return dest;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    while (n-- > 0) {
	if (*s1 != *s2) {
	    return *(unsigned char *)s1 - *(unsigned char *)s2;
	}
	if (*s1 == '\0') {
	    break;
	}
	s1++;
	s2++;
    }
    return 0;
}

void kernel_main() {
    get_timer_count();
    wait_msec(1000);

    extern int __bss_start, __bss_end;
    char *bssstart, *bssend;
    
    // Homework 8: FAT FS

    // Initialize SD
    if(sd_init() != SD_OK) {
        esp_printf(my_putc, "Failed to initialize SD card\n");
	return -1;
    }

    // Initialize FAT fs 
    if(fatInit() != 0) {
	esp_printf(my_putc, "Failed to initialize FAT filesystem\n");
	return -1;
    }

    // Open the file
    struct file *file_handle = fatOpen("mnt/disk/test.txt");
    if(file_handle == NULL) {
	esp_printf(my_putc, "Failed to open file\n");
	return -1;
    }

    // Read the contents into buffer
    uint8_t buffer[CLUSTER_SIZE];
    int bytes_read = fatRead(file_handle, buffer, sizeof(buffer));
    if(bytes_read < 0) {
	esp_printf(my_putc, "Failed to read file data\n");
	return -1;
    }

    esp_printf(my_putc, "Read %d bytes from %s:\n", bytes_read, file_handle->rde.file_name);
    for(int i = 0; i < bytes_read; i++) {
	esp_printf(my_putc, "%c ", buffer[i]);
    }
    esp_printf(my_putc, "\n");

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

