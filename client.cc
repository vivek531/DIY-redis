#include <netinet/in.h>
#include <iostream>
#include <stdarg.h>
#include <unistd.h>
#include <cstring>

// fd = socket()
// connect(fd, address)
// do_something_with(fd)
// close(fd)

int main() {
int fd = socket(AF_INET, SOCK_STREAM, 0);
if (fd < 0) {
    std::cout << ("socket()") << std::endl;
    return 1;
}

struct sockaddr_in addr = {};
addr.sin_family = AF_INET;
addr.sin_port = ntohs(1234);
addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);  // 127.0.0.1
int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
if (rv) {
    std::cout << ("connect") << std::endl;
}

char msg[] = "hello";
int sz = write(fd, msg, strlen(msg));
std::cout << "client wrote num of bytes: " << sz << std::endl;

char rbuf[64] = {};
ssize_t n = read(fd, rbuf, sizeof(rbuf) - 1);
if (n < 0) {
    std::cout << ("read") << std::endl;
}
printf("server says: %s\n", rbuf);
close(fd);
}