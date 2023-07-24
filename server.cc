#include <netinet/in.h>
#include <iostream>
#include <stdarg.h>
#include <unistd.h>
#include <cstring>

// fd = socket()
// bind(fd, address)
// listen(fd)
// while True:
//     conn_fd = accept(fd)
//     do_something_with(conn_fd)
//     close(conn_fd)

static void do_something(int connfd) {
    char rbuf[64] = {};
    ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        std::cout << ("read() error") << std::endl;
        return;
    }
    printf("client says: %s\n", rbuf);

    char wbuf[] = "world";
    int sz = write(connfd, wbuf, strlen(wbuf));
    std::cout << "server returns num of bytes: " << sz << std::endl;
}

int main() {
int fd = socket(AF_INET, SOCK_STREAM, 0);


int val = 1;
setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

struct sockaddr_in addr = {};
addr.sin_family = AF_INET;
addr.sin_port = ntohs(1234);
addr.sin_addr.s_addr = ntohl(0);    // wildcard address 0.0.0.0

int rv = bind(fd, (const sockaddr *)&addr, sizeof(addr));
if (rv) {
    std::cout << ("bind()") << std::endl;
    return 1;
}

// listen
rv = listen(fd, SOMAXCONN);
if (rv) {
    std::cout << ("listen()") << std::endl;
    return 1;
}

while (true) {
    // accept
    struct sockaddr_in client_addr = {};
    socklen_t socklen = sizeof(client_addr);
    int connfd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
    if (connfd < 0) {
        continue;   // error
    }

    do_something(connfd);
    close(connfd);
}
}