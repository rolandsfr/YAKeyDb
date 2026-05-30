#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "cli.h"

int main() {
    Hashtable table;
    init_table(&table);

    // get store file
    printf("please enter storefile path: ");
    char filepath[1024] = {0};
    fgets(filepath, sizeof(filepath), stdin);
    filepath[strcspn(filepath, "\r\n")] = '\0';
    FILE* file = fopen(filepath, "r+");
    
    if(file == NULL) {
        printf("Failed to open the file. Starting without data.\n");
    } else {
        load_file(file, &table);
        fclose(file);
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
            if(word_count == 3) {
                add_item(&table, arg_payload[1], arg_payload[2]);
                printf(">> success\n");
            } else {
                printf("expected 2 args (key value pair).\n");
            }

        } else if(strcmp(arg_payload[0], "DELETE") == 0) {
            if(word_count == 2) {
                int res = delete_item(&table, arg_payload[1]);
                if(res == -1) {
                    printf(">> fail\n");
                } else {
                    printf(">> success\n");
                }
            } else {
                printf("expected 1 arg (key).\n");
            }
        } else if(strcmp(arg_payload[0], "GET") == 0) {
            if(word_count == 2) {
                char* value = get_item(&table, arg_payload[1]);
                if(!value) {
                    printf(">> null\n");
                } else {
                    printf(">> %s\n", value);
                }
            } else {
                printf("expected 1 arg (key).\n");
            }
        } else if(strcmp(arg_payload[0], "LIST") == 0) {
            print_table(&table);
        } else if(strcmp(arg_payload[0], "COUNT") == 0) {
            printf("%zu\n", table.size);
        } else if(strcmp(arg_payload[0], "EXISTS") == 0) {
            if(word_count == 2) {
                int exists = get_item_idx(&table, arg_payload[1]);
                if(exists == -1) {
                    printf(">> False\n");
                } else {
                    printf(">> True\n");
                }
            } else {
                printf("expected 1 arg (key).\n");
            }
        } else if (strcmp(arg_payload[0], "EXIT") == 0) {
            file = fopen(filepath, "w"); // reopen to truncate contents to overwrite with updated data
            if(!file) {
                printf("failed to open the file.\n");
                return 1;
            }
            save_table(file, &table);
            fclose(file);
            break;
        } else {
            printf(">> unknown instruction\n");
        }

    }

    free_table(&table);
}