// Binary tree
//
// This typically executes in log_2(N) time for insertion, deletion and sorting
// See https://en.wikipedia.org/wiki/Binary_tree
//
// This supports the following operations:
//  create          Create a new stand-alone node
//	destroy			Destroy a tree (recursively)
//	insert			Insert a node into a tree (recursively)
//	search			Search for an item in the tree (recursively)
//	search_parent	Search for the parent of an item in the tree (recursively)
//	detach			Detach a node or sub-tree
//	print_preorder	Print the contents of a tree (pre-order: root, left, right)
// 	print_inorder	Print the contents of a tree (in-order: left, root, right)
//	print_postorder	Print the contents of a tree (post-order: left, right, root)
//	print_tree		Print a tree (in-order: left, right, root)

#include <errno.h>      // For errno
#include <stdbool.h>    // For bool, true, false
#include <stdio.h>      // For printf
#include <stdlib.h>     // For EXIT_SUCCESS, malloc, free
#include <string.h>     // For strerror

#define NELEMENTS(a)    (sizeof(a) / sizeof(a[0]))

typedef struct node_t node_t;
struct node_t {
    int data;
    node_t *left;
    node_t *right;
};

typedef int (*compare_t)(node_t *tree, int data);

// Comparison function used for insertion, search et al
int compare_data(node_t *tree, int data) {
	// Or more simply: return data - tree->data;
	if(data < tree->data) {
		return -1;
	}
	else if(data > tree->data) {
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
        printf("ERROR! %s", strerror(errno));
        return NULL;
    }
    node->data  = data;
    node->left  = NULL;
    node->right = NULL;
    return node;
}

// Destroy a tree (recursively)
void destroy(node_t **tree) {
	// Is the tree empty?
    if(*tree == NULL) {
    	// Nothing to do
    	return;
    }
    // The tree is not empty
    else {
    	// Destroy the left sub-tree
    	destroy(&(*tree)->left);

    	// Destroy the right sub-tree
    	destroy(&(*tree)->right);

    	// Free the node
    	free(*tree);
    	*tree = NULL;
    }
}

// Insert a node into a tree (recursively)
void insert(node_t **tree, int data, compare_t compare) {
	// Is the tree empty?
    if(*tree == NULL) {
    	// Create and insert the new node here
    	*tree = create(data);
    }
    // The tree is not empty
    else {
    	// Compare against the current node
    	int result = compare(*tree, data);
    	if(result < 0) {
    		// Recurse into the left sub-tree
    		insert(&(*tree)->left, data, compare);
    	}
    	else if(result > 0) {
    		// Recurse into the right sub-tree
    		insert(&(*tree)->right, data, compare);
    	}
    	else {
    		// Already in the tree, do not insert
    		printf("Already in tree: data = %d, tree = %p\n", data, *tree);
    	}
    }
}

// Search for an item in the tree (recursively)
node_t *search(node_t **tree, int data, compare_t compare) {
	// Is the tree empty?
    if(*tree == NULL) {
    	// The data is not in the tree
    	return NULL;
    }
    // The tree is not empty
    else {
    	// Compare against the current node
    	int result = compare(*tree, data);
    	if(result < 0) {
    		// Recurse into the left sub-tree
    		return search(&(*tree)->left, data, compare);
    	}
    	else if(result > 0) {
    		// Recurse into the right sub-tree
    		return search(&(*tree)->right, data, compare);
    	}
    	else {
    		// Found the item, return the node
    		return *tree;
    	}
    }
}

// Search for the parent of an item in the tree (recursively)
node_t *search_parent(node_t **tree, node_t *parent, int data, compare_t compare) {
	// Is the tree empty?
    if(*tree == NULL) {
    	// The node is not in the tree, hence no parent
    	return NULL;
    }
    // The tree is not empty
    else {
    	// Compare against the current node
    	int result = compare(*tree, data);
    	if(result < 0) {
    		// Recurse into the left sub-tree
    		return search_parent(&(*tree)->left, *tree, data, compare);
    	}
    	else if(result > 0) {
    		// Recurse into the right sub-tree
    		return search_parent(&(*tree)->right, *tree, data, compare);
    	}
    	else {
    		// Found the item, return the parent
    		return parent;
    	}
    }
}

// Detach a node or sub-tree
node_t *detach(node_t *parent, node_t *child) {
	node_t *detached = NULL;

	if((parent != NULL) && (child != NULL)) {
		if(child == parent->left) {
			detached = child;
			parent->left = NULL;
		}
		else if(child == parent->right) {
			detached = child;
			parent->right = NULL;
		}
		else {
    		printf("Not a child of the parent: data = %d, child = %p\n", child->data, child);
		}
	}

	return detached;
}

