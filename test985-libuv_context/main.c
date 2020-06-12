#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <uv.h>


struct context
{
    uv_fs_t  open_req;
    uv_fs_t  read_req;
    uv_fs_t  close_req;
    uv_buf_t read_buffer;
    char     read_buffer_mem[10];
    uv_file  current_file;
    int64_t  current_pos;
};

static void context_init(struct context *ctx);

static void on_open(uv_fs_t *req);
static void on_read(uv_fs_t *req);
static void on_close(uv_fs_t *req);


int main(int argc, char **argv)
{
    if (argc != 2) {
        fputs("usage: main <file>", stderr);
        return 1;
    }
    const char *filename = argv[1];

    struct context *ctx = malloc(sizeof *ctx);
    uv_loop_t *loop = malloc(sizeof *loop);
    uv_loop_init(loop);
    context_init(ctx);

    uv_fs_open(loop, &ctx->open_req, filename, UV_FS_O_RDONLY, 0, &on_open);
    uv_run(loop, UV_RUN_DEFAULT);
    uv_fs_req_cleanup(&ctx->open_req);

    uv_loop_close(loop);
    free(loop);
    free(ctx);

    return 0;
}


static void context_init(struct context *ctx)
{
    ctx->open_req.data = ctx;
    ctx->read_req.data = ctx;
    ctx->close_req.data = ctx;
    ctx->read_buffer = uv_buf_init(ctx->read_buffer_mem, sizeof ctx->read_buffer_mem);
}


static void on_open(uv_fs_t *req)
{
    if (req->result < 0) {
        fprintf(stderr, "uv_fs_open: %s\n", uv_strerror((int) req->result));
        goto clean;
    }
    uv_file file = (uv_file) req->result;
    uv_loop_t *loop = req->loop;
    struct context *ctx = req->data;

    ctx->current_file = file;
    ctx->current_pos = 0;
    uv_fs_read(loop, &ctx->read_req, file, &ctx->read_buffer, 1, 0, &on_read);

clean:
    uv_fs_req_cleanup(req);
}


static void on_read(uv_fs_t *req)
{
    if (req->result < 0) {
        fprintf(stderr, "uv_fs_read: %s\n", uv_strerror((int) req->result));
        goto clean;
    }
    uv_loop_t *loop = req->loop;
    struct context *ctx = req->data;
    uv_file file = ctx->current_file;

    if (req->result > 0) {
        int64_t n_read = (int64_t) req->result;
        char *data = ctx->read_buffer.base;

        printf("Read (%zu): %.*s\n", n_read, (int) n_read, data);

        ctx->current_pos += n_read;
        uv_fs_req_cleanup(req);
        uv_fs_read(loop, &ctx->read_req, file, &ctx->read_buffer, 1, ctx->current_pos, &on_read);
        return;
    } else {
        uv_fs_close(loop, &ctx->close_req, file, &on_close);
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
