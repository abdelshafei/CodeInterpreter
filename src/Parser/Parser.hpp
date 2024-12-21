#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <iostream>
#include "Expr.hpp"

class Parser {
    public:
        Parser(vector<Token*>* tokens); 

    private:
        vector<Token*> tokens;
        int current = 0;
};

#endif 