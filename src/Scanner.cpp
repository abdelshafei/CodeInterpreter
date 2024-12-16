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
    } else if(type == EQUAL_EQUAL || type == BANG_EQUAL || type == LESS_EQUAL || type == GREATER_EQUAL) {
        text = src.at(current-1);
        text += src.at(current);
        ++current;
    } else {
        text = src.at(current-1);
    }
    tokens.push_back(new Token(type, text, literal, line));
}

int Scanner::getNewLineIndex() {
    int index = current;

    for(int i = current; i < src.size(); i++) {
        if(src.at(i) == '\n') {
            index = i;
            break;
        }
    }

    return index;
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
      case '\n': line++; break;
      case ' ': break;//space
      case '\t': break;//tab
      case '\r': break;//enter
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
      case '=': if(src.size() != current && src.at(current) == '=') addToken(EQUAL_EQUAL);
                else                                                addToken(EQUAL);
                break;
      case '!': if(src.size() != current && src.at(current) == '=') addToken(BANG_EQUAL);
                else                                                addToken(BANG);
                break;
      case '<': if(src.size() != current && src.at(current) == '=') addToken(LESS_EQUAL);
                else                                                addToken(LESS);
                break;
      case '>': if(src.size() != current && src.at(current) == '=') addToken(GREATER_EQUAL);
                else                                                addToken(GREATER);
                break;
      case '/': if(src.size() != current && src.at(current) == '/') current = getNewLineIndex() - 1;
                else                                                addToken(SLASH); 
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