#ifndef EXPR_H
#define EXPR_H

#include "../Scanner/Token.hpp"

class Expr {
    public:
        class Visitor {
            public:
                // virtual string visitAssignExpr(const class Assign& expr) = 0;
                virtual string visitBinaryExpr(const class Binary& expr) const;
                // virtual string visitCallExpr(const class Call& expr) = 0;
                // virtual string visitGetExpr(const class Get& expr) = 0;
                virtual string visitGroupingExpr(const class Grouping& expr) const;
                virtual string visitLiteralExpr(const class Literal& expr) const;
                // virtual string visitLogicalExpr(const class Logical& expr) = 0;
                // virtual string visitSetExpr(const class Set& expr) = 0;
                // virtual string visitSuperExpr(const class Super& expr) = 0;
                // virtual string visitThisExpr(const class This& expr) = 0;
                virtual string visitUnaryExpr(const class Unary& expr) const;
                // virtual string visitVariableExpr(const class Variable& expr) = 0;
                virtual ~Visitor() = default; 
        };

        virtual string accept(Visitor& visitor) const;

        virtual ~Expr() = default;
};

class Binary : public Expr {
    public:
        Binary(Expr* left, Token oprator, Expr* right)
            : left(left), oprator(oprator), right(right) {}

        string accept(Visitor& visitor) const override {
            return visitor.visitBinaryExpr(*this);
        }

        Expr* left;
        Token oprator;
        Expr* right; 
};

class Grouping : public Expr {
    public:
        Grouping(Expr* expression)
            : expression(expression) {}

        string accept(Visitor& visitor) const override {
            return visitor.visitGroupingExpr(*this);
        }

        Expr* expression; 
};

class Literal : public Expr {
    public:
        using T = std::variant<int, double, std::string, bool>;
        Literal(T value)
            : value(value) {}
        
        string accept(Visitor& visitor) const override {
            return visitor.visitLiteralExpr(*this);
        }

        T value;
};

class Unary : public Expr { //Unary operators: ++, -- etc.
    public: 
        Unary(Token oprator, Expr* right) 
            : oprator(oprator), right(right) {}

        string accept(Visitor& visitor) const override {
            return visitor.visitUnaryExpr(*this);
        }

        Token oprator;
        Expr* right;
};

#endif