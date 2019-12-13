#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    size_t const max_size = 0x200000000;   // 8GiB

    for (size_t size = 1; size <= max_size; size *= 2) {
        printf("malloc %zu\n", size);

        unsigned char *mem = malloc(size);
        if (mem == NULL) {
            perror("malloc");
            break;
        }

        for (size_t i = 0; i < size; i++) {
            mem[i] = (unsigned char) i;
        }

        free(mem);
    }

    return 0;
}
