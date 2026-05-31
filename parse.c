#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"

int parse_buf(char* buf, char params[][256]) {
    char c;
    int idx = 0;
    int word = 0;
    char* start = buf;

    while((c = *buf++) && word < MAX_ARG_COUNT) {
        if(c == ' ' || c == '\n') {
            strncpy(params[word], start, sizeof(char) * (idx));
            // check for size constraints
            params[word][idx] = '\0';
            start = buf;
            word++;
            idx = 0;
        } else {
            idx++;
        }
    }


    // save last word if no \n present at the end
    if(idx > 0 && word < MAX_ARG_COUNT) {
        strncpy(params[word], start, sizeof(char) * (idx)); 
        params[word][idx] = '\0';
        word++;
    }

    return word;
}

void init_arg_handle(arg_handle_t* arg_handle) {
    arg_handle->filename = NULL;
    arg_handle->flag_count = 0;
    arg_handle->option_count = 0;

    for(int i = 0; i < MAX_FLAG_COUNT; i++) {
       arg_handle->flags[i] = NULL;
    }

    for(int i = 0; i < MAX_OPTION_COUNT; i++) {
       arg_handle->options[i].name = NULL;
       arg_handle->options[i].value = NULL;
    }
}

void parse_argv(int argc, char** argv, arg_handle_t* arg_handle) {
    // process first arg
    if(argc >= 2 && strncmp(argv[1], "--", 2) != 0) {
        arg_handle->filename = argv[1];
    }

    // process flags / options
    for(int i = 0; i < argc; i++) {
        if(argv[i] && strncmp(argv[i], "--", 2) == 0) {
            // option
            if(argc >= i + 2 && strncmp(argv[i + 1], "--", 2) != 0 ) {
                arg_handle->options[arg_handle->option_count] = (option_entry_t){
                    .name = argv[i],
                    .value = argv[i + 1]
                };
                arg_handle->option_count++;
            } else {
                // flag
                arg_handle->flags[arg_handle->flag_count] = argv[i];
                arg_handle->flag_count++;
            }
        }
    }
}

int has_flag(arg_handle_t arg_handle, const char* flag_name) {
    for(int i = 0; i < arg_handle.flag_count; i++) {
        if(strcmp(arg_handle.flags[i], flag_name) == 0) return 1;
    }
    return 0;
}

int get_option(arg_handle_t arg_handle, char* option_name, char** buf) {
    for(int i = 0; i < arg_handle.option_count; i++) {
        if(arg_handle.options[i].name && strcmp(arg_handle.options[i].name, option_name) == 0) {
            *buf = arg_handle.options[i].value;
            return 1;
        }
    }
    return 0;
}