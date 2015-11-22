#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>    // For bool, true, false

#define NELEMENTS(a) (sizeof(a) / sizeof(a[0]))

// A node in a singly-linked list
typedef struct node_t node_t;
struct node_t {
    int     data;
    node_t* next;
};

// Make a new node
node_t* new(int data) {
    node_t* node = malloc(sizeof(node_t));
    if(node != NULL) {
        node->data = data;
        node->next = NULL;
    }
    return node;
}

// Insert a new node at the head of a list
bool insert_head(node_t** head, int data) {
    if(head == NULL) {
        return false;
    }

    // Make the new node
    node_t* node = new(data);
    if(node == NULL) {
        return false;
    }

    // Insert the new node at the head
    node->next = *head;
    *head      = node;
    return true;
}

// Delete an existing node from the head of a list
bool delete_head(node_t** head) {
    if(head == NULL) {
        return false;
    }

    if(*head == NULL) {
        // The list was empty
        return false;
    }

    // Delete the existing node at the head
    node_t* node = *head;
    *head = node->next;
    free(node);
    return true;
}

// Reverse a list
bool reverse(node_t** head) {
    if(head == NULL) {
        return false;
    }

    node_t* prev = NULL;
    node_t* curr = *head;
    node_t* next = NULL;

    while(curr != NULL) {
        // Remember next
        next = curr->next;

        // Current points back to previous
        curr->next = prev;

        // Move on to the next node
        prev = curr;
        curr = next;
    }

    // Update the head
    *head = prev;
    return true;
}

// Destroy a list
bool destroy(node_t** head) {
    if(head == NULL) {
        return false;
    }

    while(*head != NULL) {
        if(false == delete_head(head)) {
            return false;
        }
    }
    return true;
}

// Insert a new node after an existing node
bool insert_after(node_t *after, int data) {
    if(after == NULL) {
        return false;
    }

    // Make the new node
    node_t* node = new(data);
    if(node == NULL) {
        return false;
    }

    /// Insert after the node (and before the next if it exists)
    if(after->next != NULL) {
        node->next = after->next;
    }
    after->next = node;
    return true;
}

// Comparison function to compare the data of two nodes
typedef int (*compare_t)(int a, int b);
int compare_data(int a, int b) {
    if(a == b) {
        return 0;
    }
    else if(a < b) {
        return -1;
    }
    else {
        return 1;
    }
}

// Insert a new node in sorted order
bool insert_sorted(node_t** head, int data, compare_t compare) {
    if(head == NULL) {
        return false;
    }

    // Empty list - insert at the head
    if(*head == NULL) {
        return insert_head(head, data);
    }

    // Sorts before head - insert at the head
    if(compare(data, (*head)->data) < 0) {
        return insert_head(head, data);
    }

    // Search through the list
    node_t* prev = NULL;
    node_t* curr = *head;
    node_t* next = NULL;
    while(curr != NULL) {
        // Compare against the current node
        int result = compare(data, curr->data);
        if(result == 0) {
            // Duplicate - do not insert
            printf("Duplicate: %d\n", data);
            return false;
        }
        else if(result < 0) {
            // Sorts before the current node - insert before the current node
//            return insert_between(prev, data, curr);
            return insert_after(prev, data);
        }
        else {
            // Sorts after the current node - keep searching
            next = curr->next;
            prev = curr;
            curr = next;
        }
    }

    // Reached the tail - insert after the tail
    return insert_after(prev, data);
}

// Print a list
void print(node_t* node) {
    printf("List: ");
    while(node != NULL) {
        printf("%d ", node->data);
        node = node->next;
    }
    printf("\n\n");
}

int main(void) {
    // Insert a new node at the head of a bad list
    if(false == insert_head(NULL, 0)) {
        printf("Insert a new node at the head of a bad list: OK\n\n");
    }

    // Insert a new node into an empty list
    node_t* head = NULL;
    if(true == insert_head(&head, 0)) {
        printf("Insert a new node into an empty list: OK\n");
        print(head);
    }
    else {
        printf("Insert a new node into an empty list: FAIL\n\n");
    }

    // Insert some nodes into an existing list
    bool result = false;
    for(int i = 1; i < 20; i++) {
        result = insert_head(&head, i);
        if(result == false) {
            printf("Insert some nodes into an existing list: FAIL\n\n");
            break;
        }
    }
    if(result == true) {
        printf("Insert some nodes into an existing list: OK\n");
        print(head);
    }

    // Delete a node from the head of a bad list
    if(false == delete_head(NULL)) {
        printf("Delete a node from the head of a bad list: OK\n\n");
    }

    // Delete a node from an empty list
    node_t *empty = NULL;
    if(false == delete_head(&empty)) {
        printf("Delete a node from an empty list: OK\n\n");
    }

    // Delete some nodes from an existing list
    for(int i = 0; i < 10; i++) {
        result = delete_head(&head);
        if(result == false) {
            printf("Delete some nodes from an existing list: FAIL\n\n");
            break;
        }
    }
    if(result == true) {
        printf("Delete some nodes from an existing list: OK\n");
        print(head);
    }

    // Reverse a bad list
    if(false == reverse(NULL)) {
        printf("Reverse a bad list: OK\n\n");
    }

    // Reverse an existing list
    if(true == reverse(&head)) {
        printf("Reverse an existing list: OK\n");
        print(head);
    }
    else {
        printf("Reverse an existing list: FAIL\n\n");
    }

    // Destroy a bad list
    if(false == destroy(NULL)) {
        printf("Destroy a bad list: OK\n\n");
    }

    // Destroy an empty list
    if(true == destroy(&empty)) {
        printf("Destroy an empty list: OK\n\n");
    }
    else {
        printf("Destroy an empty list: FAIL\n\n");
    }

    // Destroy an existing list
    if(true == destroy(&head)) {
        printf("Destroy an existing list: OK\n");
        print(head);
    }
    else {
        printf("Destroy an existing list: FAIL\n\n");
    }

    // Insert some nodes in sorted order
    printf("\nInsert some nodes in sorted order:\n");
    node_t *sorted = NULL;
    insert_sorted(&sorted, 2, compare_data); // empty list
    insert_sorted(&sorted, 4, compare_data); // append to tail
    insert_sorted(&sorted, 3, compare_data); // insert in middle
    insert_sorted(&sorted, 1, compare_data); // insert at head
    insert_sorted(&sorted, 2, compare_data); // duplicate
    print(sorted);

    // Clean-up
    destroy(&sorted);

    return EXIT_SUCCESS;
}
