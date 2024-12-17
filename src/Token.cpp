#include "Token.hpp"
#include <map>
#include <sstream>
#include <typeinfo>
#include <cxxabi.h> // For demangling
#include <memory> 
#include <string_view>

Token::Token(TokenType type, const string& lexeme, const string& literal, int lineNo) 
    : type(type), lexeme(lexeme), literal(literal), lineNo(lineNo) {}

const string& Token::convertTokenTypeToStr(TokenType type) const {
    return TokenTypeNames[type];
}

const string Token::toString() const {
    ostringstream oss;

    oss << convertTokenTypeToStr(type) << " "
        << lexeme << " "
        << (literal.empty() == true ? "null" : literal);

    return oss.str();
}