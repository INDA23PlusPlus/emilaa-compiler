#pragma once

#include "defines.h"

#define VECTOR_DEFAULT_CAPACITY 1
#define VECTOR_RESIZE_FACTOR 2

enum {
    VECTOR_CAPACITY,
    VECTOR_LENGTH,
    VECTOR_STRIDE,
    VECTOR_FIELD_LENGTH
};

void *_vector_create(u64 length, u64 stride);
void _vector_destroy(void *array);

u64 _vector_field_get(void *array, u64 field);
void _vector_field_set(void *array, u64 field, u64 value);

void *_vector_resize(void *array);

void *_vector_push(void *array, const void *value_ptr);
void _vector_pop(void *array, void *dst);

void *_vector_insert_at(void *array, u64 index, const void *value_ptr);
void *_vector_pop_at(void *array, u64 index, void *dst);

#define vector_create(type) \
        _vector_create(VECTOR_DEFAULT_CAPACITY, sizeof(type))

#define vector_reserve(type, capacity) \
        _vector_create(capacity, sizeof(type))
    
#define vector_destroy(array) _vector_destroy(array);

#define vector_push(array, value)               \
        {                                       \
            typeof(value) temp = value;         \
            array = _vector_push(array, &temp); \
        }

#define vector_pop(array, value_ptr) \
        _vector_pop(array, value_ptr)

#define vector_insert_at(array, index, value)               \
        {                                                   \
            typeof(value) temp = value;                     \
            array = _vector_insert_at(array, index, &temp); \
        }

#define vector_pop_at(array, index, value_ptr) \
        _vector_pop_at(array, index, value_ptr)

#define vector_clear(array) \
        _vector_field_set(array, VECTOR_LENGTH, 0)

#define vector_capacity(array) \
        _vector_field_get(array, VECTOR_CAPACITY)

#define vector_length(array) \
        _vector_field_get(array, VECTOR_LENGTH)

#define vector_stride(array) \
        _vector_field_get(array, VECTOR_STRIDE)

#define vecotr_length_set(array, value) \
        _vector_field_set(array, VECTOR_LENGTH, value)