// Delete an item in the tree
void delete(node_t **tree, int data, compare_t compare) {
	// Find the node and its parent
	node_t *node   = search(tree, data, compare);
	node_t *parent = search_parent(tree, NULL, data, compare);

	if((node != NULL) && (parent != NULL)) {
		// Get the parent's link to the node
		node_t **link = (node == parent->left) ? &parent->left : &parent->right;

		// If the node has no right sub-tree?
		if(node->right == NULL) {
			// If the node has no left sub-tree?
			if(node->left == NULL) {
				// Node has no left or right sub-tree, so can be simply deleted
				printf("\nNo L, no R: delete item %d at %p\n", node->data, node);
				*link = NULL;
				free(node);
			}
			else {
				// Node has left sub-tree only, so replace it with the left sub-tree
				printf("\nHas L, no R: delete item %d at %p\n", node->data, node);
				*link = node->left;
				free(node);
			}
		}
		// If the node has no left sub-tree?
		else if(node->left == NULL) {
			// Node has right sub-tree only, so replace it with the right sub-tree
			printf("\nNo L, Has R: delete item %d at %p\n", node->data, node);
			*link = node->right;
			free(node);

		}
		else {
			// Node has both left and right sub-trees
			printf("\nHas L, Has R: delete item %d at %p\n", node->data, node);

			// TODO
			// Successor is the left-most node in the right sub-tree
			// Successor acquires the left and right sub-tree
		}
	}
}

// Print the contents of a tree (pre-order: root, left, right)
void print_preorder(node_t *tree) {
	if(tree != NULL) {
		printf("%2d\n", tree->data);
		print_preorder(tree->left);
		print_preorder(tree->right);
	}
}

// Print the contents of a tree (in-order: left, root, right)
void print_inorder(node_t *tree) {
	if(tree != NULL) {
		print_inorder(tree->left);
		printf("%2d\n", tree->data);
		print_inorder(tree->right);
	}
}

// Print the contents of a tree (post-order: left, right, root)
void print_postorder(node_t *tree) {
	if(tree != NULL) {
		print_postorder(tree->left);
		print_postorder(tree->right);
		printf("%2d\n", tree->data);
	}
}

// Print a tree (in-order: left, right, root)
void print_tree(node_t *tree, int level) {
	if(tree != NULL) {
		print_tree(tree->right, level+1);
		printf("%*s%2d\n", level*4, "", tree->data);
		print_tree(tree->left, level+1);
	}
}

int main(void) {
	// Create a new tree
	node_t *tree = NULL;

	// Insert some data
	int values[] = { 9, 4, 15, 6, 12, 17, 2 };
	for(size_t i = 0; i < NELEMENTS(values); i++) {
		insert(&tree, values[i], compare_data);
	}
	printf("\nTree:\n");
	print_tree(tree, 0);

	// Verify that duplicates are not inserted
	insert(&tree, 9, compare_data);

	// Search for an item
	node_t *node = search(&tree, 12, compare_data);
	if(node != NULL) {
		printf("\nFound item %d at %p\n", node->data, node);
	}

	// Search for the parent of an item
	node_t *parent = search_parent(&tree, NULL, 12, compare_data);
	if(parent != NULL) {
		printf("\nFound parent %d of item 12 at %p\n", parent->data, parent);
	}

	// Detach the item
	node_t *detached = detach(parent, node);
	printf("\nDetached:\n");
	print_tree(detached, 0);
	printf("\nTree:\n");
	print_tree(tree, 0);

	// Destroy the tree
	destroy(&tree);

	// Create a new tree
	int values2[] = { 2, 5, 6, 4, 3, 1 };
	for(size_t i = 0; i < NELEMENTS(values2); i++) {
		insert(&tree, values2[i], compare_data);
	}
	printf("\nTree:\n");
	print_tree(tree, 0);

	// Delete a node that has no children
	delete(&tree, 1, compare_data);
	printf("\nTree:\n");
	print_tree(tree, 0);

	// Delete a node that has one child
	delete(&tree, 4, compare_data);
	printf("\nTree:\n");
	print_tree(tree, 0);

	// Reset the tree
	destroy(&tree);
	for(size_t i = 0; i < NELEMENTS(values2); i++) {
		insert(&tree, values2[i], compare_data);
	}
	printf("\nTree:\n");
	print_tree(tree, 0);

	// Delete a node that has two children
	delete(&tree, 2, compare_data);
	printf("\nTree:\n");
	print_tree(tree, 0);

    return EXIT_SUCCESS;
}
