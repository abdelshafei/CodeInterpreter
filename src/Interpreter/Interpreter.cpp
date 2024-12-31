#include "Interpreter.hpp"
bool Interpreter::isTruthy(T value) const {
    return visit([](const auto& val) -> bool {
        if constexpr (is_same_v<decay_t<decltype(val)>, bool>) {
            return !val;
        } else if constexpr (is_same_v<decay_t<decltype(val)>, nullptr_t>) {
            return true;
        } 
        
        return false;

    }, value);
}

string Interpreter::NormalizeDouble(const string& txt) const {
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

string Interpreter::to_string(const T& value) const {
    if (holds_alternative<double>(value)) {
        return NormalizeDouble(std::to_string(get<double>(value)));
    } else if(holds_alternative<int>(value)) {
        return std::to_string(get<int>(value));
    } else if (holds_alternative<bool>(value)) {
        return get<bool>(value) ? "true" : "false";
    } else if (holds_alternative<string>(value)) {
        return get<string>(value);
    } else if (holds_alternative<nullptr_t>(value)) {
        return "nil";
    } else {
        throw runtime_error("wrong Type of T detected");
    } 
}

T Interpreter::Product(T left, T right) const {
    if(holds_alternative<double>(left) && holds_alternative<double>(right)) {
        return get<double>(left) * get<double>(right);
    } else if(holds_alternative<int>(left) && holds_alternative<int>(right)) {
        return get<int>(left) * get<int>(right);
    } else if(holds_alternative<double>(left) && holds_alternative<int>(right)) {
        return get<double>(left) * get<int>(right);
    } else if(holds_alternative<int>(left) && holds_alternative<double>(right)) {
        return get<int>(left) * get<double>(right);
    } else {
        throw runtime_error("Operands must be numbers.");
    }
}

T Interpreter::Quotient(T left, T right) const {
    if(holds_alternative<double>(left) && holds_alternative<double>(right)) {
        if(get<double>(right) == 0) {
            throw runtime_error("Division by zero.");
        }
        return get<double>(left) / get<double>(right);
    } else if(holds_alternative<int>(left) && holds_alternative<int>(right)) {
        if(get<int>(right) == 0) {
            throw runtime_error("Division by zero.");
        } else if(get<int>(left) % get<int>(right) == 0) {
            return get<int>(left) / get<int>(right);
        } else {
            return static_cast<double>(get<int>(left)) / static_cast<double>(get<int>(right));
        }
    } else if(holds_alternative<double>(left) && holds_alternative<int>(right)) {
        if(get<int>(right) == 0) {
            throw runtime_error("Division by zero.");
        }
        return get<double>(left) / get<int>(right);
    } else if(holds_alternative<int>(left) && holds_alternative<double>(right)) {
        if(get<double>(right) == 0) {
            throw runtime_error("Division by zero.");
        }
        return get<int>(left) / get<double>(right);
    } else {
        throw runtime_error("Operands must be numbers.");
    }
}

T Interpreter::Difference(T left, T right) const {
    if(holds_alternative<double>(left) && holds_alternative<double>(right)) {
        return get<double>(left) - get<double>(right);
    } else if(holds_alternative<int>(left) && holds_alternative<int>(right)) {
        return get<int>(left) - get<int>(right);
    } else {
        throw runtime_error("Operands must be numbers.");
    }
}

T Interpreter::Sum(T left, T right) const {
    if(holds_alternative<double>(left) && holds_alternative<double>(right)) {
        return get<double>(left) + get<double>(right);
    } else if(holds_alternative<int>(left) && holds_alternative<int>(right)) {
        return get<int>(left) + get<int>(right);
    } else if(holds_alternative<string>(left) && holds_alternative<string>(right)) {
        return get<string>(left) + get<string>(right);
    } else {
        throw runtime_error("Operands must be two numbers or two strings");
    }
}

bool Interpreter::isLogical(T lValue, T rValue, TokenType relational) const {
    if(relational == GREATER) { 

        if(holds_alternative<double>(lValue) && holds_alternative<double>(rValue)) {
            return get<double>(lValue) > get<double>(rValue);
        } else if(holds_alternative<int>(lValue) && holds_alternative<int>(rValue)) {
            return get<int>(lValue) > get<int>(rValue);
        }

        throw runtime_error("Operands must be numbers.");

    } else if(relational == GREATER_EQUAL) {

        if(holds_alternative<double>(lValue) && holds_alternative<double>(rValue)) {
            return get<double>(lValue) >= get<double>(rValue);
        } else if(holds_alternative<int>(lValue) && holds_alternative<int>(rValue)) {
            return get<int>(lValue) >= get<int>(rValue);
        }

        throw runtime_error("Operands must be numbers.");

    } else if(relational == LESS) {

        if(holds_alternative<double>(lValue) && holds_alternative<double>(rValue)) {
            return get<double>(lValue) < get<double>(rValue);
        } else if(holds_alternative<int>(lValue) && holds_alternative<int>(rValue)) {
            return get<int>(lValue) < get<int>(rValue);
        }

        throw runtime_error("Operands must be numbers.");  

    } else if(relational == LESS_EQUAL) {

        if(holds_alternative<double>(lValue) && holds_alternative<double>(rValue)) {
            return get<double>(lValue) <= get<double>(rValue);
        } else if(holds_alternative<int>(lValue) && holds_alternative<int>(rValue)) {
            return get<int>(lValue) <= get<int>(rValue);
        }

        throw runtime_error("Operands must be numbers.");

    } else if(relational == BANG_EQUAL) {

        if(holds_alternative<double>(lValue) && holds_alternative<double>(rValue)) {
            return get<double>(lValue) != get<double>(rValue);
        } else if(holds_alternative<int>(lValue) && holds_alternative<int>(rValue)) {
            return get<int>(lValue) != get<int>(rValue);
        } else if(holds_alternative<bool>(lValue) && holds_alternative<bool>(rValue)) {
            return get<bool>(lValue) != get<bool>(rValue);
        } else if(holds_alternative<string>(lValue) && holds_alternative<string>(rValue)) {
            return get<string>(lValue) != get<string>(rValue);
        } else if((holds_alternative<int>(lValue) && holds_alternative<string>(rValue)) || (holds_alternative<string>(lValue) && holds_alternative<int>(rValue))) {
            return true;
        } else if((holds_alternative<double>(lValue) && holds_alternative<string>(rValue)) || (holds_alternative<string>(lValue) && holds_alternative<double>(rValue))) {
            return true;
        }

        throw runtime_error("Operands must be numbers.");      

    } else if(relational == EQUAL_EQUAL) {
        if(holds_alternative<double>(lValue) && holds_alternative<double>(rValue)) {
            return get<double>(lValue) == get<double>(rValue);
        } else if(holds_alternative<int>(lValue) && holds_alternative<int>(rValue)) {
            return get<int>(lValue) == get<int>(rValue);
        } else if(holds_alternative<bool>(lValue) && holds_alternative<bool>(rValue)) {
            return get<bool>(lValue) == get<bool>(rValue);
        } else if(holds_alternative<string>(lValue) && holds_alternative<string>(rValue)) {
            return get<string>(lValue) == get<string>(rValue);
        } else if((holds_alternative<int>(lValue) && holds_alternative<string>(rValue)) || (holds_alternative<string>(lValue) && holds_alternative<int>(rValue))) {
            return false;
        } else if((holds_alternative<double>(lValue) && holds_alternative<string>(rValue)) || (holds_alternative<string>(lValue) && holds_alternative<double>(rValue))) {
            return false;
        }

        throw runtime_error("Operands must be numbers or strings.");     
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------------

T Interpreter::evaluate(const Expr& expr) {
    if (auto binary = dynamic_cast<const Binary*>(&expr)) {
        return evalBinaryExpr(*binary);
    } else if (auto grouping = dynamic_cast<const Grouping*>(&expr)) {
        return evalGroupingExpr(*grouping);
    } else if (auto literal = dynamic_cast<const Literal*>(&expr)) {
        return evalLiteralExpr(*literal);
    } else if (auto unary = dynamic_cast<const Unary*>(&expr)) {
        return evalUnaryExpr(*unary);
    } else {
        throw std::runtime_error("Unsupported expression type.");
    }
}

T Interpreter::evalLiteralExpr(const Literal& expr) {
    return expr.value;
}

T Interpreter::evalGroupingExpr(const Grouping& expr) {
    return evaluate(*expr.expression);
}

T Interpreter::evalUnaryExpr(const Unary& expr) {
    T right = evaluate(*expr.right);

    switch(expr.oprator->type) {
        case MINUS:
            if(holds_alternative<double>(right)) {
                return -get<double>(right);
            } else if(holds_alternative<int>(right)){
                return -get<int>(right);
            }
            throw runtime_error("Operand must be a number.");
        case BANG:
            return isTruthy(right);
        default:
            return static_cast<nullptr_t>(nullptr);
    }
}

T Interpreter::evalBinaryExpr(const Binary& expr) {

    T left = evaluate(*expr.left);
    T right = evaluate(*expr.right);

    try {
        switch (expr.oprator->type)
        {
            case MINUS: 
                return Difference(left, right);
            case PLUS:
                return Sum(left, right);
            case STAR:
                return Product(left, right);
            case SLASH:
                return Quotient(left, right);
            case GREATER:
                return isLogical(left, right, GREATER);
            case GREATER_EQUAL:
                return isLogical(left, right, GREATER_EQUAL);
            case LESS:
                return isLogical(left, right, LESS);
            case LESS_EQUAL:
                return isLogical(left, right, LESS_EQUAL);
            case BANG_EQUAL:
                return isLogical(left, right, BANG_EQUAL);
            case EQUAL_EQUAL:
                return isLogical(left, right, EQUAL_EQUAL);
            default:
                return static_cast<nullptr_t>(nullptr);
        }
    } catch(runtime_error& err) {
        throw err;
    }
}

//-----------------------------------------------------------------------------------------------------

void Interpreter::visitPrintStmt(const Print& stmt) {
    T value = evaluate(*stmt.value);
    cout << to_string(value)<< endl;
    return;
}

void Interpreter::visitExpressionStmt(const Expression& stmt) {
    evaluate(*stmt.value);
    return;
}

void Interpreter::excute(const Stmt& statement) {
    statement.accept(*this);
}

void Interpreter::interpret(vector<Stmt*> statements) {
    for(Stmt* statement : statements) {
        excute(*statement);
    }
}



