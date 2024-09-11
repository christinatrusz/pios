#include "list.h"
#include <stddef.h>


// Add new element to linked list
void list_add(struct list_element *list_head, struct list_element *new_element) {
     if (list_head == NULL || new_element == NULL) {
        // Handle error or return early
        return;
    }    
   
    // Point new element to the current next element
    new_element->next = list_head->next; 
   
    // Upate list head to point to new element
    list_head->next = new_element; 
}


// Remove element from linked list 
void list_remove(struct list_element *head, struct list_element *element_to_remove) {
    if (head == NULL || element_to_remove == NULL) {
        // Handle error or return early
        return;
    }

    struct list_element *current = head;

    // Find the element's predecessor
    while (current != NULL && current->next != element_to_remove) {
        current = current->next;
    }

    // If element_to_remove was not found
    if (current == NULL) {
        return;
    }

    // Remove the element from the list
    current->next = element_to_remove->next;
}

