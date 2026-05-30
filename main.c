#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "cli.h"



int main(int argc, char** argv) {
    Hashtable table;
    init_table(&table);


    // get store file
    printf("please enter storefile path: ");
    char storefile[256];
    scanf("%s", storefile);

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
                printf("expected 2 args.\n");
            }

        } else if(strcmp(arg_payload[0], "DELETE") == 0) {

        } else if(strcmp(arg_payload[0], "GET") == 0) {

        } else if(strcmp(arg_payload[0], "LIST") == 0) {

        } else if (strcmp(arg_payload[0], "EXIT") == 0) {
           break; 
        }

    }


    print_table(&table);

    // TODO: free table
}