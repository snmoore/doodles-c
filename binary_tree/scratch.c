#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>    // For bool, true, false
#include <string.h>

#define NELEMENTS(a) (sizeof(a) / sizeof(a[0]))

// Comparison function to compare two values
typedef int (*compare_t)(int a, int b);
int compare_values(int a, int b) {
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

// Structure for a node
typedef struct node_t node_t;
struct node_t {
    int data;
    node_t* left;
    node_t* right;
};

// Make a new node
node_t* new(int data) {
    node_t* node = malloc(sizeof(node_t));
    if(node != NULL) {
        node->data  = data;
        node->left  = NULL;
        node->right = NULL;
    }
    return node;
}

// Insert a new node into a tree
bool insert(node_t** tree, int data, compare_t compare) {
    if(tree == NULL) {
        // Bad tree
        return false;
    }

    // Is the current node empty?
    if(*tree == NULL) {
        // Make and insert the new node here
        *tree = new(data);
        if(*tree == NULL) {
            printf("Bad create\n");
            return false;
        }
        else {
            return true;
        }
    }
    else {
        // Compare against the current node
        int result = compare(data, (*tree)->data);
        if(result == 0) {
            // Do not insert duplicates
            return false;
        }
        else if(result < 0) {
            // Insert into the left sub-tree
            return insert(&(*tree)->left, data, compare);
        }
        else {
            // Insert into the right sub-tree
            return insert(&(*tree)->right, data, compare);
        }
    }
}

// Find a node
node_t** find(node_t** tree, int data, compare_t compare) {
    if(tree == NULL) {
        // Bad tree
        return NULL;
    }

    // Is the current node empty?
    if(*tree == NULL) {
        // Not found
        return NULL;
    }
    else {
        // Compare against the current node
        int result = compare(data, (*tree)->data);
        if(result == 0) {
            // Found the node, return the link to the node
            return tree;
        }
        else if(result < 0) {
            // Search into the left sub-tree
            return find(&(*tree)->left, data, compare);
        }
        else {
            // Search into the right sub-tree
            return find(&(*tree)->right, data, compare);
        }
    }
}

// Find the parent of a node
node_t** find_parent(node_t** parent, node_t** tree, int data, compare_t compare) {
    if(tree == NULL) {
        // Bad tree
        return NULL;
    }

    // Is the current node empty?
    if(*tree == NULL) {
        // Not found
        return NULL;
    }
    else {
        // Compare against the current node
        int result = compare(data, (*tree)->data);
        if(result == 0) {
            // Found the node, return the link to the parent node
            return parent;
        }
        else if(result < 0) {
            // Search into the left sub-tree
            return find_parent(tree, &(*tree)->left, data, compare);
        }
        else {
            // Search into the right sub-tree
            return find_parent(tree, &(*tree)->right, data, compare);
        }
    }
}

// Find the left-most node in a tree i.e. the 'first' or lowest node
node_t** find_left_most(node_t** tree) {
    if(tree == NULL) {
        // Bad tree
        return NULL;
    }

    // Is the tree empty?
    if(*tree == NULL) {
        return NULL;
    }
    else {
        for( ; (*tree)->left != NULL; tree = &(*tree)->left) {
            // Do nothing
        }
        return tree;
    }
}

// Destroy a tree
bool destroy(node_t** tree) {
    if(tree == NULL) {
        // Bad tree
        return false;
    }

    // Is the current node empty?
    if(*tree == NULL) {
        // Nothing to do
        return true;
    }
    else {
        // Recursively destroy the left branch
        destroy(&(*tree)->left);

        // Recursively destroy the right branch
        destroy(&(*tree)->right);

        // Free the node
        free(*tree);
    }

    return true;
}

// Print a tree (in-order: left, right, root)
void print(node_t** tree, int level) {
    if(tree == NULL) {
        // Bad tree
        return;
    }

    // Is the current node empty?
    if(*tree == NULL) {
        // Nothing to do
        return;
    }
    else {
        // Recursively print the right branch
        print(&(*tree)->right, level + 1);

        // Print the current node
        printf("%*s%2d\n", level * 4, "",  (*tree)->data);

        // Recursively print the left branch
        print(&(*tree)->left, level + 1);
    }
}

int main(void) {
    // Insert a new node into a bad tree
    if(false == insert(NULL, 0, compare_values)) {
        printf("Insert a new node into a bad tree: OK\n\n");
    }

    // Insert a new node into an empty tree
    node_t* tree = NULL;
    if(true == insert(&tree, 9, compare_values)) {
        printf("Insert a new node into an empty list: OK\n");
        print(&tree, 0);
    }
    else {
        printf("Insert a new node into an empty list: FAIL\n\n");
    }

    // Insert some nodes into an existing tree
    bool result = false;
    int values[] = { 4, 15, 6, 12, 17, 2 };
    for(size_t i = 0; i < NELEMENTS(values); i++) {
        result = insert(&tree, values[i], compare_values);
        if(result == false) {
            printf("Insert some nodes into an existing tree: FAIL\n\n");
            break;
        }
    }
    if(result == true) {
        printf("Insert some nodes into an existing tree: OK\n");
        print(&tree, 0);
    }

    // Find a node in a bad tree
    if(NULL == find(NULL, 15, compare_values)) {
        printf("Find for a node in a bad tree: OK\n\n");
    }

    // Find a node in an empty tree
    node_t* empty = NULL;
    if(NULL == find(&empty, 15, compare_values)) {
        printf("Find a node in an empty tree: OK\n\n");
    }
    else {
        printf("Find a node in an empty tree: FAIL\n\n");
    }

    // Find a node in an existing tree
    node_t** link = NULL;
    link = find(&tree, 15, compare_values);
    if(link != NULL) {
        printf("Find a node in an existing tree: OK\n");
        print(link, 0);
    }
    else {
        printf("Find a node in an existing tree: FAIL\n\n");
    }

    // Find the parent of a node in a bad tree
    if(NULL == find_parent(NULL, NULL, 12, compare_values)) {
        printf("Find the parent of a node in a bad tree: OK\n\n");
    }

    // Find the parent of a node in an empty tree
    if(NULL == find_parent(NULL, &empty, 12, compare_values)) {
        printf("Find the parent of a node in an empty tree: OK\n\n");
    }
    else {
        printf("Find the parent of a node in an empty tree: FAIL\n\n");
    }

    // Find the parent of a node in an existing tree
    link = find_parent(NULL, &tree, 12, compare_values);
    if(link != NULL) {
        printf("Find the parent of a node in an existing tree: OK\n");
        print(link, 0);
    }
    else {
        printf("Find the parent of a node in an existing tree: FAIL\n\n");
    }

    // Find the left-most node in a bad tree
    if(NULL == find_left_most(NULL)) {
        printf("Find the left-most node in a bad tree: OK\n\n");
    }

    // Find the left-most node in an empty tree
    if(NULL == find_left_most(&empty)) {
        printf("Find the left-most node in an empty tree: OK\n\n");
    }
    else {
        printf("Find the left-most node in an empty tree: FAIL\n\n");
    }

    // Find the left-most node in an existing tree
    link = find_left_most(&tree);
    if(link != NULL) {
        printf("Find the left-most node in an existing tree: OK\n");
        print(link, 0);
    }
    else {
        printf("Find the left-most node in an existing tree: FAIL\n\n");
    }

    // Destroy a bad tree
    if(false == destroy(NULL)) {
        printf("Destroy a bad tree: OK\n\n");
    }

    // Destroy an empty tree
    if(true == destroy(&empty)) {
        printf("Destroy a empty tree: OK\n\n");
    }
    else {
        printf("Destroy a empty tree: FAIL\n\n");
    }

    // Destroy an existing tree
    if(true == destroy(&tree)) {
        printf("Destroy an existing tree: OK\n\n");
    }
    else {
        printf("Destroy an existing tree: FAIL\n\n");
    }

    return EXIT_SUCCESS;
}
