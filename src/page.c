#include "page.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  // For uintptr_t

// Statically allocate an array of struct ppages:
struct ppage physical_page_array[128];  

struct ppage *free_pages_list = NULL;  

// Initialize the list of free physical pages:
void init_pfa_list(void) {
    for (int i = 0; i < 128; i++) {
        physical_page_array[i].physical_addr = (void *)((uintptr_t)(i * 0x200000));  // Correct cast
        physical_page_array[i].next = (i == 127) ? NULL : &physical_page_array[i + 1];
        physical_page_array[i].prev = (i == 0) ? NULL : &physical_page_array[i - 1];
    }
    free_pages_list = &physical_page_array[0];  // Set free list head
}

// Allocate npages physical pages from the free list:
struct ppage *allocate_physical_pages(unsigned int npages) {
    struct ppage *allocated_list = NULL;
    struct ppage *current = free_pages_list;

    for (unsigned int i = 0; i < npages && current != NULL; i++) {
        struct ppage *next = current->next;

        // Unlink current page from the free list
        if (next != NULL) {
            next->prev = NULL;
        }
        free_pages_list = next;

        // Add current page to the allocated list
        current->next = allocated_list;
        allocated_list = current;
        current = free_pages_list;
    }

    return allocated_list;  // Return the head of the allocated list
}

// Return a list of pages to the free list:
void free_physical_pages(struct ppage *ppage_list) {
    while (ppage_list != NULL) {
        struct ppage *next = ppage_list->next;

        // Insert the page back at the beginning of the free list
        ppage_list->next = free_pages_list;
        if (free_pages_list != NULL) {
            free_pages_list->prev = ppage_list;
        }
        free_pages_list = ppage_list;

        ppage_list = next;
    }
}

