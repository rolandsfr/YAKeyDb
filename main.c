#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "core.h"
#include "parse.h"

int main(int argc, char** argv ) {
    Hashtable table;
    init_table(&table);

    arg_handle_t arg_handle;
    init_arg_handle(&arg_handle);
    parse_argv(argc, argv, &arg_handle);

    if(arg_handle.filename) {
        FILE* file = fopen(argv[1], "a+");
    
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

    char arg_payload[MAX_ARG_COUNT][256] = {0};
    int word_count = 0;

    // wait for commands
    while(1) {
        // reset command payload buffer
        memset(arg_payload, 0, sizeof(arg_payload));

        char buff[1024];
        fgets(buff, sizeof(buff), stdin);
        word_count = parse_buf(buff,   arg_payload);

        if(strcmp(arg_payload[0], "SET" ) == 0) {
            // expect key and value
            if(require_words(3, word_count)) {
                keydb_log(logfile, arg_payload, word_count);
                add_item(&table, arg_payload[1], arg_payload[2]);
                printf(">> success\n");
            }

        } else if(strcmp(arg_payload[0], "DELETE") == 0) {
            if(require_words(2, word_count)) {
                keydb_log(logfile, arg_payload, word_count);
                if(delete_item(&table, arg_payload[1]) != 0) {
                    printf(">> fail\n");
                } else {
                    printf(">> success\n");
                }
            }
        } else if(strcmp(arg_payload[0], "GET") == 0) {
            keydb_log(logfile, arg_payload, word_count);
            if(require_words(2, word_count)) {
                char *value = get_item(&table, arg_payload[1]);
                if (!value) {
                    printf(">> null\n");
                } else {
                    printf(">> %s\n", value);
                }
            }
        } else if(strcmp(arg_payload[0], "LIST") == 0) {
            keydb_log(logfile, arg_payload, word_count);
            print_table(&table);
        } else if(strcmp(arg_payload[0], "COUNT") == 0) {
            keydb_log(logfile, arg_payload, word_count);
            printf("%zu\n", table.size);
        } else if(strcmp(arg_payload[0], "EXISTS") == 0) {
            keydb_log(logfile, arg_payload, word_count);
            if(require_words(2, word_count)) {
                if(get_item_idx(&table, arg_payload[1]) == -1) {
                    printf(">> False\n");
                } else {
                    printf(">> True\n");
                }
            }
        } else if (strcmp(arg_payload[0], "EXIT") == 0) {
            keydb_log(logfile, arg_payload, word_count);
            char filename[1024];
            if(!arg_handle.filename) {
                printf(">> No data file was provided in cli arguments. Please provide one to save progress: \n");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\r\n")] = '\0';

                FILE* file = fopen(filename, "w");
            
                if(!file) {
                    printf("failed to open the file.\n");
                    return 1;
                }
                save_table(file, &table);
                fclose(file);
            }
            break;
        } else {
            printf(">> unknown instruction\n");
        }

    }

    if (logfile) {
        fclose(logfile);
    }
    free_table(&table);
}