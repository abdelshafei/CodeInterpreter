#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <any>
#include <iostream>
#include "TokenType.hpp"

using namespace std;

class Token {
    public:
        Token(TokenType type, const string& lexeme, const string& literal, int lineNo);
        const string toString() const;
        const string& convertTokenTypeToStr(TokenType type) const;
    
    private:
        TokenType type;
        string lexeme;
        string literal;
        int lineNo;
};

#endif