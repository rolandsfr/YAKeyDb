#pragma once

// typedef enum {SET, DELETE, GET, LIST, EXIT} cmd_type;

// per cmd input
#define MAX_ARG_COUNT 3

// returns full word count
int parse_buf(char* buf, char params[][256]);