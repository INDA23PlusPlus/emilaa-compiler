#pragma once

#include "defines.h"

typedef enum Type {
    UNKNOWN = -1,
    IDENTIFIER,
    NUMBER,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    ADD,
    SUB,
    MUL,
    DIV,
    WHILE,
    IF,
    EQUAL,
    EQCMP,
    LET,
    GRT,
    LEE,
    GRE,
    SEMICOLON,
    DOT,
    PRINTCOMMAND,
    MAIN,
    FDECL,
    IDECL,
    RETURN,
    END
} Type;

typedef struct Token {
    char *s;
    Type t;
} Token;

i32 parse(char *input);

void factor();
void term();
void expr();
void cond();
void stmt();
void block();
void main_block();

void next_sym();
void error(const char *s, ErrorType t);
b8 accept();
void expect();
void check_decl();

b8 is_alpha(const char c);
b8 is_digit(const char c);

void trim_string(char *s);
u64 is_reserved(char *s, u64 pos);
Token *tokenize(char *s);
Type determine_token_type(const char *s);
