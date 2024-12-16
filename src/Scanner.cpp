#include "Scanner.hpp"

Scanner::Scanner(const string& src) : src(src) 
{
    while(!isAtEnd()) {
        scanToken(); 
        if(isAtEnd()) 
            addToken(END_OF_FILE);
    }
}

bool Scanner::getErrStatus() { return isError; }
bool Scanner::isAtEnd() { return current >= src.size(); }
const char Scanner::advance() { return src.at(current++); }

void Scanner::addToken(TokenType type) {
    addToken(type, nullptr);
}

void Scanner::addToken(TokenType type, any literal) {
    string text;
    if(type == END_OF_FILE) {
        text = "";
    } else {
        text = src.at(current-1);
    }
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
      case '/': addToken(SLASH); break;
      case '=': if(src.at(current) == '=' && src.size() != current) addToken(EQUAL_EQUAL);
                else                       addToken(EQUAL);
                break;
      case '!': if(src.at(current) == '=' && src.size() != current) addToken(BANG_EQUAL);
                else                       addToken(BANG);
                break;
      case '<': if(src.at(current) == '=' && src.size() != current) addToken(LESS_EQUAL);
                else                       addToken(LESS);
                break;
      case '>': if(src.at(current) == '=' && src.size() != current) addToken(GREATER_EQUAL);
                else                       addToken(GREATER);
                break;
      default:  cerr << "[line " << line << "]"
                << " Error: Unexpected character: "
                << c
                << endl; 
                isError = true; 
                break;
    }
}

void Scanner::print() {
    for(Token* token : tokens) {
        cout << token->toString() << endl;
    }
}