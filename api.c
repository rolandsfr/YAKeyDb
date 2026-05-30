#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "api.h"
#include "cli.h"

#define CAPACITY_UNIT 4

void init_entry_list(Hashtable* table, size_t capacity) {
    table->entry_list = malloc(sizeof(Entry) * capacity);
    
    if(!table->entry_list) {
        printf("Allocation failed!");
        return;
    }
    for(size_t i = 0; i < capacity; i ++) {
        table->entry_list[i] = (Entry){
            .key = NULL,
            .value = NULL
        };
    } 
}

void free_entry_list(Entry* entry_list, size_t capacity) {
    for(size_t i = 0; i < capacity; i++) {
        if(entry_list[i].key && entry_list[i].value) {
            free(entry_list[i].key);
            free(entry_list[i].value);
        }
    }

    free(entry_list);
}

void init_table(Hashtable* table) {
    table->capacity = CAPACITY_UNIT;
    table->size = 0;
    init_entry_list(table, CAPACITY_UNIT);
}

void print_table(Hashtable* table) {
    for(size_t i = 0; i < table->capacity; i++) {
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

void resize_table(Hashtable* table) {
    Entry* entry_list;
    size_t new_capacity = table->capacity * CAPACITY_UNIT;
    entry_list = malloc(sizeof(Entry) * new_capacity);
    
    if(!table->entry_list) {
        printf("Allocation failed!");
        return;
    }

    for(size_t i = 0; i < new_capacity; i++) {
        entry_list[i] = (Entry){
            .key = NULL,
            .value = NULL
        };
    }

    for(size_t i = 0; i < table->capacity; i++) {
        if(table->entry_list[i].key != NULL) {
            // rehash with new capacity
            int idx = hash(table->entry_list[i].key, new_capacity);
            int start = idx;
            while(entry_list[idx].key != NULL) {
                idx++;
                if(idx >= table->capacity) idx = 0;
                if(idx == start) return;
            }
            entry_list[idx] = table->entry_list[i];
        } 
    }

    free(table->entry_list); // no need to free strings, they will be used in new array
    table->capacity = new_capacity;
    table->entry_list = entry_list;
    printf("reallocated to capacity: %zu\n", table->capacity);
}

void add_item(Hashtable* table, char* key, char* value) {
    if(table->size == table->capacity) {
        resize_table(table);
    }
    
    size_t idx = hash(key, table->capacity);

    while(table->entry_list[idx].key != NULL) {
        if (strcmp(table->entry_list[idx].key, key) == 0) {
            // update value
            free(table->entry_list[idx].value);
            table->entry_list[idx].value = malloc(strlen(value) + 1);
            strcpy(table->entry_list[idx].value, value);
            return;
        }
        idx++;
        if(idx >= table->capacity) idx = 0;
    }

    table->entry_list[idx].key = malloc(strlen(key) + 1);
    strcpy(table->entry_list[idx].key, key);
    table->entry_list[idx].value = malloc(strlen(value) + 1);
    strcpy(table->entry_list[idx].value, value);
    table->size++;
}

int get_item_idx(Hashtable*table, char* key) {
    int idx = hash(key, table->capacity);
    int start = idx;
    while(table->entry_list[idx].key != NULL) {
        if(strcmp(table->entry_list[idx].key, key) == 0) {
            return idx;
        }
        idx++;
        if(idx >= table->capacity) idx = 0;
        if(idx == start) break;
    }
    return -1;
}

char* get_item(Hashtable* table, char* key) {
    int idx = get_item_idx(table, key);
    if(idx == -1) {
        return NULL;
    } else {
        return table->entry_list[idx].value;
    }
}

int delete_item(Hashtable* table, char* key) {
    int idx = get_item_idx(table, key);
    if(idx == -1) return 1;
    free(table->entry_list[idx].key);
    free(table->entry_list[idx].value);
    table->entry_list[idx] = (Entry){
        .key = NULL,
        .value = NULL
    };
    return 0;
}

void free_table(Hashtable* table) {
    free_entry_list(table->entry_list, table->capacity);
    table->entry_list = NULL;
    table->capacity = 0;
    table->size = 0;
}

// FILE IO


void save_table(FILE* file, Hashtable* table) {
    for(size_t i = 0; i < table->capacity; i++) {
        if(!table->entry_list[i].key || !table->entry_list[i].value) continue;

        fprintf(file, "%s %s\n", table->entry_list[i].key, table->entry_list[i].value);
    }
}

void load_file(FILE* file, Hashtable* table) {
    char buf[256];
    char params[MAX_ARG_COUNT][256];

    while(fgets(buf, sizeof(buf), file)) {
        int word_count = parse_buf(buf, params);
        if(word_count < 2) {
            printf("bad input\n");
            continue;
        }
        add_item(table, params[0], params[1]);
    }
}