// HTTP server implementation with routing and request handling
// Provides a simple HTTP server with path-based routing

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "net.h"
#include "httpbase.h"

// Buffer size constants for HTTP parsing
#define METHOD_SIZE 16
#define PATH_SIZE   2048
#define PROTO_SIZE  16

// HTTP server structure containing routing information
typedef struct HTTP{
    char* host;         // Server host address
    int32_t len;        // Number of registered routes
    int32_t cap;        // Capacity of routes array
    void(**funcs)(int, HTTPrequests*); // Array of route handler functions
    HashTab* tab;       // Hash table mapping paths to handler indices
} HTTP;

// Create a new HTTP server instance
extern HTTP* new_http(char* address){
    HTTP* http = (HTTP*)malloc(sizeof(HTTP));
    http->cap = 1000;   // Initial capacity for routes
    http->len = 0;      // No routes initially
    // Allocate and copy host address
    http->host = (char*)malloc(sizeof(char) * strlen(address) + 1);
    strcpy(http->host, address);
    // Create hash table for path-to-handler mapping
    http->tab = new_hashtab(http->cap, STRING_TYPE, DECIMAL_TYPE);
    // Allocate array for handler functions
    http->funcs = (void(*)(int, HTTPrequests*))malloc(http->cap * sizeof(void(*)(int, HTTPrequests*)));
    return http;
}

// Free all memory allocated for HTTP server
extern void freehttp(HTTP* http){
    free_hashtab(http->tab);
    free(http->host);
    free(http->funcs);
    free(http);
}

// Register a new route handler for a specific path
extern void handle_http(HTTP* http, char* path, void(*handle)(int, HTTPrequests*)){
    // Store path-to-index mapping in hash table
    set_hashtab(http->tab, string(path), decimal(http->len));
    // Store handler function in array
    http->funcs[http->len] = handle;
    http->len += 1;
    // Expand capacity if needed
    if (http->len == http->cap) {
        http->cap <<= 1; // Double the capacity
        http->funcs = (void(**)(int, HTTPrequests*))realloc(http->funcs, 
            http->cap * (sizeof (void(*)(int, HTTPrequests*))));
    }
}

// Create a new empty HTTP request structure
static HTTPrequests new_request(void) {
    return (HTTPrequests){
        .method = {0},  // Initialize method buffer
        .path = {0},    // Initialize path buffer
        .prot = {0},    // Initialize protocol buffer
        .state = 0,     // Initial parsing state
        .ind = 0,       // Initial character index
    };
}

// Reset request parsing state for next field
static void null_request(HTTPrequests *request) {
    request->state += 1; // Move to next parsing state
    request->ind = 0;    // Reset character index
}

// Parse HTTP request line (method, path, protocol)
static void parse_request(HTTPrequests *request, char *buffer, size_t size) {
    // printf("%s\n", buffer);
    for (size_t i = 0; i < size; ++i) {
        switch(request->state) {
            case 0: // Parsing HTTP method
                if (buffer[i] == ' ' || request->ind == METHOD_SIZE-1) {
                    request->method[request->ind] = '\0';
                    null_request(request);
                    continue;
                }
                request->method[request->ind] = buffer[i];
            break;
            case 1: // Parsing request path
                if (buffer[i] == ' ' || request->ind == PATH_SIZE-1) {
                    request->path[request->ind] = '\0';
                    null_request(request);
                    continue;
                }
                request->path[request->ind] = buffer[i];
            break;
            case 2: // Parsing HTTP protocol
                if (buffer[i] == '\n' || request->ind == PROTO_SIZE-1) {
                    request->prot[request->ind] = '\0';
                    null_request(request);
                    continue;
                }
                request->prot[request->ind] = buffer[i];
            break;
            default: return; // Parsing complete
        }
        request->ind += 1;
    }
}

// Send 404 Not Found response
static void page404_html(int connect){
    char* header = "HTML/1.1 404 Not Found\n\nnot found";
    size_t headsz = strlen(header);
    send_net(connect, header, headsz);
}

// Route incoming request to appropriate handler
static int8_t switch_http(HTTP *http, int conn, HTTPrequests *request) {
    // Check if exact path exists
    if (!in_hashtab(http->tab, string(request->path))) {
        char buffer[PATH_SIZE];
        memcpy(buffer, request->path, PATH_SIZE);
        int32_t index = strlen(request->path);
        if (index == 0) {
            page404_http(conn);
            return 1;
        }
        index -= 1;
//      buffer[index] = '\0';
        // Try to find parent directory handler
        for (; index > 0 && buffer[index] != '/'; --index) {
            buffer[index] = '\0';
        }
        if(!in_hashtab(http->tab, string(buffer))) {
            page404_http(conn);
            return 2;
        }
        // Call parent directory handler
        index = get_hashtab(http->tab, string(buffer)).decimal;
        http->funcs[index](conn, request);
        return 0;
    }
    // Call exact path handler
    int32_t index = get_hashtab(http->tab, string(request->path)).decimal;
    http->funcs[index](conn, request);
    return 0;
}

// Start HTTP server and listen for connections
extern int8_t listen(HTTP* http){
    // Create listening socket
    int listener = listen_net(http->host);
    if (listener < 0) {
        return 1;
    }
    // Main server loop
    while(1) {
        // Accept new connection
        int conn = accept_net(listener);
        if (conn < 0) {
            continue;
        }
        // Initialize request structure
        HTTPrequests req = new_request();
        // Read and parse HTTP request
        while(1) {
            char buffer[BUFSIZ] = {0};
            int n = recv_net(conn, buffer, BUFSIZ);
            if (n < 0) {
                break;
            }
            parse_request(&req, buffer, n);
            if (n != BUFSIZ) {
                break; // End of request
            }
        }
        // Route request to appropriate handler
        switch_http(http, conn, &req);
        close_net(conn);
    }
    close_net(listener);
    return 0;
}

// Send HTML file as HTTP response
extern void htmlparse_http(int connect, char* name){
    // Send HTTP 200 OK response with HTML content type
    char buffer[BUFSIZ] = "HTTP/1.1 200 OK\nContent-type: text/html\n\n";
    size_t readsz = strlen(buffer);
    send_net(connect, buffer, readsz);
    // Open and send HTML file content
    FILE* file = fopen("outfile", "r");
    if(file == NULL)
        return;
    while((readsz = fread(buffer, sizeof(char), BUFSIZ, file)) != 0)
        send_net(connect, buffer, readsz);
    fclose(file);
}