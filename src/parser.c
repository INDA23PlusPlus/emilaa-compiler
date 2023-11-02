#include "parser.h"
#include "container/vector.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char *tok_name[26] = {
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
    "IF",
    "EQUAL",
    "EQCMP",
    "LET",
    "GRT",
    "LEE",
    "GRE",
    "SEMICOLON",
    "DOT",
    "PRINTCOMMAND",
    "MAIN",
    "FDECL",
    "IDECL",
    "RETURN",
    "END"
};

char **decl_list;

Token *tokens;
Token sym;
u64 cursor = 0;

i32 parse(char *input) {
    trim_string(input);
    tokens = tokenize(input);
    free(input);

    decl_list = vector_create(char *);

    next_sym();
    main_block();

    FILE *output = fopen("out/output.c", "w");
    if(!output) { exit(FAILED_TO_CREATE_FILE); }

    fprintf(output, "#include <stdint.h>\n#include <stdio.h>\n\n");

    u64 size = vector_length(tokens) - 1;
    b8 in_stmt = FALSE;

    for(u64 i = 0; i < size; i++) {
        if(in_stmt && tokens[i].t == LBRACE) {
            in_stmt = FALSE;
            fprintf(output, ") {\n");
        }
        else if(tokens[i].t == RBRACE ||
                tokens[i].t == LBRACE) {
            fprintf(output, "%s\n", tokens[i].s);
        }
        else if(tokens[i].t == SEMICOLON) {
            fprintf(output, "%s\n", tokens[i].s);
        } 
        else if(tokens[i].t == IDECL) {
            fprintf(output, "int ");
        } 
        else if(tokens[i].t == WHILE) {
            in_stmt = TRUE;
            fprintf(output, "while( ");
        }
        else if(tokens[i].t == MAIN) {
            fprintf(output, "main() ");
            free(tokens[i].s);
            free(tokens[i+1].s);
            i += 2;
        } else if(tokens[i].t == PRINTCOMMAND) {
            fprintf(output, "printf(\"%%d\\n\", ");
            free(tokens[i].s);
            i += 1;
        }
        else {
            fprintf(output, "%s ", tokens[i].s);
        }

        free(tokens[i].s);
    }

    fclose(output);

    vector_destroy(decl_list);
    vector_destroy(tokens);

    return SUCCESS;
}

void factor() {
    if(accept(IDENTIFIER)) {}
    else if(accept(NUMBER)) {}
    else if(accept(LPAREN)) {
        expr();
        expect(RPAREN);
    } else {
        error("Syntax error.", SYNTAX_ERROR);
    }
}

void term() {
    factor();
    while(sym.t == MUL || sym.t == DIV) {
        next_sym();
        term();
    }
}

void expr() {
    term();
    while(sym.t == ADD || sym.t == SUB) {
        next_sym();
        term();
    }
}

void cond() {
    expr();
    if(sym.t == EQCMP ||
       sym.t == LET   ||
       sym.t == LEE   ||
       sym.t == GRT   ||
       sym.t == GRE
    ) {
        next_sym();
        expr();
    } else {
        error("Invalid conditional operator.", UNEXPECTED_TOKEN);
        next_sym();
    }
}

void stmt() {
    if(accept(IDECL)) {                 // DECLARATION
        vector_push(decl_list, tokens[cursor-1].s);
        expect(IDENTIFIER);
        expect(EQUAL);
        expr();
        expect(SEMICOLON);
    } else if(accept(IDENTIFIER)) {     // ASSIGNMENT
        expect(EQUAL);
        expr();
        expect(SEMICOLON);
    } else if(accept(IF)) {             // IF STATEMENT
        cond();
        expect(LBRACE);
        block();
        expect(RBRACE);
    } else if(accept(WHILE)) {          // WHILE BLOCK
        cond();
        expect(LBRACE);
        block();
        expect(RBRACE);
    } else if(accept(PRINTCOMMAND)) {   // PRINT COMMAND
        expect(LPAREN);
        check_decl();
        expect(IDENTIFIER);
        expect(RPAREN);
        expect(SEMICOLON);
    } else if(accept(RETURN)) {
        expr();
        expect(SEMICOLON);
    }
}

void block() {
    stmt();
    if(sym.t != END && sym.t != RBRACE) {
        block();
    }
}

void main_block() {
    expect(IDECL);
    expect(MAIN);
    expect(LPAREN);
    expect(RPAREN);
    expect(LBRACE);
    block();
    expect(RBRACE);
}

void next_sym() {
    sym = tokens[cursor++];
}

void error(const char *s, ErrorType t) {
    printf("%s\n", s);
    exit(t);
}

b8 accept(Type t) {
    if(sym.t == t) {
        if(t == IDENTIFIER) {
            check_decl();
        }

        next_sym();
        return TRUE;
    }

    return FALSE;
}

