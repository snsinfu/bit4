#include <stdio.h>
#include <stdlib.h>

#include <uv.h>


// uv objects must outlive async funcntions and must be valid until callback
// function finishes. Here I just use globals as the program is just async,
// not multi-threaded.
//
// Cleaner implemenntation would use a big context struct, I think? Alloc a
// context variable in main, fill data field of request objects with the
// address of the context variable (?), then fire the initial event...

static uv_fs_t open_req;
static uv_fs_t read_req;
static uv_fs_t close_req;

static char read_buffer_storage[1024];
static uv_buf_t read_buffer = {
    .base = read_buffer_storage,
    .len  = sizeof read_buffer_storage
};

static void on_open(uv_fs_t *req);
static void on_read(uv_fs_t *req);
static void on_close(uv_fs_t *req);


int main(int argc, char **argv)
{
    if (argc != 2) {
        fputs("usage: main <file>", stderr);
        return 1;
    }
    char const *filename = argv[1];

    uv_loop_t *loop = malloc(sizeof *loop);
    uv_loop_init(loop);

    uv_fs_open(loop, &open_req, filename, UV_FS_O_RDONLY, 0, &on_open);
    uv_run(loop, UV_RUN_DEFAULT);
    uv_fs_req_cleanup(&open_req);

    uv_loop_close(loop);
    free(loop);

    return 0;
}


static void on_open(uv_fs_t *req)
{
    // Documentation says the request MUST be cleaned up. So I use goto clean.
    // Same for other callbacks.
    // https://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_req_cleanup

    if (req->result < 0) {
        fprintf(stderr, "uv_fs_open: %s\n", uv_strerror((int) req->result));
        goto clean;
    }
    uv_file file = req->file;
    uv_loop_t *loop = req->loop;

    // FIXME: I'm still not sure but the offset 0 causes "invalid seek" error.
    uv_fs_read(loop, &read_req, file, &read_buffer, 1, 0, &on_read);

clean:
    uv_fs_req_cleanup(req);
}


static void on_read(uv_fs_t *req)
{
    if (req->result < 0) {
        fprintf(stderr, "uv_fs_read: %s\n", uv_strerror((int) req->result));
        goto clean;
    }
    uv_file file = req->file;
    uv_loop_t *loop = req->loop;

    if (req->result > 0) {
        for (unsigned i = 0; i < req->nbufs; i++) {
            uv_buf_t buf = req->bufs[i];
            printf("%u: %p %zu\n", i, buf.base, buf.len);
        }
    } else {
        uv_fs_close(loop, &close_req, file, &on_close);
    }

clean:
    uv_fs_req_cleanup(req);
}


static void on_close(uv_fs_t *req)
{
    if (req->result < 0) {
        fprintf(stderr, "uv_fs_close: %s\n", uv_strerror((int) req->result));
        goto clean;
    }

clean:
    uv_fs_req_cleanup(req);
}
