#if defined(_WIN32)

#include "sockedwin32.h"

#include <stdexcept>
#include <sstream>
#include <iostream>

void skdInitSocket()
{
    WSADATA skdWSA;
    if (WSAStartup(MAKEWORD(2, 2), &skdWSA) != 0) {
        std::cerr << "WSAStartup failed. Error Code: " << WSAGetLastError() << "\n";
    }
}

void skdCreateSocket(SKDSocket& skt, int af, int type, int protocol)
{
    skt.socket = socket(af, type, protocol);
    if (skt.socket == INVALID_SOCKET) {
        closesocket(skt.socket);
        WSACleanup();
        std::cerr << "Socket creation failed! Error Code: " << WSAGetLastError() << "\n";
    }
}

void skdSetSocketOpt(SKDSocket& skt, int level, int optname, int optval)
{
    if (setsockopt(skt.socket, level, optname, (char*)&optval, sizeof(optval)) == SOCKET_ERROR) {
        std::cerr << "Failed to set socket options. Error Code: " << WSAGetLastError() << "\n";
    }
}

void skdBindSocket(SKDSocket& skt, uint16_t family, const char* address, uint16_t port)
{
    sockaddr_in sock_addr;
    sock_addr.sin_family = family;
    sock_addr.sin_addr.s_addr = inet_addr(address);
    sock_addr.sin_port = htons(port);

    if (bind(skt.socket, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) == SOCKET_ERROR) {
        closesocket(skt.socket);
        WSACleanup();
        std::cerr << "Bind failed. Error Code: " << WSAGetLastError() << "\n";
    }

    skt.address = sock_addr;
}

void skdCreateListener(SKDSocket& skt, uint64_t backlog)
{
    if (listen(skt.socket, backlog) == SOCKET_ERROR) {
        closesocket(skt.socket);
        WSACleanup();
        
        std::cerr << "Failed to create listener. Error Code: " << WSAGetLastError() << std::endl;
    }
}

void skdConnectSocket(SKDSocket& skt)
{
    if (connect(skt.socket, (struct sockaddr*)&skt.address, sizeof(skt.address)) == SOCKET_ERROR)
    {
        int error = WSAGetLastError();
        std::cerr << "Connection failed. Error Code: " << error << "\n";

        closesocket(skt.socket);
        WSACleanup();
    }
}

void skdCloseSocket(SKDSocket& skt)
{
    closesocket(skt.socket);
}

void skdDestroySocket(SKDSocket& skt)
{
    closesocket(skt.socket);
    WSACleanup();
}

#endif