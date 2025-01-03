#include "AstPrinter.hpp"

#include <sstream>

string AstPrinter::print(Expr* expression) const {
    return expression->accept(*this);
}

string AstPrinter::NormalizeDouble(const string& txt) const {
    string strBuilder = "";

    bool atDot = false;
    bool isAllZeros = false;
    bool isDecimal = false;

    for(const char& c : txt) {
        if(c == '.') {
            atDot = true;
        }

        if(!atDot) {
            strBuilder += c;
        } else {
            if(c == '0') isAllZeros = true;
            else if((c >= '0' && c <= '9') && c != '0')  {
                isAllZeros = false;
                isDecimal = true;
            }

            if(!isAllZeros) {
              strBuilder += c;  
            }
        }
    }

    if(isAllZeros && !isDecimal) strBuilder += "0";

    return strBuilder;
}

string AstPrinter::visitBinaryExpr(const Binary& expr) const {
    return parenthesize(expr.oprator->lexeme, expr.left, expr.right);
}

string AstPrinter::visitGroupingExpr(const Grouping& expr) const {
    return parenthesize("group", expr.expression);
}

string AstPrinter::visitLiteralExpr(const Literal& expr) const {
    return visit([&](const auto& val) -> string {
        if constexpr (is_same_v<decay_t<decltype(val)>, string>) {
            return val; 
        } else if constexpr (is_same_v<decay_t<decltype(val)>, bool>) {
            return val ? "true" : "false";
        } else if constexpr (is_same_v<decay_t<decltype(val)>, nullptr_t>) {
            return "nil";
        } else if constexpr (is_same_v<decay_t<decltype(val)>, double>) {
            return this->NormalizeDouble(to_string(val)); 
        } else {
            return to_string(val) + ".0";
        }
    }, expr.value);
}

string AstPrinter::visitUnaryExpr(const Unary& expr) const {
    return parenthesize(expr.oprator->lexeme, expr.right);
}

string AstPrinter::visitAssignExpr(const class Assign& expr) const {
    return parenthesize(expr.name->lexeme, expr.value);
}

string AstPrinter::visitVariableExpr(const class Variable& expr) const {
    return expr.name->literal;
}

template<typename... Expr>
string AstPrinter::parenthesize(const string& name, Expr*... exprs) const {
    string strBuilder;

    strBuilder += "(";
    strBuilder += name;

    ((strBuilder += " " + exprs->accept(*this)), ...);

    strBuilder += ")";

    return strBuilder;
}