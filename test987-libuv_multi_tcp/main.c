#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <uv.h>


struct context
{
    const char      *task_name;
    const char      *host;
    const char      *path;

    uv_loop_t       *loop;
    uv_getaddrinfo_t resolv_req;
    uv_connect_t     connect_req;
    uv_write_t       write_req;
    uv_tcp_t         tcp;
    uv_buf_t         buf;
    char             buf_mem[1024];

    struct addrinfo  hints;
    char            *content;
    size_t           content_capacity;
    size_t           content_size;
};

static void init(struct context *ctx, uv_loop_t *loop);
static void cleanup(struct context *ctx);
static void start(struct context *ctx);
static void on_resolved(uv_getaddrinfo_t *req, int status, struct addrinfo *addr);
static void on_connect(uv_connect_t *req, int status);
static void on_write(uv_write_t *req, int status);
static void on_alloc(uv_handle_t *handle, size_t size, uv_buf_t *buf);
static void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf);
static void finish_ok(struct context *ctx);
static void finish_error(struct context *ctx);


int main(void)
{
    uv_loop_t *loop = malloc(sizeof *loop);
    uv_loop_init(loop);

    // Queue concurrent HTTP request jobs
    struct context *ctx_com = malloc(sizeof *ctx_com);
    init(ctx_com, loop);
    ctx_com->task_name = "com";
    ctx_com->host = "www.example.com";
    ctx_com->path = "/";
    start(ctx_com);

    struct context *ctx_net = malloc(sizeof *ctx_net);
    init(ctx_net, loop);
    ctx_net->task_name = "net";
    ctx_net->host = "www.example.net";
    ctx_net->path = "/";
    start(ctx_net);

    struct context *ctx_org = malloc(sizeof *ctx_org);
    init(ctx_org, loop);
    ctx_org->task_name = "org";
    ctx_org->host = "www.example.org";
    ctx_org->path = "/";
    start(ctx_org);

    // Start
    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
    free(loop);

    cleanup(ctx_com);
    cleanup(ctx_net);
    cleanup(ctx_org);
    free(ctx_com);
    free(ctx_net);
    free(ctx_org);

    return 0;
}


static void init(struct context *ctx, uv_loop_t *loop)
{
    memset(ctx, 0, sizeof *ctx);
    ctx->loop = loop;
    ctx->resolv_req.data = ctx;
    ctx->connect_req.data = ctx;
    ctx->write_req.data = ctx;
    ctx->tcp.data = ctx;
    ctx->buf = uv_buf_init(ctx->buf_mem, sizeof ctx->buf_mem);
}


static void cleanup(struct context *ctx)
{
    // https://docs.libuv.org/en/v1.x/dns.html#c.uv_getaddrinfo_t.addrinfo
    uv_freeaddrinfo(ctx->resolv_req.addrinfo);
    free(ctx->content);
}


static void start(struct context *ctx)
{
    printf("Start %s\n", ctx->task_name);

    // Not sure if hints must be valid until the callback ends. AddressSanitizer
    // does not catch anything if I change this to local variable, so local
    // variable might be ok. It's not documented.
    // https://docs.libuv.org/en/v1.x/dns.html
    struct addrinfo *hints = &ctx->hints;
    memset(hints, 0, sizeof *hints);
    hints->ai_family = PF_INET;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_protocol = IPPROTO_TCP;
    hints->ai_flags = 0;

    uv_getaddrinfo(
        ctx->loop, &ctx->resolv_req, &on_resolved, ctx->host, "http", hints
    );
}


static void on_resolved(uv_getaddrinfo_t *req, int status, struct addrinfo *addr)
{
    uv_loop_t *loop = req->loop;
    struct context *ctx = req->data;

    printf("Resolve %s\n", ctx->task_name);

    if (status < 0) {
        fprintf(stderr, "resolve error: %s\n", uv_strerror(status));
        finish_error(ctx);
        return;
    }

    uv_tcp_init(loop, &ctx->tcp);
    uv_tcp_connect(&ctx->connect_req, &ctx->tcp, addr->ai_addr, &on_connect);
}


static void on_connect(uv_connect_t *req, int status)
{
    uv_loop_t *loop = req->handle->loop;
    struct context *ctx = req->data;
    uv_stream_t *stream = (uv_stream_t *) &ctx->tcp;

    printf("Connect %s\n", ctx->task_name);

    if (status < 0) {
        fprintf(stderr, "connect error: %s\n", uv_strerror(status));
        finish_error(ctx);
        return;
    }

    int message_size = snprintf(
        ctx->buf_mem,
        sizeof ctx->buf_mem,
        "GET %s HTTP/1.0\r\n"
        "Host: %s\r\n"
        "Accept: text/html\r\n"
        "\r\n",
        ctx->path,
        ctx->host
    );
    assert(message_size > 0);

    ctx->buf.base = ctx->buf_mem;
    ctx->buf.len = (size_t) message_size;
    uv_write(&ctx->write_req, stream, &ctx->buf, 1, &on_write);
}


static void on_write(uv_write_t *req, int status)
{
    uv_loop_t *loop = req->handle->loop;
    struct context *ctx = req->data;
    uv_stream_t *stream = (uv_stream_t *) &ctx->tcp;

    printf("Write %s\n", ctx->task_name);

    if (status < 0) {
        fprintf(stderr, "write error: %s\n", uv_strerror(status));

        // Be sure to immediately close the handle on error, not in cleanup(),
        // because delaying this leaves TCP connection with the server open.
        // Same applies for on_read.
        uv_close((uv_handle_t *) &ctx->tcp, NULL);

        finish_error(ctx);
        return;
    }

    uv_read_start(stream, on_alloc, on_read);
}


static void on_alloc(uv_handle_t *handle, size_t size, uv_buf_t *buf)
{
    struct context *ctx = handle->data;
    buf->base = ctx->buf_mem;
    buf->len = sizeof ctx->buf_mem;
}


static void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
{
    uv_loop_t *loop = stream->loop;
    struct context *ctx = stream->data;

    printf("Read %s\n", ctx->task_name);

    if (nread == UV_EOF) {
        uv_close((uv_handle_t *) &ctx->tcp, NULL);
        finish_ok(ctx);
        return;
    }
    if (nread < 0) {
        fprintf(stderr, "read error: %s\n", uv_strerror((int) nread));
        uv_read_stop(stream);
        uv_close((uv_handle_t *) &ctx->tcp, NULL);
        finish_error(ctx);
        return;
    }

    while (ctx->content_capacity - ctx->content_size < (size_t) nread) {
        size_t new_capacity = (ctx->content_capacity + 512) * 2;
        void *new_memory = realloc(ctx->content, new_capacity);
        assert(new_memory != NULL);
        ctx->content = new_memory;
        ctx->content_capacity = new_capacity;
    }

    memcpy(ctx->content + ctx->content_size, buf->base, (size_t) nread);
    ctx->content_size += (size_t) nread;
}


static void finish_ok(struct context *ctx)
{
    unsigned char checksum = 0;
    for (size_t i = 0; i < ctx->content_size; i++) {
        checksum ^= (unsigned char) ctx->content[i];
        checksum *= 97;
    }

    printf("Done %s (http://%s%s)\n", ctx->task_name, ctx->host, ctx->path);
    printf("  Size: %zu\n", ctx->content_size);
    printf("  Checksum: %d\n", (int) checksum);
    printf("\n");
}


static void finish_error(struct context *ctx)
{
    printf("Error %s (http://%s%s)\n", ctx->task_name, ctx->host, ctx->path);
    printf("\n");
}
