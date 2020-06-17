#include <stdio.h>
#include <string.h>

#include <hs/hs.h>


struct context
{
    const char *text;
    int matches;
};

static int on_match(
    unsigned id,
    unsigned long long start,
    unsigned long long end,
    unsigned flags,
    void *data
);


int main()
{
    int exit_code = 1;

    const char *regex = "a(b|.a)+";
    const char *text =
        // CC-BY-SA: https://en.wikipedia.org/wiki/Abracadabra
        "Abracadabra is of unknown origin, and its first occurrence is in the "
        "second century works of Serenus Sammonicus, according to the Oxford "
        "English Dictionary.[1] Several folk etymologies are associated with "
        "the word:[2] from phrases in Hebrew that mean \"I will create as I "
        "speak\",[3] or Aramaic \"I create like the word\" (אברא כדברא),[4] "
        "to folk etymologies that point to similar words in Latin and Greek "
        "such as abraxas.[5] According to the OED Online, \"no documentation "
        "has been found to support any of the various conjectures.\"[5]";

    // Declare resources to be freed on exit. (cf. cleanup label)
    hs_database_t *db = NULL;
    hs_stream_t *stream = NULL;
    hs_scratch_t *scratch = NULL;
    hs_compile_error_t *err = NULL;
    hs_error_t status;

    const unsigned flags = HS_FLAG_CASELESS | HS_FLAG_UTF8 | HS_FLAG_SOM_LEFTMOST;
    const unsigned mode = HS_MODE_STREAM | HS_MODE_SOM_HORIZON_LARGE;
    status = hs_compile(regex, flags, mode, NULL, &db, &err);
    if (status != HS_SUCCESS) {
        fprintf(stderr, "error: %s\n", err->message);
        goto cleanup;
    }

    status = hs_open_stream(db, 0, &stream);
    if (status != HS_SUCCESS) {
        fprintf(stderr, "error: %s\n", err->message);
        goto cleanup;
    }

    status = hs_alloc_scratch(db, &scratch);
    if (status != HS_SUCCESS) {
        fprintf(stderr, "error: %d\n", status);
        goto cleanup;
    }

    struct context ctx = {
        .text = text,
        .matches = 0
    };
    const size_t text_size = strlen(text);
    const size_t max_chunk = 10;
    for (size_t start = 0; start < text_size; start += max_chunk) {
        size_t end = start + max_chunk;
        if (end > text_size) {
            end = text_size;
        }
        const char *chunk = text + start;
        unsigned chunk_size = (unsigned) (end - start);

        status = hs_scan_stream(stream, chunk, chunk_size, 0, scratch, &on_match, &ctx);
        if (status == HS_SCAN_TERMINATED) {
            fprintf(stderr, "Terminated\n");
            break;
        }
        if (status != HS_SUCCESS) {
            fprintf(stderr, "error: %d\n", status);
            goto cleanup;
        }
    }

    printf("\n");
    printf("%d matches found\n", ctx.matches);

    // Mark success.
    exit_code = 0;

cleanup:
    hs_free_database(db);
    hs_close_stream(stream, NULL, NULL, NULL);
    hs_free_scratch(scratch);
    hs_free_compile_error(err);

    return exit_code;
}


static int on_match(
    unsigned id,
    unsigned long long start,
    unsigned long long end,
    unsigned flags,
    void *data
)
{
    (void) flags;
    struct context *ctx = data;

    printf("Match\n");
    printf("  expression %u\n", id);
    printf("  range      [%llu, %llu)\n", start, end);
    printf("  text       %.*s\n", (int) (end - start), ctx->text + start);

    ctx->matches++;

    return 0;
}
