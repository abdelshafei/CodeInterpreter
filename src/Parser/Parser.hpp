#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <iostream>
#include "Expr.hpp"
#include <iomanip>

class Parser {
    public:
        Parser(vector<Token*>* tokens); 

        void cleanUpExpressions();

        // Method Helpers
        template<typename... TokenType>
        bool match(TokenType... types);
        template<typename... TokenType>
        bool matchTypes(TokenType... types);
        bool check(TokenType type);
        Token* advance();
        bool isAtEnd();
        Token* peek();
        Token* peekAfter();
        Token* previous();
        void synchronize();

        // Expression Collectors
        Expr* expression();
        Expr* equality();
        Expr* comparison();
        Expr* term();
        Expr* factor();
        Expr* unary();
        Expr* primary();
        Expr* parse();

        // Syntax Error Catcher
        Token* consume(TokenType type, string msg);
        runtime_error err(Token* token, string msg);

        vector<Expr*>* getExprs() { return &expressions; }
    private:
        vector<Token*> tokens;
        vector<Expr*> expressions; // to free them at the end after printing
        int current = 0;
};

#endif 