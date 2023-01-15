#include <cstdio>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define DEBUG_BUFFER_SIZE (4096 - sizeof(DWORD))

#define LOG(format, ...) DbgPrint(format, ##__VA_ARGS__)

struct Socket
{
	WSADATA wsa;
	SOCKET sock;
	sockaddr_in addr;
};

Socket g_Socket;

void InitSocket(const char *ip, u_short port)
{
	memset(&g_Socket, 0, sizeof g_Socket);
	if(WSAStartup(MAKEWORD(2, 2), &g_Socket.wsa) == 0)
	{
		if((int)(g_Socket.sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) != SOCKET_ERROR)
		{
			memset(&g_Socket.addr, 0, sizeof g_Socket.addr);
			g_Socket.addr.sin_family = AF_INET;
			g_Socket.addr.sin_port = htons(port);
			g_Socket.addr.sin_addr.S_un.S_addr = inet_addr(ip);
		}
	}
}

void Send(const char *message)
{
	if(g_Socket.sock)
	{
		sendto(g_Socket.sock, message, (int)strlen(message), 0, (sockaddr *)&g_Socket.addr, sizeof g_Socket.addr);
	}
}

void CloseSocket()
{
	if(g_Socket.sock)
	{
		closesocket(g_Socket.sock);
	}
	WSACleanup();
	memset(&g_Socket, 0, sizeof g_Socket);
}

void DbgPrint(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	char buffer[DEBUG_BUFFER_SIZE];
	vsnprintf(buffer, sizeof buffer, format, args);
	Send(buffer);
	va_end(args);
}

int main()
{
	InitSocket("127.0.0.1", 49152);
	LOG("Message");
	CloseSocket();
	return 0;
}