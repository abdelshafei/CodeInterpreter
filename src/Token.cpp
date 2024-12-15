#include "Token.hpp"
#include <sstream>
#include <typeinfo>
#include <cxxabi.h>
#include <memory> 

Token::Token(TokenType type, const string& lexeme, const any& literal, int lineNo) 
    : type(type), lexeme(lexeme), literal(literal), lineNo(lineNo) {}

const string& Token::toString() const {
    ostringstream oss;
    
    int status;
    const char* mangledName = literal.type().name();
    std::unique_ptr<char, void(*)(void*)> demangledName(
        abi::__cxa_demangle(mangledName, nullptr, nullptr, &status), std::free);

    oss << type
        << lexeme
        << (status == 0 ? demangledName.get() : mangledName);

    return oss.str();
}