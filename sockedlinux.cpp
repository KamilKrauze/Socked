#if defined(__linux__)
#include "sockedlinux.h"

#include <iostream>
#include <string.h>

void skdInitSocket() {return;}

void skdCreateSocket(SkdSocket& skt, int af, int type, int protocol) 
{
    if ((skt.socket = socket(af, type, protocol)) == 0)
    {
        perror("Socked failed");
        exit(EXIT_FAILURE);
    }
}

void skdCloseSocket(SkdSocket& skt)
{
    close(skt.socket);
}

void skdCleanupSocket(SkdSocket& skt)
{
    return;
}


void skdSetSocketOpt(SkdSocket& skt, int level, int optname, int optval)
{
    if (skt.socket, level, optname, &optval, sizeof(optval) < 0)
    {
        perror("Could not set socket option!");
        close(skt.socket);
        exit(EXIT_FAILURE);
    }
}

void skdSetSocketSpecs(SkdSocket& skt, uint16_t family, const char* address, uint16_t port)
{
    skt.specs.family = family;
    skt.specs.port = port;
    if (inet_pton(family, address, &skt.specs.address.data) <= 0)
    {
        perror("Invalid address!");
        close(skt.socket);
        exit(EXIT_FAILURE);
    }
}

char* skdAddressFamilyToString(uint16_t family)
{
    // char* family_str = ((family == AF_INET) ? "IPv4" : (family == AF_INET6) ? "IPv6" : "Unknown");
    // return family_str;
    return nullptr
}

uint16_t skdGetPortAsHost(uint16_t n_port)
{
    return ntohs(n_port);
}

char* skdGetIPv4AddressAsHost(SkdSocket& skt)
{
    // char* ip = new char[INET_ADDRSTRLEN];
    // if (inet_ntop(AF_INET, &skt.specs.address.data, ip, INET_ADDRSTRLEN) <= 0)
    // {
    //     std::cerr << "Invalid address! WSA Error Code: " << WSAGetLastError() << std::endl;
    //     exit(EXIT_FAILURE);
    // }

    return nullptr;
}

void skdAccept(SkdSocket& server_skt, SkdSocket& client_skt)
{
    // int client_addr_size = sizeof(client_skt.specs);
    // client_skt.socket = accept(server_skt.socket, (struct sockaddr*)&client_skt.specs, &client_addr_size);
}

void skdBindSocket(SkdSocket& skt, uint16_t family, const char* address, uint16_t port)
{
    skdSetSocketSpecs(skt, family, address, port);

    if (bind(skt.socket, (struct sockaddr*)&skt.specs, sizeof(skt.specs)) < 0)
    {
        perror("Connection failed!");
        close(skt.socket);
        exit(EXIT_FAILURE);
    }
}

void skdConnectSocket(SkdSocket& skt)
{
    if (connect(skt.socket, (struct sockaddr*)&skt.specs, sizeof(skt.specs)) < 0)
    {
        perror("Connection failed!");
        close(skt.socket);
        exit(EXIT_FAILURE);
    }
}

void skdCreateListener(SkdSocket& skt, uint64_t backlog)
{
    if (listen(skt.socket, backlog) < 0)
    {
        perror("Failed to create listener!");
        close(skt.socket);
        exit(EXIT_FAILURE);
    }
}

void skdSend(SkdSocket& skt, const char* msg, size_t size, int flags)
{
    send(skt.socket, msg, size, flags);
}

void skdSendTo(SkdSocket& skt, const char* msg, size_t size, int flags)
{
    // sendto(skt.socket, msg, size, flags, (struct sockaddr*)skt.specs.address.data, sizeof(skt.specs.address.data));
}

int64_t skdReceive(SkdSocket& skt, char* buffer, size_t size, int flags)
{
    uint64_t bytes = 0;
    bytes = recv(skt.socket, buffer, size, flags);
    return bytes;
}

int64_t skdReceiveFrom(SkdSocket& skt, char* buffer, size_t size, int flags)
{
    // int addr_size = sizeof(skt.specs.address.data);
    // return recvfrom(skt.socket, buffer, size, flags, (struct sockaddr*)skt.specs.address.data, &addr_size);
    return 0;
}

void skdPrintSocketAsNetwork(SkdSocket& skt)
{
    std::string family = skdAddressFamilyToString(skt.specs.family);
    std::cout <<
        "Socket Data:\n" <<
        "\t- Socket->[" << skt.socket << "]\n"
        "\t- Specs:\n" <<
        "\t\t- Family->[" << family << "]\n" <<
        "\t\t- Port->[" << skt.specs.port << "]\n" <<
        "\t\t- Address->[" << skt.specs.address.data << "]\n";
    return;
}

void skdPrintSocketAsHost(SkdSocket& skt)
{
    std::string family = skdAddressFamilyToString(skt.specs.family);
    uint16_t port = skdGetPortAsHost(skt.specs.port);
    char* address = skdGetIPv4AddressAsHost(skt);
    std::cout <<
        "Socket Data:\n" <<
        "\t- Socket->[" << skt.socket << "]\n" <<
        "\t- Specs:\n" <<
        "\t\t- Family->[" << family << "]\n" <<
        "\t\t- Port->[" << port << "]\n" <<
        "\t\t- Address->[" << address << "]\n";
    return;
}

void skdPrintSocket(SkdSocket& skt)
{
    std::string family = skdAddressFamilyToString(skt.specs.family);
    uint16_t port = skdGetPortAsHost(skt.specs.port);
    std::string address = skdGetIPv4AddressAsHost(skt);

    std::cout <<
        "Socket Data:\n" <<
        "\t- Socket->[" << skt.socket << "]\n" <<
        "\t- Specs:\n" <<
        "\t\t- Family->[" << family << "]\n" <<
        "\t\t- Port->[" << skt.specs.port << "]->[" << port << "]\n" <<
        "\t\t- Address->[" << skt.specs.address.data << "]->[" << address << "]\n";
    return;
}

#endif