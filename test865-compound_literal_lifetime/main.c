#include <stdio.h>


struct node {
    int          value;
    struct node *prev;
    struct node *next;
};

struct node *list_init(struct node *sentinel)
{
    sentinel->prev = sentinel;
    sentinel->next = sentinel;
    return sentinel;
}

int main(void)
{
    // Compound literal is an lvalue and its lifetime extends to the entire
    // enclosing block.
    // https://en.cppreference.com/w/c/language/compound_literal
    struct node *node = list_init(&(struct node) { .value = -1 });
    printf("%p\n", (void *) node->prev);
    return 0;
}
