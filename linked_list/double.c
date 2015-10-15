// A doubly linked list
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
    node_t *prev;
    node_t *next;
};

// Create a new stand-alone node
node_t * create(int data) {
    node_t *node = malloc(sizeof(node_t));
    if(node == NULL) {
        printf("ERROR! %s", strerror(errno));
        return NULL;
    }
    node->data = data;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

// Insert a new node at the head of the linked list
bool insert_head(node_t **head, int data) {
    node_t *new = create(data);
    if(new == NULL) {
        return false;
    }
    (*head)->prev = new;
    new->next     = *head;
    *head         = new;
    return true;
}

// Delete a node at the head of the linked list
int delete_head(node_t **head) {
    node_t *node = *head;
    int data = node->data;

    *head         = node->next;
    (*head)->prev = NULL;
    free(node);

    return data;
}

// Reverse the linked list
//
//  A: prev = null, curr = head, next = curr->next
//  B: prev   <--   curr   -->   next
//     next   <--   curr   -->   prev
//  C:              prev         curr
void reverse(node_t **head) {
    node_t *prev = NULL;
    node_t *curr = *head;
    node_t *next;
    node_t *temp;

    while(curr != NULL) {
        next       = curr->next;

        temp       = curr->next;
        curr->next = curr->prev;
        curr->prev = temp;

        prev       = curr;
        curr       = next;
    }

    *head = prev;
}

// Print the contents of the linked list
void print(node_t *node) {
    while(node != NULL) {
        printf("Node: %d, prev = %p, curr = %p, next = %p\n", node->data, node->prev, node, node->next);
        node = node->next;
    }
}

int main(void) {
    // Create a new doubly linked list
    node_t *head = create(0);
    if(head == NULL) {
        return EXIT_FAILURE;
    }
    print(head);

    // Insert some nodes
    bool result = true;
    for(int i = 1; i < 10 && result == true; i++) {
        result = insert_head(&head, i);
    }
    print(head);

    // Reverse the linked list
    reverse(&head);
    print(head);

    // Delete some nodes
    for(int i = 0; i < 5; i++) {
        int data = delete_head(&head);
        printf("Deleted: %d\n", data);
    }
    print(head);

    return EXIT_SUCCESS;
}
