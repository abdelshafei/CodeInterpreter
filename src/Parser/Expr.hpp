#ifndef EXPR_H
#define EXPR_H

#include "../Scanner/Token.hpp"

class Expr {
    public:
        using T = std::variant<int, double, std::string, bool, nullptr_t>;
        class Visitor { //Interface
            public:
                // virtual T visitAssignExpr(const class Assign& expr) const = 0;
                virtual T visitBinaryExpr(const class Binary& expr) const = 0;
                // virtual T visitCallExpr(const class Call& expr) const = 0;
                // virtual T visitGetExpr(const class Get& expr) const = 0;
                virtual T visitGroupingExpr(const class Grouping& expr) const = 0;
                virtual T visitLiteralExpr(const class Literal& expr) const = 0;
                // virtual T visitLogicalExpr(const class Logical& expr) const = 0;
                // virtual T visitSetExpr(const class Set& expr) const = 0;
                // virtual T visitSuperExpr(const class Super& expr) const = 0;
                // virtual T visitThisExpr(const class This& expr) const = 0;
                virtual T visitUnaryExpr(const class Unary& expr) const = 0;
                // virtual T visitVariableExpr(const class Variable& expr) const = 0;
                virtual ~Visitor() = default; 
        };

        virtual string accept(const Visitor& visitor) const {
            throw runtime_error("accept called on base class Expr");
        }

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
        using T = std::variant<int, double, string, bool, nullptr_t>;
        Literal(T value)
            : value(value) {}
        ~Literal() override = default;
        
        string accept(const Visitor& visitor) const override {
            return visitor.visitLiteralExpr(*this);
        }

        T value;
};

class Unary : public Expr { 
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