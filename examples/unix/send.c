#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER "127.0.0.1"
#define PORT 49152

int main(void) {
    struct sockaddr_in addr;
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    memset((char*)&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_aton(SERVER, &addr.sin_addr);
    const char message[] = "Message";
    sendto(sock, message, strlen(message), 0, (struct sockaddr*)&addr, sizeof(addr));
    shutdown(sock, 0);
    return 0;
}