#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct watch_entry {
    const char         *path;
    struct watch_entry *prev, *next;
};

struct watch_entry *list_from_array(struct watch_entry *entries, size_t len);
struct watch_entry *list_drop(struct watch_entry *node);
struct watch_entry *list_prepend(struct watch_entry *head, struct watch_entry *node);
struct watch_entry *list_append(struct watch_entry *tail, struct watch_entry *node);

static void do_work(struct watch_entry *list);


int main(int argc, char **argv)
{
    struct watch_entry *entries = NULL;
    size_t len = 0;

    for (int i = 1; i < argc; i++) {
        void *new_ptr = realloc(entries, (len + 1) * sizeof *entries);
        if (new_ptr == NULL) {
            fprintf(stderr, "error: Failed to allocate memory - %s\n", strerror(errno));
            return 1;
        }
        entries = new_ptr;
        len++;
        entries[len - 1] = (struct watch_entry) { .path = argv[i] };
    }

    do_work(list_from_array(entries, len));

    return 0;
}

// Function: do_work
//
// Parameters:
//   list - Pointer to the head of a list of watch_entry elements.
//
void do_work(struct watch_entry *list)
{
    struct watch_entry *queued = list;
    struct watch_entry *active = NULL;

    // Move some entries from 'queued' list to 'active' list.
    size_t n = 0;
    for (struct watch_entry *ent = queued; ent; ) {
        struct watch_entry *next = ent->next;
        n++;
        if (n % 2 == 0) { // FIXME: Changing rhs to 1 causes lost 'b' node and aliased 'a' nodes
            active = list_prepend(active, list_drop(ent));
        }
        ent = next;
    }

    printf("Queued:\n");
    for (struct watch_entry *ent = queued; ent; ent = ent->next) {
        printf("  '%s'\n", ent->path);
    }
    printf("Active:\n");
    for (struct watch_entry *ent = active; ent; ent = ent->next) {
        printf("  '%s'\n", ent->path);
    }
}

// Function: list_from_array
//
// Builds a list from a contiguous array of entries.
//
// Parameters:
//   arr - Pointer to the first element of the array.
//   len - Number of elements in the array.
//
// Returns:
//   Pointer to the head of the created list (i.e., the `arr` argument).
//
struct watch_entry *list_from_array(struct watch_entry *arr, size_t len)
{
    if (len == 0) {
        return NULL;
    }

    arr[0].prev = NULL;
    for (size_t i = 1; i < len; i++) {
        arr[i - 1].next = &arr[i];
        arr[i].prev = &arr[i - 1];
    }
    arr[len - 1].next = NULL;
    return arr;
}

// Function: list_drop
//
// Drops a node from a list. Adjacent nodes get connected in the list.
//
// Parameters:
//   node - Pointer to a node to drop from a list.
//
// Returns:
//   Pointer to the dropped node (i.e., the `node` argument).
//
struct watch_entry *list_drop(struct watch_entry *node)
{
    struct watch_entry *prev = node->prev;
    struct watch_entry *next = node->next;
    if (prev) {
        prev->next = next;
    }
    if (next) {
        next->prev = prev;
    }
    return node;
}

// Function: list_prepend
//
// Prepends a node to the head of a list.
//
// Parameters:
//   head - Pointer to the head of a list. Its prev member must be NULL.
//   node - Pointer to a node to prepend.
//
// Returns:
//   Pointer to the new head (i.e., the `node` argument).
//
struct watch_entry *list_prepend(struct watch_entry *head, struct watch_entry *node)
{
    node->next = head;
    if (head) {
        head->prev = node;
    }
    return node;
}

// Function: list_append
//
// Appends a node to the head of a list.
//
// Parameters:
//   last - Pointer to the last node of a list. Its next member must be NULL.
//   node - Pointer to a node to prepend.
//
// Returns:
//   Pointer to the new last node (i.e., the `node` argument).
//
struct watch_entry *list_append(struct watch_entry *last, struct watch_entry *node)
{
    node->prev = last;
    if (last) {
        last->next = node;
    }
    return node;
}
