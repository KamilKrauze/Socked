#ifndef SOCKED_H
#define SOCKED_H

#if defined(_WIN32)
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib") // Link the Winsock library
#include "sockedwin32.h"

#elif defined (__linux__)
#include "sockedlinux.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#else
#error Platform is not supported currently!
#endif

#include <cstdint>
struct SKDSocket
{
    uint64_t socket;
    sockaddr_in specs;
};

void skdInitSocket();
void skdCreateSocket(SKDSocket& skt, int af, int type, int protocol);
void skdCloseSocket(SKDSocket& skt);
void skdDestroySocket(SKDSocket& skt);

void skdSetSocketOpt(SKDSocket& skt, int level, int optname, int optval);
void skdSetSocketSpecs(SKDSocket& skt, uint16_t family, const char* address, uint16_t port);

void skdBindSocket(SKDSocket& skt, uint16_t family, const char* address, uint16_t port);
void skdConnectSocket(SKDSocket& skt);
void skdCreateListener(SKDSocket& skt, uint64_t backlog);


#endif // !SOCKED_H

