#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <iostream>
#include "Expr.hpp"
#include <iomanip>
#include "../Env/Stmt.hpp"

class Parser {
    public:
        Parser(vector<Token*>* tokens); 
        ~Parser();

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
        bool isDouble(const string& no);
        bool isBool(TokenType type);

        // Expression Collectors
        Expr* expression();
        Expr* equality();
        Expr* comparison();
        Expr* term();
        Expr* factor();
        Expr* unary();
        Expr* primary();
        Expr* parseExpr(); // Used for evaluation

        // Statement Collectors
        Stmt* expressionStmt();
        Stmt* printStmt();
        Stmt* statement();
        vector<Stmt*> parseStmt(); // Used for when running the program

        // Syntax Error Catcher
        Token* consume(TokenType type, string msg);
        runtime_error err(Token* token, string msg);

        vector<Expr*>* getExprs() { return &expressions; }
    private:
        vector<Token*> tokens;
        vector<Expr*> expressions; // to free them at the end after printing
        vector<Stmt*>* Statements; // to free them at the end of the program

        int current = 0;
};

#endif 