void expect(Type t) {
    if(sym.t == t) {
        next_sym();
        return;
    }

    printf("Expected %s, got %s\n", tok_name[t], tok_name[sym.t]);
    error("Syntax error.", SYNTAX_ERROR);
}

void check_decl() {
    u64 size = vector_length(decl_list);
    for(u64 i = 0; i < size; i++) {
        if(strcmp(sym.s, decl_list[i]) == 0) {
            return;
        }
    }

    error("Identifier is used, but never declared.", SEMANTIC_ERROR);
}

b8 is_alpha(const char c) {
    return (64 < c && c < 91) || (96 < c && c < 123);
}

b8 is_digit(const char c) {
    return (47 < c && c < 58);
}

void trim_string(char *s) {
    u64 i = 0;
    do {
        if(s[i] == '\t' || s[i] == '\n') {
            s[i] = ' ';
        }
    } while(s[++i] != '\0');
    i = 0;

    while(s[i] != '\0') {
        const u64 offset = is_reserved(s, i) + 1;
        if(offset < 2) { i++; continue; }
        u64 k = strlen(s) + offset;
        while(i < k - offset) {
            s[k] = s[k-offset];
            k--;
        }

        s[i+offset] = ' ';
        if(offset == 3) {
            s[i+offset+1] = ' ';
            s[i+2] = s[i+1];
            s[i+1] = s[i];
        } else {
            s[i+1] = s[i];
        }
        s[i] = ' ';

        i += offset + 1;
    }
    i = 0;

    do {
        if(s[i] == ' ') {
            while(s[i + 1] == ' ') {
                u64 k = i + 1;
                while(s[k + 1] != '\0') {
                    s[k] = s[k + 1];
                    k++;
                }
                s[k] = '\0';
            }
        }
    } while(s[++i] != '\0');

    s[strlen(s)-1] = '\0';
}

u64 is_reserved(char *s, u64 pos) {
    if((59 < s[pos] && s[pos] < 63) && s[pos+1] == 61) { return 2; }

    if((39 < s[pos] && s[pos] < 48) || 
       (58 < s[pos] && s[pos] < 63) || 
       (122 < s[pos] && s[pos] < 126)) { return 1; }
    
    return 0;
}

Token *tokenize(char *s) {
    Token *tokens = vector_create(Token);

    char *t = strtok(s, " ");
    while(t) {
        Token tok;
        u64 len = strlen(t);
        tok.s = malloc(len + 1);
        strcpy(tok.s, t);
        tok.s[len] = '\0';
        tok.t = determine_token_type(tok.s);

        vector_push(tokens, tok);

        t = strtok(NULL, " ");
    }
    Token end = {0, END};
    vector_push(tokens, end);

    return tokens;
}

Type determine_token_type(const char *s) {
    if(strcmp(s, "main") == 0)       { return MAIN; }
    if(strcmp(s, "i32") == 0)        { return IDECL; }
    if(strcmp(s, "f32") == 0)        { return FDECL; }
    if(strcmp(s, "while") == 0)      { return WHILE; }
    if(strcmp(s, "if") == 0)      { return IF; }
    if(strcmp(s, "print") == 0)      { return PRINTCOMMAND; }
    if(strcmp(s, "return") == 0)      { return RETURN; }
    if(strcmp(s, "(") == 0)          { return LPAREN; }
    if(strcmp(s, ")") == 0)          { return RPAREN; }
    if(strcmp(s, "{") == 0)          { return LBRACE; }
    if(strcmp(s, "}") == 0)          { return RBRACE; }
    if(strcmp(s, ";") == 0)          { return SEMICOLON; }
    if(strcmp(s, "+") == 0)          { return ADD; }
    if(strcmp(s, "-") == 0)          { return SUB; }
    if(strcmp(s, "*") == 0)          { return MUL; }
    if(strcmp(s, "/") == 0)          { return DIV; }
    if(strcmp(s, ".") == 0)          { return DOT; }
    if(strcmp(s, "=") == 0)          { return EQUAL; }
    if(strcmp(s, "==") == 0)         { return EQCMP; }
    if(strcmp(s, "<") == 0)          { return LET; }
    if(strcmp(s, ">") == 0)          { return GRT; }
    if(strcmp(s, "<=") == 0)         { return LEE; }
    if(strcmp(s, ">=") == 0)         { return GRE; }

    if(is_alpha(s[0])) { return IDENTIFIER; }
    if(is_digit(s[0])) {
        u64 i = 0;
        while(s[++i] != '\0') { if(!is_digit(s[i])) { exit(SYNTAX_ERROR); } }
        return NUMBER;
    }

    return UNKNOWN;
}