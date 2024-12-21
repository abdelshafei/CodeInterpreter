#ifndef ASTPRINTER_H
#define ASTPRINTER_H

#include "Expr.hpp"

class AstPrinter : public Expr::Visitor {
    public:
        string print(Expr expression);
        string visitBinaryExpr(const Binary& expr) override;
        string visitGroupingExpr(const Grouping& expr) override;
        string visitLiteralExpr(const Literal& expr) override;
        string visitUnaryExpr(const Unary& expr) override;
        template<typename... Expr>
        string parenthesize(const string& name, const Expr... exprs);
};

#endif