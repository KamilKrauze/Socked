#ifndef SOCKED_H
#define SOCKED_H

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") // Link the Winsock library
#include "sockedwin32.h"

#elif defined (__linux__)
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "sockedlinux.h"

#else
#error Platform is not supported currently!
#endif

#include <cstdint>

struct SkdSocketSpecs
{
    uint16_t family;
    uint16_t port;
    
    union
    {
        struct {
            uint8_t network; // Network identifier
            uint8_t host; // Host on the network
            uint8_t logic_host; // Logical host
            uint8_t imp_number; // IMP number
        } bytes;
        struct { 
            uint16_t unused; // ---
            uint16_t imp; // IMP field 
        } words;
        uint32_t data;
    } address;

    char zero[8];
};

struct SkdSocket
{
    uint64_t socket;
    SkdSocketSpecs specs;
};

void skdInitSocket();
void skdCreateSocket(SkdSocket& skt, int af, int type, int protocol);
void skdCloseSocket(SkdSocket& skt);
void skdCleanupSocket(SkdSocket& skt);

void skdSetSocketOpt(SkdSocket& skt, int level, int optname, int optval);
void skdSetSocketSpecs(SkdSocket& skt, uint16_t family, const char* address, uint16_t port);

void skdBindSocket(SkdSocket& skt, uint16_t family, const char* address, uint16_t port);
void skdConnectSocket(SkdSocket& skt);
void skdCreateListener(SkdSocket& skt, uint64_t backlog);

#endif // !SOCKED_H

