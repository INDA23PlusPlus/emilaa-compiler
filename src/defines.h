#pragma once

typedef signed char         i8;
typedef signed short        i16;
typedef signed int          i32;
typedef signed long long    i64;

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef float               f32;
typedef double              f64;

typedef signed char         b8;
typedef signed int          b32;

#define TRUE 1
#define FALSE 0

typedef enum ErrorType {
    UNKNOWN_ERROR = -1,
    SUCCESS,
    NO_INPUT,
    INPUT_NOT_FOUND,
    SYNTAX_ERROR,
    UNEXPECTED_TOKEN,
    SEMANTIC_ERROR,
    FAILED_TO_CREATE_FILE
} ErrorType;