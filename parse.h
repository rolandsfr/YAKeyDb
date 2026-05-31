#pragma once

#define MAX_ARG_COUNT 3

#define MAX_FLAG_COUNT 10

/**  returns full word count */
int parse_buf(char* buf, char params[][256]);

typedef struct {
    char* filename;
    char* flags[MAX_FLAG_COUNT];
    int flag_count;
} arg_handle_t;

void init_arg_handle(arg_handle_t* arg_handle);

void parse_argv(int argc, char** argv, arg_handle_t* arg_handle);

int has_flag(arg_handle_t arg_handle, const char* flag_name);