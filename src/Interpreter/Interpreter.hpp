#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../Parser/Expr.hpp"

class Interpreter : public Expr::Visitor {
    public:
        string evaluate (Expr* expr) const;
        string visitBinaryExpr(const Binary& expr) const override;
        string visitGroupingExpr(const Grouping& expr) const override;
        string visitLiteralExpr(const Literal& expr) const override;
        string visitUnaryExpr(const Unary& expr) const override;
        bool isTruthy(T value) const;

        string TtoString(T value) const;
};

#endif