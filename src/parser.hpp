#pragma once

#include <string>
#include <vector>
#include <cstdio>

enum Symbol {
    IDENTIFIER = 0,
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
    EQUAL,
    EQCMP,
    LET,
    GRT,
    LEE,
    GRE,
    SEMICOLON,
    PRINTCOMMAND,
    END
};

const std::string sym_names[] = {
    "IDENTIFIER",
    "NUMBER",
    "LPAREN",
    "RPAREN",
    "LBRACE",
    "RBRACE",
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "WHILE",
    "EQUAL",
    "EQCMP",
    "LET",
    "GRT",
    "LEE",
    "GRE",
    "SEMICOLON",
    "PRINTCOMMAND",
    "EOF"
};

struct Token {
    Symbol symbol;
    std::string text;
};

class Parser {
    public:
        Token sym;
        size_t cursor{0};
        std::vector<Token> input{};

        Symbol determine_symbol(const std::string &s);
        bool parse();
        
    private:
        bool found_error{false};
        size_t indent{1};
        std::string output{"PROGRAM:\n"};

        void error(const char *s);
        bool is_alpha(const char c);
        bool is_digit(const char c);
        void next_sym();
        void write_token();

        bool expect(Symbol s);
        bool accept(Symbol s);
        void factor();
        void term();
        void expression();
        void condition();
        void block();
        void statement();
};