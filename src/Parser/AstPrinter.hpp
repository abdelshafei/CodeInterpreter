#ifndef ASTPRINTER_H
#define ASTPRINTER_H

#include "Expr.hpp"

class AstPrinter : public Expr::Visitor {
    public:
        string print(Expr* expression) const;
        string visitBinaryExpr(const Binary& expr) const override;
        string visitGroupingExpr(const Grouping& expr) const override;
        string visitLiteralExpr(const Literal& expr) const override;
        string visitUnaryExpr(const Unary& expr) const override;
        string visitAssignExpr(const class Assign& expr) const override;
        string visitVariableExpr(const class Variable& expr) const override;
        template<typename... Expr>
        string parenthesize(const string& name, Expr*... exprs) const;

        string NormalizeDouble(const string& txt) const;
       

};

#endif