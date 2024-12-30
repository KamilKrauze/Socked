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

typedef uint64_t skdSocketRef;

/// <summary>
/// Stores socket specification.
/// </summary>
struct SkdSocketSpecs
{
    // Address family IPv4/IPv6
    uint16_t family;

    // Network port
    uint16_t port;
    
    // Network address
    union Address
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

/// <summary>
/// The network socket data
/// </summary>
struct SkdSocket
{
    // Socket pointer
    skdSocketRef socket;

    // Socket specification
    SkdSocketSpecs specs;
};

/* Socket creation/destruction */

void skdInitSocket();
void skdCreateSocket(SkdSocket& skt, int af, int type, int protocol);
void skdCloseSocket(SkdSocket& skt);
void skdCleanupSocket();

/* Socket mutation */

void skdSetSocketOpt(SkdSocket& skt, int level, int optname, int optval);
void skdSetSocketSpecs(SkdSocket& skt, uint16_t family, const char* address, uint16_t port);

/* Socket connection/listening */

skdSocketRef skdAccept(SkdSocket& server_skt, SkdSocket& client_skt);
void skdBindSocket(SkdSocket& skt, uint16_t family, const char* address, uint16_t port);
void skdConnectSocket(SkdSocket& skt);
void skdCreateListener(SkdSocket& skt, uint64_t backlog);

/* Socket data sending and retrieval */

// Send data via TCP
void skdSend(SkdSocket& skt, const char* msg, size_t size, int flags);

// Send data via Datagram
void skdSendTo(SkdSocket& skt, const char* msg, size_t size, int flags);

// Receive data from TCP connection
uint64_t skdReceive(SkdSocket& skt, char* buffer, size_t size, int flags);

// Receive data from Datagram connection.
uint64_t skdReceiveFrom(SkdSocket& skt, char* buffer, size_t size, int flags);

#endif // !SOCKED_H

