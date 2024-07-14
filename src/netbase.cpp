#include <apue.h>

int tcp_socket_bind_listen(const char *ip_bind, const in_port_t port)
{

    int op = 1, ret, lfd;
    SOCKADDRIN server_sockaddr = {0};
    // 创建socket
    lfd = socket(AF_INET, SOCK_STREAM, 0);
    HANDLE_ERROR_N1_EXIT(lfd, "socket");
    
    ret = setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(op));//端口复用
    HANDLE_ERROR_N1_EXIT(ret, "setsockopt");
    // bind
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(port);
    if(NULL == ip_bind)
        server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    else
        inet_pton(AF_INET, ip_bind, &server_sockaddr.sin_addr.s_addr);

    ret = bind(lfd, (PSOCKADDR)&server_sockaddr, sizeof(SOCKADDRIN));
    HANDLE_ERROR_N1_EXIT(ret, "bind");
    // listen
    ret = listen(lfd, MAX_CONNECTIONS);
    HANDLE_ERROR_N1_EXIT(ret, "listen");
    return lfd;
}

char *ip_from_sockaddr(PSOCKADDRIN paddr)
{

    static char ipv4[100];
    inet_ntop(AF_INET, &paddr->sin_addr.s_addr, ipv4, 100);
    return ipv4;
}