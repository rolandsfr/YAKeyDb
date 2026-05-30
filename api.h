#pragma once

#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char* key;
    char* value;
} Entry;

typedef struct  {
    size_t size;
    size_t capacity;
    Entry* entry_list;
} Hashtable;

void init_table(Hashtable* table);

void print_table(Hashtable* table);

void add_item(Hashtable* table, char* key, char* value);

char* get_item(Hashtable*table, char*key);

void free_table(Hashtable* table);

/** IO */ 

void save_table(FILE* file, Hashtable* table);

void load_file(FILE* file, Hashtable* table);