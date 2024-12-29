#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../Parser/Expr.hpp"

using Value = variant<bool, double, string>;

class Interpreter {
    public:
        T evaluate (const Expr& expr) ;
        T evalBinaryExpr(const Binary& expr);
        T evalGroupingExpr(const Grouping& expr);
        T evalLiteralExpr(const Literal& expr);
        T evalUnaryExpr(const Unary& expr);

        bool isTruthy(T value) const;
        string to_string(const T& value) const;
        string NormalizeDouble(const string& txt) const;
        T Difference(T lValue, T rValue) const;
        T Sum(T lValue, T rValue) const;
        T Product(T lValue, T rValue) const;
        T Quotient(T lValue, T rValue) const;
        bool isLogical(T lValue, T rValue, TokenType relational) const;
};

#endif