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
extern int8_t listen(HTTP* http);
static int8_t switch_http(HTTP *http, int conn, HTTPrequests *request);
static void parse_request(HTTPrequests *request, char *buffer, size_t size);
static void null_request(HTTPrequests *request);
static HTTPrequests new_request(void);
static void page404_html(int connect);

#endif /* HTTP_BASE_H*/