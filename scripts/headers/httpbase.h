#ifndef HTTP_BASE_H
#define HTTP_BASE_H
#include <stdint.h>
#include <stddef.h>

typedef struct HTTP HTTP;
typedef struct HTTPrequests{
    char method[16]; // Reserve 16 bytes for HTTP method
    char path[2048]; // Reserve 2048 bytes for path (not 2MB!)
    char prot[16];
    uint8_t state;
    size_t ind;
} HTTPrequests;

extern HTTP* new_http(char* address);
extern void freehttp(HTTP* http);
extern void handle_http(HTTP* http, char* path, void(*)(int, HTTPrequests*));
extern int8_t listen(HTTP* http);
extern void htmlparse_http(int connect, char* name);

#endif /* HTTP_BASE_H */