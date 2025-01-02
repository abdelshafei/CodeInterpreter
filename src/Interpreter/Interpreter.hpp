#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../Parser/Expr.hpp"
#include "../Parser/Stmt.hpp"
#include <vector>
#include "../Env/Enviroment.hpp"

class Interpreter : public Stmt::Visitor {
    public:
        void visitPrintStmt(const Print& stmt) override;
        void visitExpressionStmt(const Expression& stmt) override;
        void visitVarStmt(const Var& stmt) override;

        T evaluate (const Expr& expr) ;
        T evalBinaryExpr(const Binary& expr);
        T evalGroupingExpr(const Grouping& expr);
        T evalLiteralExpr(const Literal& expr);
        T evalUnaryExpr(const Unary& expr);
        T evalVariableExpr(const Variable& expr);

        bool isTruthy(T value) const;
        string to_string(const T& value) const;
        string NormalizeDouble(const string& txt) const;
        T Product(T lValue, T rValue) const;
        T Quotient(T lValue, T rValue) const;
        T Difference(T lValue, T rValue) const;
        T Sum(T lValue, T rValue) const;
        bool isLogical(T lValue, T rValue, TokenType relational) const;

        void interpret(vector<Stmt*> statements);
        void excute(const Stmt& statement);
    
    private:
        Enviroment* enviroment = new Enviroment();

};

#endif