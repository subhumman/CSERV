// Socket networking implementation for cross-platform support
// Supports both Linux and Windows platforms

#if __linux__
#include <unistd.h>
#include <arpa/inet.h>
#elif __WIN32
#include <WinSock2.h>
#else 
#warning "net.h: plat unwer"
#endif

#if defined(__linux__) || defined(__WIN32)
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "net.h"
#include "hash.h"

// Function prototype for parsing address string
static int8_t pars_address(char* address, char* ipv4, char* port);

// Create a listening socket on the specified address
extern int listen_net(char* address){
#ifdef __WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0){
        return -1;
    }
#endif
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0){
        return -2;
    }
    int opt = 1;
    if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0){
        return -3;
    }
    char ipv4[16];
    char port[6];
    if (pars_address(address, ipv4, port) != 0){
        return -4;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = inet_addr(ipv4);
    if(bind(listener, (struct sockaddr*)&addr, sizeof(addr))  != 0){
        return -5;
    }
    if(listen(listener, SOMAXCONN) != 0){
        return -6;
    }
    return listener;
}

// Accept a new connection on the listening socket
extern int accept_net(int listener){
    return accept(listener, NULL, NULL); // NULL because we don't care about client info
}

// Connect to a remote address
extern int connect_net(char* address){
#ifdef __WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0){
        return -7;
    }
#endif
    int conn = socket(AF_INET, SOCK_STREAM, 0);
    if (conn < 0){
        return -8;
    }
    char ipv4[16];
    char port[6];
    if (pars_address(address, ipv4, port) != 0){
        return -9;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = inet_addr(ipv4);
    if(connect(conn, (struct sockaddr*)&addr, sizeof(addr)) != 0){
        return -10;
    }
    return conn;
}

// Close a socket connection
extern int close_net(int conn){
#ifdef __linux__
    return close(conn);
#elif __WIN32
    return closesocket(conn);
#else
    return -1;
#endif
}

// Send data over a socket connection
extern int send_net(int conn, char* buf, size_t size){
    return send(conn, buf, (int)size, 0);
}

// Receive data from a socket connection
extern int recv_net(int conn, char* buf, size_t size){
    return recv(conn, buf, (int)size, 0);
}

// Parse address string in format "ip:port" into separate IP and port strings
static int8_t pars_address(char* address, char* ipv4, char* port){
    size_t i = 0, j = 0;
    for(; address[i] != ':' && address[i] != '\0'; ++i){
        if(i >= 15) return 2;
        ipv4[i] = address[i];
    }
    if(address[i] != ':') return 1;
    ipv4[i] = '\0';
    ++i;
    for(; address[i] != '\0'; ++i, ++j){
        if(j >= 5) return 3;
        port[j] = address[i];
    }
    port[j] = '\0';
    return 0;
}

#endif /* defined(__linux__) || defined(__WIN32) */