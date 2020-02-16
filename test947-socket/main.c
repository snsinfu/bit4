#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char **argv)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    (void) argc;
    char const *host = argv[1];
    char const *port = "80";
    struct addrinfo *addr_list = NULL;
    getaddrinfo(host, port, NULL, &addr_list);
    connect(sock, addr_list->ai_addr, addr_list->ai_addrlen);

    char buf[1024];
    char const *req =
        "GET / HTTP/1.0\r\n"
        "Host: %s\r\n"
        "Accept: text/html\r\n"
        "\r\n";
    snprintf(buf, sizeof buf, req, host);
    write(sock, buf, strlen(buf));

    ssize_t n = read(sock, buf, sizeof buf);
    fwrite(buf, 1, (size_t) n, stdout);

    return 0;
}
