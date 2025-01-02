#include "Enviroment.hpp"

void Enviroment::insert(const string& name, T value) {
    vals.insert(make_pair(name, value));
}

T Enviroment::fetch(Token* name) {
    if(vals.contains(name->lexeme)) 
        return vals.find(name->lexeme)->second;
    
    throw runtime_error("Undefined variable '" + name->lexeme + "'.");
}