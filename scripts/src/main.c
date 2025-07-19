#include <string.h>
#include "headers/httpbase.h"


// pageindex error 
void pageindex(int connect, HTTPrequests *req){
    if(strcmp(req->path, "/") != 0){
        htmlparse_http(connect, "404 ERR");
        return; // досрочный выход
    }
    htmlparse_http(connect, "index.html");
}

// pagescream error
void pagescream(int connect, HTTPrequests *req){
    if(strcmp(req->path, "/scream") != 0){
        htmlparse_http(connect, "404 ERR");
        return;
    }
    htmlparse_http(connect, "scream.html");
}

// main realization
int main(void){
    HTTP *server = new_http("127.0.0.1:8080");
    handle_http(server, "/", pageindex);
    handle_http(server, "/scream", pagescream);
}

