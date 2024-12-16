#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <any>
#include <iostream>

enum TokenType {
  // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  /// One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  /// Literals.
  IDENTIFIER, STRING, NUMBER,

  /// Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE, 
  
  /// null.
  END_OF_FILE
};

using namespace std;

class Token {
    public:
        Token(TokenType type, const string& lexeme, const any& literal, int lineNo);
        const string toString() const;
    
    private:
        TokenType type;
        string lexeme;
        any literal;
        int lineNo;
};

#endif
