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

void skdSetSocketSpecs(SKDSocket& skt, uint16_t family, const char* address, uint16_t port)
{
    skt.specs.sin_family = family;
    skt.specs.sin_addr.s_addr = inet_addr(address);
    skt.specs.sin_port = htons(port);
}

void skdBindSocket(SKDSocket& skt, uint16_t family, const char* address, uint16_t port)
{
    skdSetSocketSpecs(skt, family, address, port);

    if (bind(skt.socket, (struct sockaddr*)&skt.specs, sizeof(skt.specs)) == SOCKET_ERROR) {
        closesocket(skt.socket);
        WSACleanup();
        std::cerr << "Bind failed. Error Code: " << WSAGetLastError() << "\n";
    }
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
    if (connect(skt.socket, (struct sockaddr*)&skt.specs, sizeof(skt.specs)) == SOCKET_ERROR)
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