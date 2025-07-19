#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "net.h"
#include "httpbase.h"
#define METHOD_SIZE 16
#define PATH_SIZE   2048
#define PROTO_SIZE  16

typedef struct HTTP{
    char* host;
    int32_t len;
    int32_t cap;
    void(**funcs)(int, HTTPrequests*);
    HashTab* tab;
} HTTP;


extern HTTP* new_http(char* address){
    HTTP* http = (HTTP*)malloc(sizeof(HTTP));
    http->cap = 1000;
    http->len = 0;
    http->host = (char*)malloc(sizeof(char) * strlen(address) + 1);
    strcpy(http->host, address);
    http->tab = new_hashtab(http->cap, STRING_TYPE, DECIMAL_TYPE);
    http->funcs = (void(**funcs)(int, HTTPrequests*))malloc(http->cap * sizeof(void(**funcs)(int, HTTPrequests*)));
    return http;
}

extern void freehttp(HTTP* http){
    free_hashtab(http->tab);
    free(http->host);
    free(http->funcs);
    free(http);
}

extern void handle_http(HTTP* http, char* path, void(*handle)(int, HTTPrequests*)){
    set_hashtab(http->tab, string(path), decimal(http->len));
	http->funcs[http->len] = handle;
	http->len += 1;
	if (http->len == http->cap) {
		http->cap <<= 1;
		http->funcs = (void(**)(int, HTTPrequests*))realloc(http->funcs, 
			http->cap * (sizeof (void(*)(int, HTTPrequests*))));
	}
}

static HTTPrequests new_request(void) {
	return (HTTPrequests){
		.method = {0},
		.path = {0},
		.prot = {0},
		.state = 0,
		.ind = 0,
	};
}

static void null_request(HTTPrequests *request) {
	request->state += 1;
	request->ind = 0;
}

static void _parse_request(HTTPrequests *request, char *buffer, size_t size) {
	// printf("%s\n", buffer);
	for (size_t i = 0; i < size; ++i) {
		switch(request->state) {
			case 0:
				if (buffer[i] == ' ' || request->ind == METHOD_SIZE-1) {
					request->method[request->ind] = '\0';
					null_request(request);
					continue;
				}
				request->method[request->ind] = buffer[i];
			break;
			case 1:
				if (buffer[i] == ' ' || request->ind == PATH_SIZE-1) {
					request->path[request->ind] = '\0';
					null_request(request);
					continue;
				}
				request->path[request->ind] = buffer[i];
			break;
			case 2:
				if (buffer[i] == '\n' || request->ind == PROTO_SIZE-1) {
					request->prot[request->ind] = '\0';
					null_request(request);
					continue;
				}
				request->prot[request->ind] = buffer[i];
			break;
			default: return;
		}
		request->ind += 1;
	}
}

static int8_t _switch_http(HTTP *http, int conn, HTTPrequests *request) {
	if (!in_hashtab(http->tab, string(request->path))) {
		char buffer[PATH_SIZE];
		memcpy(buffer, request->path, PATH_SIZE);
		int32_t index = strlen(request->path);
		if (index == 0) {
			_page404_http(conn);
			return 1;
		}
		index -= 1;
//		buffer[index] = '\0';
		for (; index > 0 && buffer[index] != '/'; --index) {
			buffer[index] = '\0';
		}
		if(!in_hashtab(http->tab, string(buffer))) {
			_page404_http(conn);
			return 2;
		}
		index = get_hashtab(http->tab, string(buffer)).decimal;
		http->funcs[index](conn, request);
		return 0;
	}
	int32_t index = get_hashtab(http->tab, string(request->path)).decimal;
	http->funcs[index](conn, request);
	return 0;
}

extern int8_t listen(HTTP* http){
    int listener = listen_net(http->host);
	if (listener < 0) {
		return 1;
	}
	while(1) {
		int conn = accept_net(listener);
		if (conn < 0) {
			continue;
		}
		HTTPrequests req = new_request();
		while(1) {
			char buffer[BUFSIZ] = {0};
			int n = recv_net(conn, buffer, BUFSIZ);
			if (n < 0) {
				break;
			}
			parse_request(&req, buffer, n);
			if (n != BUFSIZ) {
				break;
			}
		}
		switch_http(http, conn, &req);
		close_net(conn);
	}
	close_net(listener);
	return 0;
}

extern void htmlparse_http(int connect, char* name){
    char buffer[BUFSIZ] = "HTTP/1.1 200 OK\nContent-type: text/html\n\n";
    size_t readsz = strlen(buffer);
    send_net(connect, buffer, readsz);
    FILE* file = fopen(filename, "r");
    if(file == NULL)
        return;
    while((readsz = fread(buffer, sizeof(char), BUFSIZ, file)) != 0)
        send_net(connect, buffer, readsz);
    fclose(file);
}