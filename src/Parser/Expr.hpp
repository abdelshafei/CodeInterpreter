#ifndef EXPR_H
#define EXPR_H

#include "../Scanner/Token.hpp"

class Expr {
    public:
        class Visitor { //Interface
            public:
                // virtual string visitAssignExpr(const class Assign& expr) const = 0;
                virtual string visitBinaryExpr(const class Binary& expr) const = 0;
                // virtual string visitCallExpr(const class Call& expr) const = 0;
                // virtual string visitGetExpr(const class Get& expr) const = 0;
                virtual string visitGroupingExpr(const class Grouping& expr) const = 0;
                virtual string visitLiteralExpr(const class Literal& expr) const = 0;
                // virtual string visitLogicalExpr(const class Logical& expr) const = 0;
                // virtual string visitSetExpr(const class Set& expr) const = 0;
                // virtual string visitSuperExpr(const class Super& expr) const = 0;
                // virtual string visitThisExpr(const class This& expr) const = 0;
                virtual string visitUnaryExpr(const class Unary& expr) const = 0;
                // virtual string visitVariableExpr(const class Variable& expr) const = 0;
                virtual ~Visitor() = default; 
        };

        virtual string accept(const Visitor& visitor) const;

        virtual ~Expr() = default;
};

class Binary : public Expr {
    public:
        Binary(Expr* left, Token* oprator, Expr* right)
            : left(left), oprator(oprator), right(right) {}
        ~Binary() override = default;

        string accept(const Visitor& visitor) const override {
            return visitor.visitBinaryExpr(*this);
        }

        Expr* left;
        Token* oprator;
        Expr* right; 
};

class Grouping : public Expr {
    public:
        Grouping(Expr* expression)
            : expression(expression) {}
        ~Grouping() override = default;

        string accept(const Visitor& visitor) const override {
            return visitor.visitGroupingExpr(*this);
        }

        Expr* expression; 
};

class Literal : public Expr {
    public:
        using T = std::variant<int, double, std::string, bool>;
        Literal(T value)
            : value(value) {}
        Literal(nullptr_t value) 
            : value("nil") {}
        ~Literal() override = default;
        
        string accept(const Visitor& visitor) const override {
            return visitor.visitLiteralExpr(*this);
        }

        T value;
};

class Unary : public Expr { //Unary operators: ++, -- etc.
    public: 
        Unary(Token* oprator, Expr* right) 
            : oprator(oprator), right(right) {}
        ~Unary() override = default;

        string accept(const Visitor& visitor) const override {
            return visitor.visitUnaryExpr(*this);
        }

        Token* oprator;
        Expr* right;
};

#endif