#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli.h"

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