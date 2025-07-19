#ifndef HTTP_BASE_H
#define HHTP_BASE_H
#include <stdint.h>
#include <stddef.h>
typedef struct HTTP HTTP;
typedef struct HTTPrequests{
    char method[16]; // резервация 16 символов(GET)
    char path[2048]; // резервация 2 мб памяти тк это предел для chromе
    char prot[16];
    // для предотвращения проблем с буфером
    uint8_t state; // счет состояний
    size_t ind; // посимвольный счет
} HTTPrequests;
extern HTTP* new_http(char* address);
extern void freehttp(HTTP* http);
extern void handle_http(HTTP* http, char* path, void(*)(int, HTTPrequests*));
extern int8_t listen(HTTP* http);
extern void htmlparse_http(int connect, char* name);

#endif /* HTTP_BASE_H*/