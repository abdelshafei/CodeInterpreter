#ifndef SCANNER_H
#define SCANNER_H

#include <vector>
#include <iostream>
#include "Token.hpp"
#include <unordered_map>

class Scanner {
    public:
        Scanner(const string& src);
        bool isAtEnd();
        const char advance();
        void addToken(TokenType type);
        void addToken(TokenType type, string literal);
        void scanToken();
        void print();
        char peek();
        bool getErrStatus();
        int skipCommentIndex();
        vector<Token*>* getTokens();

        /* String literal */
        string getStringLiteral();

        /* Numerical literal */
        string getNumberLiteral();
        bool isDigit(char c);
        bool isDouble(const string&);
        string NormalizeDouble(const string&);

        /* Identifier tokens */
        bool isAlpha(char c);
        bool isAlphaNumeric(char c);
        void identifier();
        bool isTypeinKeyword(TokenType type);


    private:
        string src;
        vector<Token*> tokens;
        int start = 0;
        int current = 0;
        int line = 1;
        bool isError = false;

        const static unordered_map<string, TokenType> keywords;
};

#endif 