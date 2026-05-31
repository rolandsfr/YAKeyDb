#pragma once

#define MAX_ARG_COUNT 3

#define MAX_FLAG_COUNT 5

#define MAX_OPTION_COUNT 5

/**  returns full word count */
int parse_buf(char* buf, char params[][256]);

typedef struct {
    char* name;
    char* value;
} option_entry_t;

typedef struct {
    option_entry_t options[MAX_OPTION_COUNT];
    int flag_count;
    int option_count;
    char* filename;
    char* flags[MAX_FLAG_COUNT];
} arg_handle_t;

void init_arg_handle(arg_handle_t* arg_handle);

void parse_argv(int argc, char** argv, arg_handle_t* arg_handle);

int has_flag(arg_handle_t arg_handle, const char* flag_name);

int get_option(arg_handle_t arg_handle, char* option_name, char** buf);
