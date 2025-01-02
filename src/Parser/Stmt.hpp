#ifndef STMT_H
#define STMT_H

#include "Expr.hpp"


class Stmt {
    public:
        class Visitor { // void Interface
            public:
                virtual void visitPrintStmt(const class Print& stmt) = 0;
                virtual void visitExpressionStmt(const class Expression& stmt) = 0;
                virtual void visitVarStmt(const class Var& stmt) = 0;
                virtual ~Visitor() = default; 
        };

        virtual void accept(Visitor& visitor) const {
            throw runtime_error("accept called on base class Stmt");
        }
        virtual ~Stmt() = default;

    private:
};

class Print : public Stmt {
    public: 
        Print(Expr* value)
            : value(value) {}
        virtual ~Print() = default;

        void accept(Visitor& visitor) const override {
            return visitor.visitPrintStmt(*this);
        }

        Expr* value;
};

class Expression : public Stmt {
    public:
        Expression(Expr* value)
            : value(value) {}
        virtual ~Expression() = default;

        void accept(Visitor& visitor) const override {
            return visitor.visitExpressionStmt(*this);
        }

        Expr* value;
};

class Var : public Stmt {
    public:
        Var(Token* name, Expr* intitializer)
            : name(name), intitializer(intitializer) {}
        ~Var() = default;

        void accept(Visitor& visitor) const override {
            return visitor.visitVarStmt(*this);
        }

        Token* name;
        Expr* intitializer;

};

#endif