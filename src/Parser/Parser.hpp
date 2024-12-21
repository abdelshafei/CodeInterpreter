#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <iostream>
#include "Tokenization/Token.hpp"

class Parser {
    public:
        Parser(vector<Token*>* tokens); 

    private:
        vector<Token*> tokens;
        int current = 0;
};

#endif 