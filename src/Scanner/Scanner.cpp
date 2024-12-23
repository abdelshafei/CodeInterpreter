#include "Scanner.hpp"

const unordered_map<string, TokenType> Scanner::keywords = 
{
    {"and", AND},
    {"class", CLASS},
    {"else", ELSE},
    {"false", FALSE},
    {"fun", FUN},
    {"for", FOR},
    {"if", IF},
    {"nil", NIL},
    {"or", OR},
    {"print", PRINT},
    {"return", RETURN},
    {"super", SUPER},
    {"this", THIS},
    {"true", TRUE},
    {"var", VAR},
    {"while", WHILE}
};

Scanner::Scanner(const string& src) : src(src) 
{
    if(isAtEnd()) addToken(END_OF_FILE);

    while(!isAtEnd()) {
        scanToken(); 
        if(isAtEnd()) 
            addToken(END_OF_FILE);
    }
}

vector<Token*>* Scanner::getTokens() { return &tokens; }

string Scanner::getStringLiteral() { 
    string stringBuilder = "";
    charNo++;
    for(int i = current; i < src.size(); i++ && ++charNo) {
        if(src.at(i) != '"' && i == src.size()-1) {
          isError = true;
          current = i+1;
          break;
        }

        if(src.at(i) == '"') {
            current = i+1;
            break;
        }
        stringBuilder += src.at(i);
    }

    return stringBuilder;
}

string Scanner::getNumberLiteral() {
    string stringBuilder = "";

    for(int i = current-1; i < src.size(); i++ && charNo++) {
        
        if(!isDigit(src.at(i)) && !(src.at(i) == '.' && (i+1) < src.size() && isDigit(src.at(i+1)))) {
            current = i;
            break;
        }

        stringBuilder += src.at(i);
        if(i == src.size() - 1) current = i + 1;
    }

    return stringBuilder;    
}

bool Scanner::isDouble(const string& txt) {
    char prevChar = '\0';

    for(const char& c : txt) {
        if(prevChar == '.' && isDigit(c)) {
            return true;
        }
        prevChar = c;
    }

    return false;
}

string Scanner::NormalizeDouble(const string& txt) {
    string strBuilder = "";

    bool atDot = false;
    bool isAllZeros = false;
    bool isDecimal = false;

    for(const char& c : txt) {
        if(c == '.') {
            atDot = true;
        }

        if(!atDot) {
            strBuilder += c;
        } else {
            if(c == '0') isAllZeros = true;
            else if(isDigit(c) && c != '0')  {
                isAllZeros = false;
                isDecimal = true;
            }

            if(!isAllZeros) {
              strBuilder += c;  
            }
        }
    }

    if(isAllZeros && !isDecimal) strBuilder += "0";

    return strBuilder;

}

bool Scanner::isAlpha(char c) { return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'); }

bool Scanner::isAlphaNumeric(char c) { return isAlpha(c) || isDigit(c); }

void Scanner::identifier() {
    while(isAlphaNumeric(peek())) advance();

    string txt = src.substr(start, (current-start));
    TokenType type = keywords.count(txt) ? keywords.at(txt) : IDENTIFIER;
    addToken(type);
}

bool Scanner::isTypeinKeyword(TokenType type) {
    for(const auto& pair : keywords) {
        if(type == pair.second) return true;
    }

    return false;
}

bool Scanner::isDigit(char c) { return c >= '0' && c <= '9'; }
bool Scanner::getErrStatus() { return isError; }
bool Scanner::isAtEnd() { return current >= src.size(); }
const char Scanner::advance() { return src.at(current++); }
char Scanner::peek() { 
    if (isAtEnd()) return '\0';
    return src.at(current);
}

void Scanner::addToken(TokenType type) {
    addToken(type, "");
}

void Scanner::addToken(TokenType type, string literal) {
    string text;
    if(type == END_OF_FILE) {
        text = "";
    } else if(type == EQUAL_EQUAL || type == BANG_EQUAL || type == LESS_EQUAL || type == GREATER_EQUAL) {
        text = src.at(current-1);
        text += src.at(current);
        ++current;
    } else if(type == STRING) {
        if(getErrStatus() == true) {
            cerr << "[Ln " << line << ", Col " << charNo << "]"
                << " Error: Unterminated string." 
                << endl;
            return;
        }
        text = "\"";
        text += any_cast<string>(literal);
        text += "\"";
    } else if(type == NUMBER) {
        text = any_cast<string>(literal);
        if(!isDouble(literal)) literal += ".0";
        else literal = NormalizeDouble(literal);
    } else if(type == IDENTIFIER) {
        text = src.substr(start, (current - start));
    } else if(isTypeinKeyword(type)) {
        text = src.substr(start, (current - start));
    } else {
        text = src.at(current-1);
    }
    tokens.push_back(new Token(type, text, literal, line, charNo));
}

int Scanner::skipCommentIndex() {
    int index = current;

    for(int i = current; i < src.size(); i++) {
        if(src.at(i) == '\n') {
            index = i;
            break;
        }

        if(i == src.size()-1) {
            index = src.size();
            break;
        }
    }

    return index;
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
      case '\n': line++; charNo = 0; break;
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
      case '/': if(src.size() != current && src.at(current) == '/') current = skipCommentIndex();
                else                                                addToken(SLASH); 
                break;
      case '"': addToken(STRING, getStringLiteral()); break;
      default:  
        if(isDigit(c)) {
            addToken(NUMBER, getNumberLiteral()); 
            break;
        } else if(isAlpha(c)) {
            start = current - 1;
            identifier();
        } else {
            cerr << "[Ln " << line << ", Col " << charNo << "]"
                << " Error: Unexpected character: "
                << c
                << endl; 
            isError = true; 
            break;
        }
    }
}

void Scanner::print() {
    for(Token* token : tokens) {
        cout << token->toString() << endl;
    }
}