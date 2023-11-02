#include "vector.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void *_vector_create(u64 length, u64 stride) {
    u64 header_size = VECTOR_FIELD_LENGTH * sizeof(u64);
    u64 array_size = length * stride;
    u64 *new_array = malloc(header_size + array_size);
    memset(new_array, 0, header_size + array_size);
    new_array[VECTOR_CAPACITY] = length;
    new_array[VECTOR_LENGTH] = 0;
    new_array[VECTOR_STRIDE] = stride;
    return (void*)(new_array + VECTOR_FIELD_LENGTH);
}

void _vector_destroy(void *array) {
    u64 *header = (u64*)array - VECTOR_FIELD_LENGTH;
    free(header);
}


u64 _vector_field_get(void *array, u64 field) {
    u64 *header = (u64*)array - VECTOR_FIELD_LENGTH;
    return header[field];
}

void _vector_field_set(void *array, u64 field, u64 value) {
    u64 *header = (u64*)array - VECTOR_FIELD_LENGTH;
    header[field] = value;
}


void *_vector_resize(void *array) {
    u64 length = vector_length(array);
    u64 stride = vector_stride(array);
    void *temp = _vector_create(VECTOR_RESIZE_FACTOR * vector_capacity(array), stride);
    memcpy(temp, array, length * stride);
    _vector_field_set(temp, VECTOR_LENGTH, length);
    _vector_destroy(array);
    return temp;
}


void *_vector_push(void *array, const void *value_ptr) {
    u64 length = vector_length(array);
    u64 stride = vector_stride(array);
    if(length >= vector_capacity(array)) {
        array = _vector_resize(array);
    }

    u64 addr = (u64)array;
    addr += length * stride;
    memcpy((void*)addr, value_ptr, stride);
    _vector_field_set(array, VECTOR_LENGTH, length + 1);
    return array;
}

void _vector_pop(void *array, void *dst) {
    u64 length = vector_length(array);
    u64 stride = vector_stride(array);
    u64 addr = (u64)array;
    addr += (length - 1) * stride;
    memcpy(dst, (void*)addr, stride);
    _vector_field_set(array, VECTOR_LENGTH, length - 1);
}


void *_vector_insert_at(void *array, u64 index, const void *value_ptr) {
    u64 length = vector_length(array);
    u64 stride = vector_stride(array);
    if(index >= length) {
        printf("Index is out of bounds... Length: %llu | Index: %llu", length, index);
        return array;
    }
    if(length >= vector_capacity(array)) {
        array = _vector_resize(array);
    }

    u64 addr = (u64)array;

    if(index != length - 1) {
        memcpy(
            (void*)(addr + ((index + 1) * stride)),
            (void*)(addr + (index * stride)),
            stride * (length - index));
    }

    memcpy((void*)(addr + (index * stride)), value_ptr, stride);

    _vector_field_set(array, VECTOR_LENGTH, length + 1);
    return array;
}

void *_vector_pop_at(void *array, u64 index, void *dst) {
    u64 length = vector_length(array);
    u64 stride = vector_stride(array);
    if(index >= length) {
        printf("Index is out of bounds... Length: %llu | Index: %llu", length, index);
        return array;
    }

    u64 addr = (u64)array;
    memcpy(dst, (void*)(addr + (index * stride)), stride);

    if(index != length - 1) {
        memcpy(
            (void*)(addr + (index * stride)),
            (void*)(addr + ((index + 1) * stride)),
            stride * (length - index));
    }

    _vector_field_set(array, VECTOR_LENGTH, length - 1);
    return array;
}