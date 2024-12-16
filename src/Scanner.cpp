#include "Scanner.hpp"

Scanner::Scanner(const string& src) 
    : src(src) {}

bool Scanner::isAtEnd() { return current >= src.size(); }
const char Scanner::advance() { return src.at(current++); }

void Scanner::addToken(TokenType type) {
    addToken(type, nullptr);
}

void Scanner::addToken(TokenType type, any literal) {
    string text = src.substr(start, current);
    tokens.push_back(new Token(type, text, literal, line));
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
      case '(': addToken(LEFT_PAREN); break;
      case ')': addToken(RIGHT_PAREN); break;
      case '{': addToken(LEFT_BRACE); break;
      case '}': addToken(RIGHT_BRACE); break;
      case ',': addToken(COMMA); break;
      case '.': addToken(DOT); break;
      case '-': addToken(MINUS); break;
      case '+': addToken(PLUS); break;
      case ';': addToken(SEMICOLON); break;
      case '*': addToken(STAR); break; 
    }
}

void Scanner::print() {
    for(Token* token : tokens) {
        cout << token->toString() << endl;
    }
}