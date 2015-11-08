// A singly linked list
//
// This supports the following operations:
//  compare_data    Comparison function used for insertion, search et al
//  create          Create a new stand-alone node
//  insert_head     Insert a new node at the head of the linked list
//  delete_head     Delete a node at the head of the linked list
//  destroy         Destroy a linked list
//  append_tail     Append a new node at the tail of the linked list
//  insert_between  Insert a new node between two existing nodes
//  insert_sorted   Insert a new node in sorted order
//  reverse         Reverse the linked list
//  print           Print the contents of the linked list
//
// TODO:
//  Delete tail
//  Get head
//  Get tail
//  Find

#include <errno.h>      // For errno
#include <stdbool.h>    // For bool, true, false
#include <stdio.h>      // For printf
#include <stdlib.h>     // For EXIT_SUCCESS, malloc, free
#include <string.h>     // For strerror

typedef struct node_t node_t;
struct node_t {
    int data;
    node_t *next;
};

typedef int (*compare_t)(int data1, int data2);

// Comparison function used for insertion, search et al
int compare_data(int data1, int data2) {
    // Or more simply: return data1 - data2;
    if(data1 < data2) {
        return -1;
    }
    else if(data1 > data2) {
        return 1;
    }
    else {
        return 0;
    }
}

// Create a new stand-alone node
node_t * create(int data) {
    node_t *node = malloc(sizeof(node_t));
    if(node == NULL) {
        printf("%s", strerror(errno));
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    return node;
}

// Insert a new node at the head of the linked list
bool insert_head(node_t **head, int data) {
    node_t *new = create(data);
    if(new == NULL) {
        printf("Failed to insert");
        return false;
    }
    new->next = *head;
    *head     = new;
    return true;
}

// Delete a node at the head of the linked list
bool delete_head(node_t **head) {
    node_t *node = *head;
    if(node == NULL) {
        printf("List was empty\n");
        return false;
    }
    *head = node->next;
    free(node);
    return true;
}

// Destroy a linked list
void destroy(node_t **head) {
    while(*head != NULL) {
        delete_head(head);
    }
}

// Append a new node at the tail of the linked list
bool append_tail(node_t **tail, int data) {
    if(*tail == NULL) {
        printf("Bad arguments");
        return false;
    }

    node_t *new = create(data);
    if(new == NULL) {
        printf("Failed to insert");
        return false;
    }
    (*tail)->next = new;
    return true;
}

// Insert a new node between two existing nodes
bool insert_between(node_t *after, int data, node_t *before) {
    if((after == NULL) || (before == NULL)) {
        printf("Bad arguments");
        return false;
    }

    node_t *new = create(data);
    if(new == NULL) {
        printf("Failed to insert");
        return false;
    }
    after->next = new;
    new->next   = before;
    return true;
}

// Insert a new node in sorted order
bool insert_sorted(node_t **head, int data, compare_t compare) {
    // If the list is empty or the new node sorts before the head
    if((*head == NULL) || (compare(data, (*head)->data) < 0)) {
        // Insert at the head
        printf("Inserting %d at head\n", data);
        return insert_head(head, data);
    }
    // The list is not empty or the new node sorts after the head
    else {
        node_t *curr = *head;
        node_t *next;

        while(curr != NULL) {
            next = curr->next;

            // Do not insert duplicates
            if(compare(data, curr->data) == 0) {
                printf("Not inserting duplicate: %d\n", data);
                return false;
            }
            // If reached the end of the list, append at the end
            else if(next == NULL) {
                printf("Appending %d at tail\n", data);
                return append_tail(&curr, data);
            }
            // If new sorts before next, insert between curr and next
            else if(compare(data, next->data) < 0) {
                printf("Inserting %d between %d and %d\n", data, curr->data, next->data);
                return insert_between(curr, data, next);
            }
            // Move on through the list
            else {
                curr = next;
            }
        }
    }

    printf("Should never get here\n");
    return false;
}

// Reverse the linked list
//
//  A: prev = null, curr = head, next = curr->next
//  B: prev   <--   curr
//  C:              prev         curr
void reverse(node_t **head) {
    node_t *prev = NULL;
    node_t *curr = *head;
    node_t *next;

    while(curr != NULL) {
        next       = curr->next;
        curr->next = prev;
        prev       = curr;
        curr       = next;
    }
    *head = prev;
}

// Print the contents of the linked list
void print(node_t *head) {
    while(head != NULL) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

int main(void) {
    // Create a new singly linked list
    node_t *head = create(0);
    if(head == NULL) {
        return EXIT_FAILURE;
    }

    // Insert some nodes
    printf("\nInsert some nodes:\n");
    bool result = true;
    for(int i = 1; i < 10 && result == true; i++) {
        result = insert_head(&head, i);
    }
    print(head);

    // Reverse the linked list
    printf("\nReverse the linked list:\n");
    reverse(&head);
    print(head);

    // Delete some nodes
    printf("\nDelete some nodes:\n");
    for(int i = 0; i < 5; i++) {
        delete_head(&head);
    }
    print(head);

    // Destroy the linked list
    destroy(&head);

    // Insert in sorted order
    printf("\nInsert some nodes in sorted order:\n");
    node_t *sorted = NULL;
    insert_sorted(&sorted, 2, compare_data); // empty list
    insert_sorted(&sorted, 4, compare_data); // append to tail
    insert_sorted(&sorted, 3, compare_data); // insert in middle
    insert_sorted(&sorted, 1, compare_data); // insert at head
    insert_sorted(&sorted, 2, compare_data); // duplicate
    print(sorted);

    // Destroy the linked list
    destroy(&sorted);

    return EXIT_SUCCESS;
}
