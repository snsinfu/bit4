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
    const char *host;
    const char *path;

    uv_async_t       start_req;
    uv_getaddrinfo_t resolv_req;
    uv_connect_t     connect_req;
    uv_write_t       write_req;
    uv_tcp_t         tcp;

    uv_buf_t         buf;
    char             buf_mem[1024];
};

static void context_init(struct context *ctx);
static void start(uv_async_t *req);
static void on_resolved(uv_getaddrinfo_t *req, int status, struct addrinfo *ai);
static void on_connect(uv_connect_t *req, int status);
static void on_write(uv_write_t *req, int status);
static void on_alloc(uv_handle_t *handle, size_t size, uv_buf_t *buf);
static void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf);


int main(void)
{
    uv_loop_t *loop = malloc(sizeof *loop);
    uv_loop_init(loop);

    struct context *fetch = malloc(sizeof *fetch);
    context_init(fetch);
    fetch->host = "www.example.com";
    fetch->path = "/";
    uv_async_init(loop, &fetch->start_req, &start);
    uv_async_send(&fetch->start_req);

    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
    free(loop);
    free(fetch);

    return 0;
}


static void context_init(struct context *ctx)
{
    ctx->start_req.data = ctx;
    ctx->resolv_req.data = ctx;
    ctx->connect_req.data = ctx;
    ctx->write_req.data = ctx;
    ctx->tcp.data = ctx;
    ctx->buf = uv_buf_init(ctx->buf_mem, sizeof ctx->buf_mem);
}


static void start(uv_async_t *req)
{
    uv_loop_t *loop = req->loop;
    struct context *ctx = req->data;

    struct addrinfo hints;
    hints.ai_family = PF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = 0;

    uv_getaddrinfo(loop, &ctx->resolv_req, &on_resolved, ctx->host, "80", &hints);
}


static void on_resolved(uv_getaddrinfo_t *req, int status, struct addrinfo *ai)
{
    uv_loop_t *loop = req->loop;
    struct context *ctx = req->data;

    if (status < 0) {
        fprintf(stderr, "resolve error: %s\n", uv_strerror(status));
        uv_close((uv_handle_t *) &ctx->start_req, NULL);
        return;
    }

    uv_tcp_init(loop, &ctx->tcp);
    uv_tcp_connect(&ctx->connect_req, &ctx->tcp, ai->ai_addr, &on_connect);
}


static void on_connect(uv_connect_t *req, int status)
{
    uv_loop_t *loop = req->handle->loop;
    struct context *ctx = req->data;
    uv_stream_t *stream = (uv_stream_t *) &ctx->tcp;

    if (status < 0) {
        fprintf(stderr, "connect error: %s\n", uv_strerror(status));
        uv_close((uv_handle_t *) &ctx->start_req, NULL);
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

    if (status < 0) {
        fprintf(stderr, "write error: %s\n", uv_strerror(status));
        uv_close((uv_handle_t *) &ctx->tcp, NULL);
        uv_close((uv_handle_t *) &ctx->start_req, NULL);
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

    if (nread == UV_EOF) {
        uv_close((uv_handle_t *) &ctx->tcp, NULL);
        uv_close((uv_handle_t *) &ctx->start_req, NULL);
        return;
    }
    if (nread < 0) {
        fprintf(stderr, "read error: %s\n", uv_strerror((int) nread));
        uv_read_stop(stream);
        uv_close((uv_handle_t *) &ctx->tcp, NULL);
        uv_close((uv_handle_t *) &ctx->start_req, NULL);
        return;
    }

    printf("=== Read (%d) ===\n%.*s\n", (int) nread, (int) nread, buf->base);
}
