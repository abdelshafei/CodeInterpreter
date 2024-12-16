#include "Token.hpp"
#include <map>
#include <sstream>
#include <typeinfo>
#include <cxxabi.h> // For demangling
#include <memory> 
#include <string_view>

Token::Token(TokenType type, const string& lexeme, const any& literal, int lineNo) 
    : type(type), lexeme(lexeme), literal(literal), lineNo(lineNo) {}

const string& Token::convertTokenTypeToStr(TokenType type) const {
    return TokenTypeNames[type];
}

const string Token::toString() const {
    ostringstream oss;
    
    int status;
    const char* mangledName = literal.type().name();
    std::unique_ptr<char, void(*)(void*)> demangledName(
        abi::__cxa_demangle(mangledName, nullptr, nullptr, &status), std::free);

    map<TokenType, string_view> types;

    oss << convertTokenTypeToStr(type) << " "
        << lexeme << " "
        << (status == 0 ? "null" : mangledName);

    return oss.str();
}