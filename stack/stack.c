// A stack implemented using a linked list

#include <stdbool.h>    // For bool, true, false
#include <stdio.h>      // For printf
#include <stdlib.h>     // For EXIT_SUCCESS, malloc, free

typedef struct node_t node_t;
struct node_t {
    int     data;
    node_t *next;
};

// Initialize the stack
static void initialize(node_t **head) {
    *head = NULL;
}

// Destroy the stack
static void destroy(node_t **head) {
    node_t *node;
    node_t *next;

    // Free any nodes that remain in the stack
    printf("Destroyed items:\n");
    for(node = *head; node != NULL; node = next) {
        next = node->next;
        printf("%d ", node->data);
        free(node);
    }
    printf("\n\n");

    // Head should no longer refer to anything
    *head = NULL;
}

// Is the stack empty?
static bool isempty(node_t *head) {
    return head == NULL;
}

// Push an item onto the stack
static void push(node_t **head, node_t *node) {
    // Node points to the previous head
    node->next = *head;

    // Node is now the head
    *head = node;
}

// Pop an item off the stack
static node_t * pop(node_t **head) {
    node_t *node = NULL;

    if(!isempty(*head)) {
        // Get the node at the head
        node = *head;

        // Head is now the next node
        *head = node->next;

        // Node should no longer refer to the head
        node->next = NULL;
    }
    return node;
}

// Reverse the order of items in the stack
static void reverse(node_t **head) {
    node_t *curr = *head;
    node_t *prev = NULL;
    node_t *next;

    while(curr != NULL) {
        next       = curr->next;
        curr->next = prev;
        prev       = curr;
        curr       = next;
    }

    *head = prev;
}

// Print the contents of the stack
static void print(node_t **head) {
    printf("Stack contents:\n");
    for(node_t *node = *head; node != NULL; node = node->next) {
        printf("%d ", node->data);
    }
    printf("\n\n");
}

int main(void) {
    // Initialize the stack
    node_t *head;
    initialize(&head);

    // Push some items onto the stack
    for(int i = 0; i < 10; i++) {
        node_t *node = malloc(sizeof(node_t));
        if(node != NULL) {
            node->data = i;
            push(&head, node);
        }
    }

    // Print the contents of the stack
    print(&head);

    // Pop some items from the stack
    printf("Popped items:\n");
    for(int i = 0; i < 5; i++) {
        node_t *node = pop(&head);
        printf("%d ", (int) node->data);
        free(node);
    }
    printf("\n\n");

    // Print the contents of the stack
    print(&head);

    // Reverse the stack
    reverse(&head);

    // Print the contents of the stack
    print(&head);

    // Destroy the stack
    destroy(&head);

    return EXIT_SUCCESS;
}
