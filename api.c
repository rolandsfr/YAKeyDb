#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "parse.h"

static int save_table_to_file(char* filename, Hashtable* table) {
    FILE* file = fopen(filename, "w");
            
    if(!file) {
        printf("failed to open the file.\n");
        return 1;
    }
    save_table(file, table);
    fclose(file);
    return 0;
}

int handle_requests(char* buffer, FILE* logfile, Hashtable* table, arg_handle_t* arg_handle) {
    char arg_payload[MAX_ARG_COUNT][256] = {0};
    int word_count = parse_buf(buffer,   arg_payload);

    if(strcmp(arg_payload[0], "SET" ) == 0) {
        // expect key and value
        if(require_words(3, word_count)) {
            keydb_log(logfile, arg_payload, word_count);
            add_item(table, arg_payload[1], arg_payload[2]);
            printf(">> success\n");
        }

    } else if(strcmp(arg_payload[0], "DELETE") == 0) {
        if(require_words(2, word_count)) {
            keydb_log(logfile, arg_payload, word_count);
            if(delete_item(table, arg_payload[1]) != 0) {
                printf(">> fail\n");
            } else {
                printf(">> success\n");
            }
        }
    } else if(strcmp(arg_payload[0], "GET") == 0) {
        keydb_log(logfile, arg_payload, word_count);
        if(require_words(2, word_count)) {
            char *value = get_item(table, arg_payload[1]);
            if (!value) {
                printf(">> null\n");
            } else {
                printf(">> %s\n", value);
            }
        }
    } else if(strcmp(arg_payload[0], "LIST") == 0) {
        keydb_log(logfile, arg_payload, word_count);
        print_table(table);
    } else if(strcmp(arg_payload[0], "COUNT") == 0) {
        keydb_log(logfile, arg_payload, word_count);
        printf("%zu\n", table->size);
    } else if(strcmp(arg_payload[0], "EXISTS") == 0) {
        keydb_log(logfile, arg_payload, word_count);
        if(require_words(2, word_count)) {
            if(get_item_idx(table, arg_payload[1]) == -1) {
                printf(">> False\n");
            } else {
                printf(">> True\n");
            }
        }
    } else if (strcmp(arg_payload[0], "EXIT") == 0 || strcmp(arg_payload[0], "SAVE") == 0) {
        keydb_log(logfile, arg_payload, word_count);
        char filename[1024];

        // not set filename
        if(!arg_handle->filename) {
            arg_handle->filename = malloc(strlen(filename) + 1);
            printf(">> No data file was provided in cli arguments. Please provide one to save progress: \n");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\r\n")] = '\0';

            // if malloc did not fail
            if(arg_handle->filename) {
                strcpy(arg_handle->filename, filename);
            } 

            save_table_to_file(filename, table);
        }  else {
            save_table_to_file(arg_handle->filename, table);
        }

        if(strcmp(arg_payload[0], "EXIT") == 0) return 2;
    } else {
        printf(">> unknown instruction\n");
    }

    return 0;
}
