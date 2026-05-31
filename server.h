#include <netinet/in.h>
#include <stdio.h>

#include "core.h"
#include "parse.h"

typedef struct {
    struct sockaddr* address;
    socklen_t addrlen;
    int server_socket_fd;
    int port;
} socket_handle;

void init_socket_handle(socket_handle* s_handle, struct sockaddr_in* address, int port, socklen_t addrlen);

int init_socket(socket_handle* s_handle);

int run_server(socket_handle* s_handle, FILE* logfile, Hashtable* table, arg_handle_t* arg_handle );