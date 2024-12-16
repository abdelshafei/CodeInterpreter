#ifndef SCANNER_H
#define SCANNER_H

#include <vector>
#include "Token.hpp"

class Scanner {
    public:
        Scanner(const string& src);
        bool isAtEnd();
        const char advance();
        void addToken(TokenType type);
        void addToken(TokenType type, any literal);
        void scanToken();

    private:
        string src;
        vector<Token*> tokens;
        int start = 0;
        int current = 0;
        int line = 1;
};

#endif 