#include "Interpreter.hpp"

string Interpreter::TtoString(T value) const {
    visit([](const auto& val) -> string {
        if constexpr (is_same_v<decay_t<decltype(val)>, string>) {
            return val; 
        } else if constexpr (is_same_v<decay_t<decltype(val)>, bool>) {
            return to_string(val);
        } else if constexpr (is_same_v<decay_t<decltype(val)>, nullptr_t>) {
            return "nil";
        } else {
            return to_string(val); 
        }
    }, value);
}

bool Interpreter::isTruthy(T value) const {
    
}

string Interpreter::evaluate(Expr* expr) const {
    return expr->accept(*this);
}

string Interpreter::visitLiteralExpr(const Literal& expr) const {
    return TtoString(expr.value);
}


