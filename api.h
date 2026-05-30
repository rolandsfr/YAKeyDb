#pragma once

typedef struct {
    char* key;
    char* value;
} Entry;

typedef struct  {
    int size;
    int capacity;
    Entry* entry_list;
} Hashtable;

void init_table(Hashtable* table);

void print_table(Hashtable* table);

void add_item(Hashtable* table, char* key, char* value);
