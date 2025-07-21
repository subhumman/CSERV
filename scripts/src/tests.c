#include "headers/tests.h"
#include "headers/httpbase.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Test HTTP request parsing logic
int test_parse_request() {
    HTTPrequests req = {0};
    char raw[] = "GET /scream HTTP/1.1\n";
    parse_request(&req, raw, strlen(raw));
    if (strcmp(req.method, "GET") != 0) {
        printf("test_parse_request: method fail\n");
        return 1;
    }
    if (strcmp(req.path, "/scream") != 0) {
        printf("test_parse_request: path fail\n");
        return 2;
    }
    if (strcmp(req.prot, "HTTP/1.1") != 0) {
        printf("test_parse_request: prot fail\n");
        return 3;
    }
    return 0;
}

// Dummy handler for routing test
static int called = 0;
void fake_handler(int conn, HTTPrequests *req) { (void)conn; (void)req; called = 1; }

// Test HTTP routing logic
int test_routing() {
    HTTP *server = new_http("127.0.0.1:8080");
    handle_http(server, "/test", fake_handler);
    HTTPrequests req = {0};
    strcpy(req.path, "/test");
    called = 0;
    int res =  switch_http(server, 0, &req);
    freehttp(server);
    if (!called) {
        printf("test_routing: handler not called\n");
        return 1;
    }
    if (res != 0) {
        printf("test_routing: switch_http returned error %d\n", res);
        return 2;
    }
    return 0;
}

// Run all tests and print summary
int run_all_tests(void) {
    int fails = 0;
    printf("Running test_parse_request...\n");
    fails += test_parse_request();
    printf("Running test_routing...\n");
    fails += test_routing();
    if (fails == 0) printf("All tests passed!\n");
    else printf("%d tests failed\n", fails);
    return fails;
}