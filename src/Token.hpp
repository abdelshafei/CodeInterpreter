#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <any>
#include <iostream>
#include "TokenType.hpp"

using namespace std;

class Token {
    public:
        Token(TokenType type, const string& lexeme, const any& literal, int lineNo);
        const string toString() const;
    
    private:
        TokenType type;
        string lexeme;
        any literal;
        int lineNo;
};

#endif
