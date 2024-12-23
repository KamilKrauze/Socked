#if defined(_WIN32)

#include "sockedwin32.h"

#include <stdexcept>
#include <sstream>
#include <iostream>

void skdInitSocket()
{
    if (WSAStartup(MAKEWORD(2, 2), &skdWSA) != 0) {
        std::clog << "WSAStartup failed. Error Code: " << WSAGetLastError() << "\n";
    }
}

void skdCreateSocket(SKDSocket& skt, int af, int type, int protocol)
{
    skt.socket = socket(af, type, protocol);
    if (skt.socket == INVALID_SOCKET) {
        WSACleanup();
        std::clog << "Socket creation failed! Error Code: " << WSAGetLastError() << "\n";
    }
}

void skdBindSocketBin(SKDSocket& skt, uint16_t family, uint32_t address, uint32_t port)
{
    sockaddr_in sock_addr;
    sock_addr.sin_family = family;
    sock_addr.sin_addr.s_addr = address;
    sock_addr.sin_port = htons(port);

    if (bind(skt.socket, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) == SOCKET_ERROR) {
        closesocket(skt.socket);
        WSACleanup();
        std::clog << "Bind failed. Error Code: " << WSAGetLastError() << "\n";
    }

    skt.address = sock_addr;
    skt.port = port;
}

void skdBindSocketStr(SKDSocket& skt, uint16_t family, const char* address, uint32_t port)
{
    sockaddr_in sock_addr;
    sock_addr.sin_family = family;
    sock_addr.sin_addr.s_addr = inet_addr(address);
    sock_addr.sin_port = htons(port);

    if (bind(skt.socket, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) == SOCKET_ERROR) {
        closesocket(skt.socket);
        WSACleanup();
        std::clog << "Bind failed. Error Code: " << WSAGetLastError() << "\n";
    }

    skt.address = sock_addr;
    skt.port = port;
}

void skdCreateListener(SKDSocket& skt, uint64_t backlog)
{
    if (listen(skt.socket, backlog) == SOCKET_ERROR) {
        closesocket(skt.socket);
        WSACleanup();
        
        std::cout << "Failed to create listener. Error Code: " << WSAGetLastError() << std::endl;
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