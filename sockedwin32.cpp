#if defined(_WIN32)

#include "sockedwin32.h"

#include <cstdio>
#include <stdexcept>
#include <iostream>
#include <string>

void skdInitSocket()
{
    WSADATA skdWSA;
    if (WSAStartup(MAKEWORD(2, 2), &skdWSA) != 0) {
        std::cerr << "WSAStartup failed. WSA Error Code: " << WSAGetLastError() << "\n";
    }
}

void skdCreateSocket(SkdSocket& skt, int af, int type, int protocol)
{
    skt.socket = socket(af, type, protocol);
    if (skt.socket == INVALID_SOCKET) {
        closesocket(skt.socket);
        WSACleanup();
        std::cerr << "Socket creation failed! WSA Error Code: " << WSAGetLastError() << "\n";
    }
}

void skdCloseSocket(SkdSocket& skt)
{
    closesocket(skt.socket);
}

void skdCleanupSocket()
{
    WSACleanup();
}

void skdSetSocketOpt(SkdSocket& skt, int level, int optname, int optval)
{
    if (setsockopt(skt.socket, level, optname, (char*)&optval, sizeof(optval)) == SOCKET_ERROR) {
        printf("Failed to set socket options, WSA Error Code: %d", WSAGetLastError());
    }
}

void skdSetSocketSpecs(SkdSocket& skt, uint16_t family, const char* address, uint16_t port)
{
    skt.specs.family = family;
    skt.specs.port = htons(port);
    if (inet_pton(skt.specs.family, address, &skt.specs.address.data) <= 0)
    {
        std::cerr << "Invalid address!" << std::endl;
        closesocket(skt.socket);
        return;
    }
}

char* skdAddressFamilyToString(uint16_t family)
{
    char* family_str = ((family == AF_INET) ? "IPv4" : (family == AF_INET6) ? "IPv6" : "Unknown");
    return family_str;
}

uint16_t skdGetPortAsHost(uint16_t n_port)
{
    return ntohs(n_port);
}

char* skdGetIPv4AddressAsHost(SkdSocket& skt)
{
    char* ip = new char[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &skt.specs.address.data, ip, INET_ADDRSTRLEN) <= 0)
    {
        std::cerr << "Invalid address! WSA Error Code: " << WSAGetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }

    return ip;
}

void skdAccept(SkdSocket& server_skt, SkdSocket& client_skt)
{
    int client_addr_size = sizeof(client_skt.specs);
    client_skt.socket = accept(server_skt.socket, (struct sockaddr*)&client_skt.specs, &client_addr_size);
}

void skdBindSocket(SkdSocket& skt, uint16_t family, const char* address, uint16_t port)
{
    skdSetSocketSpecs(skt, family, address, port);

    if (bind(skt.socket, (struct sockaddr*)&skt.specs, sizeof(skt.specs)) == SOCKET_ERROR) {
        closesocket(skt.socket);
        WSACleanup();
        std::cerr << "Bind failed. WSA Error Code: " << WSAGetLastError() << "\n";
    }
}

void skdCreateListener(SkdSocket& skt, uint64_t backlog)
{
    if (listen(skt.socket, backlog) == SOCKET_ERROR) {
        closesocket(skt.socket);
        WSACleanup();

        std::cerr << "Failed to create listener. WSA Error Code: " << WSAGetLastError() << std::endl;
    }
}

void skdConnectSocket(SkdSocket& skt)
{
    if (connect(skt.socket, (struct sockaddr*)&skt.specs, sizeof(skt.specs)) == SOCKET_ERROR)
    {
        std::cerr << "Connection failed. WSA Error Code: " << WSAGetLastError() << "\n";

        closesocket(skt.socket);
        WSACleanup();
    }
}


void skdSend(SkdSocket& skt, const char* msg, size_t size, int flags)
{
    send(skt.socket, msg, size, flags);
}

void skdSendTo(SkdSocket& skt, const char* msg, size_t size, int flags)
{
    sendto(skt.socket, msg, size, flags, (struct sockaddr*)skt.specs.address.data, sizeof(skt.specs.address.data));
}

int64_t skdReceive(SkdSocket& skt, char* buffer, size_t size, int flags)
{
    uint64_t bytes = 0;
    bytes = recv(skt.socket, buffer, size, flags);
    return bytes;
}

int64_t skdReceiveFrom(SkdSocket& skt, char* buffer, size_t size, int flags)
{
    int addr_size = sizeof(skt.specs.address.data);
    return recvfrom(skt.socket, buffer, size, flags, (struct sockaddr*)skt.specs.address.data, &addr_size);
}

void skdPrintSocketAsNetwork(SkdSocket& skt)
{
    auto family = skdAddressFamilyToString(skt.specs.family);
    printf("Socket Data:\n\t- Socket->[%u]\n\t - Specs:\n\t\t- Family->[%s]\n\t\t- Port->[%u]\n\t\t- Address->[%u]", 
        skt.socket, family, skt.specs.port, skt.specs.address.data
    );
    delete family;
    return;
}

void skdPrintSocketAsHost(SkdSocket& skt)
{
    auto family = skdAddressFamilyToString(skt.specs.family);
    uint16_t port = skdGetPortAsHost(skt.specs.port);
    char* address = skdGetIPv4AddressAsHost(skt);
    printf("Socket Data:\n\t- Socket->[%u]\n\t - Specs:\n\t\t- Family->[%s]\n\t\t- Port->[%u]\n\t\t- Address->[%s]", 
        skt.socket, family, port, address);
    delete address;
    delete family;
    return;
}

void skdPrintSocket(SkdSocket& skt)
{
    auto family = skdAddressFamilyToString(skt.specs.family);
    uint16_t port = skdGetPortAsHost(skt.specs.port);
    char* address = skdGetIPv4AddressAsHost(skt);

    printf("Socket Data:\n\t- Socket->[%u]\n\t - Specs:\n\t\t- Family->[%s]\n\t\t- Port->[%u]->[%s]\n\t\t- Address->[%u]->[%s]", 
        skt.socket, family, skt.specs.port, port, skt.specs.address.data, address
    );

    delete address;
    delete family;
    return;
}

#endif