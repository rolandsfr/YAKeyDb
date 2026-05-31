#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>

#include "core.h"
#include "parse.h"
#include "api.h"
#include "server.h"

int main(int argc, char** argv ) {
    Hashtable table;
    init_table(&table);

    arg_handle_t arg_handle;
    init_arg_handle(&arg_handle);
    parse_argv(argc, argv, &arg_handle);

    if(arg_handle.filename) {
        FILE* file = fopen(arg_handle.filename, "a+");
    
        if(file == NULL) {
            printf("Failed to open the file. Starting without data.\n");
        } else {
            load_file(file, &table);
            fclose(file);
        }
    }

    int use_log = has_flag(arg_handle, "--log");

    char logfile_name[16];
    FILE *logfile = NULL;

    if (use_log) {
        time_t now = time(NULL);
        snprintf(logfile_name, sizeof(logfile_name), "%lu.log", (unsigned long)now);
        logfile = fopen(logfile_name, "a");
        if (!logfile) {
            printf(">> [warn] Could not create logfile. Progress will not be logged.\n");
        }
    }

    char* port = NULL;
    if(get_option(arg_handle, "--port", &port))  {
        socket_handle s_handle;
        struct sockaddr_in address;
        socklen_t addrlen = sizeof(address);

        init_socket_handle(&s_handle, &address, atoi(port), addrlen);
        if(init_socket(&s_handle)) {
            run_server(&s_handle, logfile, &table, &arg_handle);
        }

    } else {
        char buff[1024];

        // REPL MODE
        while(1) {
            memset(buff, 0, sizeof(buff));
            fgets(buff, sizeof(buff), stdin);
            int res = handle_requests(buff, logfile, &table, &arg_handle);
            // exit
            if(res == 2) return 0;
            // error
            if(res != 0) return res;
        }
    }


    if (logfile) {
        fclose(logfile);
    }
    free_table(&table);
}