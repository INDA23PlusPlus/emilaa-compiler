#include "parser.hpp"

void Parser::error(const char *s) {
    printf("%s\n", s);
    found_error = true;
}

bool Parser::is_alpha(const char c) {
    const std::string alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    return alpha.find(c) != std::string::npos;
}

bool Parser::is_digit(const char c) {
    const std::string alpha = "0123456789";
    return alpha.find(c) != std::string::npos;
}

Symbol Parser::determine_symbol(const std::string &s) {
    std::string us = s;
    for(auto &c : us) { c = std::toupper(c); }
    if(us == "WHILE") { return WHILE; }
    if(us == "PRINT") { return PRINTCOMMAND; }
    if(us == "(") { return LPAREN; }
    if(us == ")") { return RPAREN; }
    if(us == "{") { return LBRACE; }
    if(us == "}") { return RBRACE; }
    if(us == ";") { return SEMICOLON; }
    if(us == "+") { return ADD; }
    if(us == "-") { return SUB; }
    if(us == "*") { return MUL; }
    if(us == "/") { return DIV; }
    if(us == "=") { return EQUAL; }
    if(us == "==") { return EQCMP; }
    if(us == "<") { return LET; }
    if(us == ">") { return GRT; }
    if(us == "<=") { return LEE; }
    if(us == ">=") { return GRE; }
    
    if(is_alpha(us[0])) { return IDENTIFIER; }
    if(is_digit(us[0])) {
        for(const auto &c : us) { if(!is_digit(c)) { error("Expected integer."); } }
        return NUMBER;
    }

    return END;
}

void Parser::next_sym() {
    sym = input.at(cursor++);
}

void Parser::write_token() {
    for(size_t i = 0; i < indent; i++) { output += '\t'; }
    if(sym.symbol == IDENTIFIER || sym.symbol == NUMBER) {
        output += sym.text + '\n';
    } else {
        output += sym_names[sym.symbol] + '\n';
    }
}

bool Parser::accept(Symbol s) {
    if(s == sym.symbol) {
        write_token();
        next_sym();
        return true;
    }
    return false;
}

bool Parser::expect(Symbol s) {
    if(s == sym.symbol) {
        write_token();
        next_sym();
        return true;
    }
    error("Unexcpected symbol.");
    return false;
}

void Parser::factor() {
    if(accept(IDENTIFIER)) {}
    else if(accept(NUMBER)) {}
    else if(accept(LPAREN)) {
        expression();
        expect(RPAREN);
    } else {
        error("Syntax error.");
        next_sym();
    }
}

void Parser::term() {
    factor();
    while(sym.symbol == MUL || sym.symbol == DIV) {
        next_sym();
        term();
    }
}

void Parser::expression() {
    term();
    while(sym.symbol == ADD || sym.symbol == SUB) {
        next_sym();
        term();
    }
}

void Parser::condition() {
    expression();
    if(sym.symbol == EQCMP ||
       sym.symbol == LET   ||
       sym.symbol == LEE   ||
       sym.symbol == GRT   ||
       sym.symbol == GRE
    ) {
        next_sym();
        expression();
    } else {
        error("Invalid conditional operator.");
        next_sym();
    }
}

void Parser::statement() {
    if(accept(IDENTIFIER)) {
        expect(EQUAL);
        expression();
        expect(SEMICOLON);
    } else if(accept(WHILE)) {
        expect(LPAREN);
        condition();
        expect(RPAREN);
        expect(LBRACE);
        block();
        expect(RBRACE);
    } else if(accept(PRINTCOMMAND)) {
        expect(LPAREN);
        expect(IDENTIFIER);
        expect(RPAREN);
        expect(SEMICOLON);
    }
}

void Parser::block() {
    statement();
    if(sym.symbol != END && sym.symbol != RBRACE) {
        block();
    }
}

bool Parser::parse() {
    next_sym();
    block();
    if(found_error) {
        printf("Compile errors were found...\n");
        return false;
    }
    printf("Code was accepted.\n\n%s", output.c_str());
    return true;
}