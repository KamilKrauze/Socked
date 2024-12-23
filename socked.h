#ifndef SOCKED_H
#define SOCKED_H

#if defined(_WIN32)
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib") // Link the Winsock library
#include "sockedwin32.h"

inline WSADATA skdWSA;

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
    sockaddr_in address;
    uint32_t port;
};

void skdInitSocket();
void skdCreateSocket(SKDSocket& skt, int af, int type, int protocol);
void skdBindSocketBin(SKDSocket& skt, uint16_t family, uint32_t address, uint32_t port);
void skdBindSocketStr(SKDSocket& skt, uint16_t family, const char* address, uint32_t port);
void skdCreateListener(SKDSocket& skt, uint64_t backlog);
void skdCloseSocket(SKDSocket& skt);
void skdDestroySocket(SKDSocket& skt);

#endif // !SOCKED_H

