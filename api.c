#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "api.h"

#define CAPACITY_UNIT 64

void init_table(Hashtable* table) {
    table->capacity = CAPACITY_UNIT;

    table->entry_list = malloc(sizeof(Hashtable) * CAPACITY_UNIT);
    if(!table->entry_list) {
        printf("Allocation failed!");
        return;
    }
    for(int i = 0; i < CAPACITY_UNIT;i ++) {
        table->entry_list[i] = (Entry){
            .key = NULL,
            .value = NULL
        };
    } 

    table->size = 0;
}

void print_table(Hashtable* table) {
    for(int i = 0; i < table->capacity; i++) {
        Entry entry = table->entry_list[i];
        if(entry.key == NULL || entry.value == NULL) continue;
        printf("%s = %s\n", entry.key, entry.value);
    }
}

// uses djb2 hash
int hash(char* key, int capacity) {
    unsigned long hash = 5381;
    int c;

    while((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % capacity;
}

void add_item(Hashtable* table, char* key, char* value) {
    if(table->size == table->capacity) {
        table->capacity = table->capacity + CAPACITY_UNIT;
        table->entry_list = (Entry*) malloc(sizeof(Entry));

        // TODO: extract into shared init function

        if(!table->entry_list) {
            printf("Allocation failed!");
            return;
        }
        for(int i = 0; i < CAPACITY_UNIT;i ++) {
            table->entry_list[i] = (Entry){
                .key = NULL,
                .value = NULL
            };
        } 

    }

    table->size++;
    int idx = hash(key, table->capacity);
    while(table->entry_list[idx].key != NULL) {
        idx++;
    }


    table->entry_list[idx].key = malloc(sizeof(strlen(key) + 1));
    strcpy(table->entry_list[idx].key, key);
    table->entry_list[idx].value = malloc(sizeof(strlen(value + 1)));
    strcpy(table->entry_list[idx].value, value);
}