#include <stdio.h>


struct node {
    int value;
    struct node *prev;
    struct node *next;
};

struct node *list_insert_after(struct node *node, struct node *el);
struct node *list_drop(struct node *node);


int main(void)
{
    struct node elements[9] = {
        { .value = 1 },
        { .value = 2 },
        { .value = 3 },
        { .value = 4 },
        { .value = 5 },
        { .value = 6 },
        { .value = 7 },
        { .value = 8 },
        { .value = 9 }
    };
    enum { n_elements = sizeof elements / sizeof *elements };

    // Build a list
    struct node list = {
        .prev = &list,
        .next = &list
    };

    for (int i = 0; i < n_elements; i++) {
        list_insert_after(list.prev, &elements[i]);
    }

    // Forward iteration: 1 2 3 4 5 6 7 8 9
    for (struct node *el = list.next; el != &list; el = el->next) {
        printf("%d ", el->value);
    }
    puts("");

    // Move last element to the third position: 1 2 9 3 4 5 6 7 8
    list_insert_after(list.next->next, list_drop(list.prev));

    // Backward iteration: 8 7 6 5 4 3 9 2 1
    for (struct node *el = list.prev; el != &list; el = el->prev) {
        printf("%d ", el->value);
    }
    puts("");
}

// Function: list_insert_after
//
// Inserts an element after a node.
//
// Parameters:
//   node - Node in a valid list.
//   el   - The element to insert.
//
// Returns:
//   Pointer to the inserted element, i.e., the argument `el`.
//
struct node *list_insert_after(struct node *node, struct node *el)
{
    struct node *next = node->next;
    el->prev = node;
    el->next = next;
    node->next = el;
    next->prev = el;
    return el;
}

// Function: list_drop
//
// Drops a node from a list.
//
// Parameters:
//   node - Node in a valid list.
//
// Returns:
//   Pointer to the dropped element, i.e., the argument `node`.
//
struct node *list_drop(struct node *node)
{
    struct node *prev = node->prev;
    struct node *next = node->next;
    prev->next = next;
    next->prev = prev;
    return node;
}
