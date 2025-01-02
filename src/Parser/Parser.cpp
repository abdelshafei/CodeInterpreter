#include "Parser.hpp"

Parser::Parser(vector<Token*>* tokens)
    : tokens(*tokens) {}

Parser::~Parser() { 
    for(Expr* expr : expressions) {
        delete expr;
    }

    if(Statements != nullptr) {
        for(Stmt* statement : *Statements) {
            delete statement;
        }

        delete Statements;
    }
}

/******************** Method Helpers ********************/
Token* Parser::peek() {
    return tokens.at(current);
}

Token* Parser::peekAfter() {
    return tokens.at(current+1);
}

bool Parser::isAtEnd() {
    return peek()->type == END_OF_FILE;
}

bool Parser::check(TokenType type) {
    return isAtEnd() ? false : (peek()->type == type);
}

Token* Parser::previous() {
    return tokens.at(current - 1);
}

Token* Parser::advance() {
    if(!isAtEnd()) ++current;
    return previous();
}

template<typename... TokenType>
bool Parser::match(TokenType... types) {
    for (auto type : {types...}) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

template<typename... TokenType>
bool Parser::matchTypes(TokenType... types) {
    for (auto type : {types...}) {
        if (check(type)) {
            return true;
        }
    }
    return false;
}

void Parser::synchronize() { // starts at the beginning of a line
    advance();

    while(!isAtEnd()) {
        if(previous()->type == SEMICOLON) return;

        switch(peek()->type) {
            case CLASS: return;
            case FUN: return;
            case VAR: return;
            case FOR: return;
            case IF: return;
            case WHILE: return;
            case PRINT: return;
            case RETURN: return;
            default: break;
        }
    }

    advance();
}

bool Parser::isDouble(const string& no) {
    for(char c : no) {
        if(c == '.') {
            return true;
        }
    }

    return false;
}

bool Parser::isBool(TokenType type) {
    return type == TRUE || type == FALSE;
}

/********************************************************/

/******************** Syntax Error Catcher ********************/
Token* Parser::consume(TokenType type, string msg) {
    if(check(type)) return advance();

    throw err(peek(), msg);
}

runtime_error Parser::err(Token* token, string msg) {
    if(token->type == END_OF_FILE) {
        return runtime_error("[Ln " + to_string(token->lineNo) + ", Col " + to_string(token->charNo)+"]" 
        + " at end " 
        + msg);
    } else {
        return runtime_error("[Ln " + to_string(token->lineNo) + ", Col " + to_string(token->charNo)+"]" 
        + " Error at '" 
        + token->lexeme 
        + "' " 
        + msg);
    }
}
/**************************************************************/

/******************** Expression Collecters *******************/
Expr* Parser::primary() {

    Expr* expr;

    if(match(FALSE)) {
        expr = new Literal(false);
        expressions.push_back(expr);
        return expr;
    } else if(match(TRUE)) {
        expr = new Literal(true);
        expressions.push_back(expr);
        return expr;
    } else if(match(NIL)) {
        expr = new Literal(nullptr);
        expressions.push_back(expr);
        return expr;
    } else if(match(STRING)) {
        expr = new Literal(previous()->literal);
        expressions.push_back(expr);
        return expr;
    } else if(match(NUMBER)) {
        if(isDouble(previous()->lexeme)) {
            expr = new Literal(stod(previous()->lexeme));
        } else {
            expr = new Literal(stoi(previous()->lexeme));
        }
        expressions.push_back(expr);
        return expr;
    } else if(match(IDENTIFIER)) {
        expr = new Variable(previous());
        expressions.push_back(expr);
    } else if(match(LEFT_PAREN)) {
        expr = expression(); // recursively calls all of the exprs in between the parenthesis
        consume(RIGHT_PAREN, "Expects ')' after expression");
        Expr* groupedExpr = new Grouping(expr);
        expressions.push_back(groupedExpr);
        return groupedExpr;
    } else if(matchTypes(BANG, PLUS, STAR, SLASH, MINUS, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL, BANG_EQUAL, EQUAL_EQUAL)) {
        if(peekAfter()->type != LEFT_PAREN) {
            if(peekAfter()->type == RIGHT_PAREN || peekAfter()->type == END_OF_FILE) {
                throw err(peek(), "Expects a number operand after expression");
            } else if(current == 0) {
                throw err(peek(), "Expects a number operand before expression");
            }
        }   
    }

    return nullptr;
}

Expr* Parser::unary() {

    if(peekAfter()->type == BANG) {
        throw err(peekAfter(), "not Expecting an operand before expression");
    }

    if(match(BANG, MINUS)) {
        Token* oprator = previous();
        Expr* right = unary();
        Expr* expr = new Unary(oprator, right);
        expressions.push_back(expr);
        return expr;
    }

    return primary();
}

Expr* Parser::factor() {
    Expr* expr = unary();

    while(primary() == nullptr && match(STAR, SLASH)) {
        Token* oprator = previous();
        Expr* right = unary();
        expr = new Binary(expr, oprator, right);
        expressions.push_back(expr);
    }

    return expr;
}

Expr* Parser::term() {
    Expr* expr = factor();

    
    while(primary() == nullptr && match(MINUS, PLUS)) {
        Token* oprator = previous();
        Expr* right = factor();
        expr = new Binary(expr, oprator, right);
        expressions.push_back(expr);
    }

    return expr;
}

Expr* Parser::comparison() {
    Expr* expr = term();

    while(primary() == nullptr && match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
        Token* oprator = previous();
        Expr* right = term();
        expr = new Binary(expr, oprator, right);
        expressions.push_back(expr);
    }

    return expr;
}

Expr* Parser::equality() {
    Expr* expr = comparison();


    while(primary() == nullptr && match(BANG_EQUAL, EQUAL_EQUAL)) {
        Token* oprator = previous();
        Expr* right = comparison();
        expr = new Binary(expr, oprator, right);
        expressions.push_back(expr);
    }

    return expr;
}

Expr* Parser::expression() { // For readabiliity
    return equality();
}


Expr* Parser::parseExpr() {
    try {
        return expression();
    } catch (const runtime_error& e) {
        throw e;
    }
}
/***************************************************************/

/******************** Statement Collecters *******************/
Stmt* Parser::expressionStmt() {
    Expr* expr = expression();
    consume(SEMICOLON, "Except ';' after value");
    return new Expression(expr);
}

Stmt* Parser::printStmt() {
    Expr* expr = expression();
    consume(SEMICOLON, "Except ';' after value");
    return new Print(expr);
}

Stmt* Parser::statement() {
    if(match(PRINT)) return printStmt();

    return expressionStmt();
}

Stmt* Parser::varDeclaration() {
    Token* name = consume(IDENTIFIER, "Expect variable name.");

    Expr* initializer = nullptr;
    if(match(EQUAL)) initializer = expression();

    consume(SEMICOLON, "Expect ';' after variable declaration.");
    return new Var(name, initializer);
}

Stmt* Parser::declaration() {
    try {
        if(match(VAR)) return varDeclaration();
        return statement();
    } catch (runtime_error& e) {
        synchronize();
        return nullptr;
    }
}

vector<Stmt*> Parser::parseStmt() {
    vector<Stmt*>* statements = new vector<Stmt*>;
    while(!isAtEnd()) {
        statements->push_back(declaration());
    }

    Statements = statements;

    return *statements;
}
/*************************************************************/