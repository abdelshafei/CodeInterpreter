#ifndef ENVIROMENT_H
#define ENVIROMENT_H


#include <unordered_map>
#include "../Scanner/Token.hpp"

class Enviroment {
    public:
        void insert(const string& name, T value);
        T fetch(Token* name);
        // once its defined it cant be undefined unless the program ends.

    private:
        std::unordered_map<string, T> vals;
};

#endif