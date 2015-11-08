// A singly linked list
//
// This supports the following operations:
//  create          Create a new stand-alone node
//  insert_head     Insert a new node at the head of the linked list
//  delete_head     Delete a node at the head of the linked list
//  reverse         Reverse the linked list
//  print           Print the contents of the linked list
//
// TODO:
//  Destroy
//  Insert tail
//  Delete tail
//  Insert after
//  Insert before
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
        printf("List was empty");
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

    return EXIT_SUCCESS;
}
