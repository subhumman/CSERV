#ifndef HTTP_BASE_H
#define HHTP_BASE_H
#include <stdint.h>
#include <stddef.h>

typedef struct HTTP HTTP;
typedef struct HTTPrequests HTTPrequests;
extern HTTP* new_http(char* address);
extern void freehttp(HTTP* http);
extern void handle_http(HTTP* http, char* path, void(*)(int, HTTPrequests*));
extern int8_t listen(HTTP* http);
extern void htmlparse_http(int connect, char* name);

#endif /* HTTP_BASE_H*/