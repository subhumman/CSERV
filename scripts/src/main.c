#include <string.h>
#include "headers/httpbase.h"

// Handler for "/" route. Serves index.html or 404 if path is not "/"
void pageindex(int connect, HTTPrequests *req){
    if(strcmp(req->path, "/") != 0){
        htmlparse_http(connect, "404 ERR");
        return; // Early exit on wrong path
    }
    htmlparse_http(connect, "index.html");
}

// Handler for "/scream" route. Serves scream.html or 404 if path is not "/scream"
void pagescream(int connect, HTTPrequests *req){
    if(strcmp(req->path, "/scream") != 0){
        htmlparse_http(connect, "404 ERR");
        return;
    }
    htmlparse_http(connect, "scream.html");
}

// Main entry point: creates HTTP server, registers routes, and starts server loop
int main(void){
    HTTP *server = new_http("127.0.0.1:8080");
    handle_http(server, "/", pageindex);
    handle_http(server, "/scream", pagescream);
    listen(server); // Start the server loop
}

