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
    close(skt.socket);
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
#endif