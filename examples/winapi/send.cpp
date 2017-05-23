#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define SERVER "127.0.0.1"
#define PORT 49152

int main() {
    SOCKET sock;
    WSADATA wsa;
    struct sockaddr_in addr;
    if (WSAStartup(MAKEWORD(2,2), &wsa) == 0) {
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) != SOCKET_ERROR) {
		memset((char*)&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(PORT);
		addr.sin_addr.S_un.S_addr = inet_addr(SERVER);
		const char message[] = "Message";
		sendto(sock, message, strlen(message), 0, (struct sockaddr*)&addr, sizeof(addr));
		closesocket(sock);
	}
    }
    WSACleanup();
    return 0;
}
