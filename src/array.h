#pragma once

#include "main.h"

typedef void (*array_callback) (void*);

struct array {
    void** data;

    uint32_t size;
    uint32_t capacity;
};

Array* array_create ();
Array* array_create_capacity (uint32_t capacity);

void array_destroy (Array* array);
void array_destroy_callback (Array* array, array_callback callback);

void array_add (Array* array, void* item);

void  array_push (Array* array, void* item);
void* array_pop  (Array* array);

void* array_get (Array* array, uint32_t index);
void* array_set (Array* array, uint32_t index, void* item);

void  array_insert (Array* array, uint32_t index, void* item);
void* array_remove (Array* array, uint32_t index);
