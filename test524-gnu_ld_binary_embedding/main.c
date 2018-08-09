#include <stddef.h>
#include <stdio.h>


int main(void)
{
    extern const char _binary_embed_dat_start[];
    extern const char _binary_embed_dat_end[];
    ptrdiff_t size = _binary_embed_dat_end - _binary_embed_dat_start;

    printf("Data: (%zd) '%.*s'\n", size, (int) size, _binary_embed_dat_start);
}
