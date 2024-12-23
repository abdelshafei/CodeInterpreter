#include "Parser.hpp"

Parser::Parser(vector<Token*>* tokens)
    : tokens(*tokens) {}

void Parser::cleanUpExpressions() {
    for(Expr* expr : expressions) {
        delete expr;
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
        case CLASS:
        case FUN:
        case VAR:
        case FOR:
        case IF:
        case WHILE:
        case PRINT:
        case RETURN:
          return;
        }
    }

    advance();
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

/******************** Expression Collecters ********************/
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
    } else if(match(NUMBER, STRING)) {
        expr = new Literal(previous()->literal);
        expressions.push_back(expr);
        return expr;
    } else if(match(LEFT_PAREN)) {
        expr = expression(); // recursively calls all of the exprs in between the parenthesis
        consume(RIGHT_PAREN, "Expect ')' after expression");
        Expr* groupedExpr = new Grouping(expr);
        expressions.push_back(groupedExpr);
        return groupedExpr;
    } else if(matchTypes(MINUS, PLUS, SLASH, STAR)) {
        // consume(NUMBER, "Expect a number after expression");
        if((previous()->type != NUMBER && peekAfter()->type != NUMBER) || (previous()->type != STRING && peekAfter()->type != STRING)) {
            throw err(peekAfter(), "Expect a number after expression");
        }
    }

    return nullptr;
}

Expr* Parser::unary() {
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

    while(match(SLASH, STAR)) {
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

    while(match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
        Token* oprator = previous();
        Expr* right = term();
        expr = new Binary(expr, oprator, right);
        expressions.push_back(expr);
    }

    return expr;
}

Expr* Parser::equality() {
    Expr* expr = comparison();


    while(match(BANG_EQUAL, EQUAL_EQUAL)) {
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


Expr* Parser::parse() {
    try {
        return expression();
    } catch (const runtime_error& e) {
        throw e;
    }
}
/***************************************************************/