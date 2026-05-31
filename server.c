#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

#include "core.h"
#include "parse.h"
#include "api.h"
#include "server.h"


void init_socket_handle(socket_handle* s_handle, struct sockaddr_in* address, int port, socklen_t addrlen) {
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(port);
    s_handle->address = (struct sockaddr*) address;
    s_handle->addrlen = addrlen;
    s_handle->port = port;
}

int init_socket(socket_handle* s_handle) {
    int opt = 1;

    s_handle->server_socket_fd = socket(AF_INET, SOCK_STREAM, 0); // local TCP without flags
    if(!s_handle->server_socket_fd ) {
        printf("Socket failed.\n");
        return 0;
    }

    if(setsockopt( s_handle->server_socket_fd , SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        printf("Socket option set failed.\n");
        return 0;
    }

    if(bind(s_handle->server_socket_fd , s_handle->address, s_handle->addrlen) < 0) {
        printf("Socket bind failed.\n");
        return 0;
    }

    return 1;
}

int run_server(socket_handle* s_handle, FILE* logfile, Hashtable* table, arg_handle_t* arg_handle ) {
    if(!s_handle->server_socket_fd) {
        printf("No socket handle for server.\n");
        return 0;
    }

    if(listen(s_handle->server_socket_fd, 3) < 0) {
        printf("Socket listen failed.");
        return 0;
    }

    printf("Listening on 0.0.0.0:%d (connect via 127.0.0.1:%d from this machine)\n", s_handle->port, s_handle->port);


    char buffer[1024];

    while(1) {
        int com_socket;
        if((com_socket = accept(s_handle->server_socket_fd, s_handle->address, &s_handle->addrlen)) < 0) {
            printf("Failed to accept new socket connection.\n");
            return 1;
        }

        while(1) {
            size_t nbytes = read(com_socket, buffer, sizeof(buffer));

            // client closed
            if(nbytes == 0) {
                break;
            }
            // error
            if(nbytes < 1) {
                printf("Error while receiving data from client.\n");
                break;
            } 

            handle_requests(buffer, logfile, table, arg_handle);
        }
    }

}