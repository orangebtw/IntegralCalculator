#ifndef INTEGRAL_HPP_ 
#define INTEGRAL_HPP_ 

#include <string>

#include "exprtk.hpp"

namespace integral {

namespace detail {
    inline bool compile_expression(exprtk::expression<double>& expression, const std::string& expr, double& x) {
        exprtk::symbol_table<double> symbolTable;
        symbolTable.add_constants();
        symbolTable.add_variable("x", x);

        expression.register_symbol_table(symbolTable);

        exprtk::parser<double> parser;
        return parser.compile(expr, expression);
    }
}

inline bool rectangles_left(double a, double b, unsigned n, exprtk::expression<double>& expression, double& result) {
    double& x = expression.get_symbol_table().get_variable("x")->ref();

    const double h = (b - a) / n;
    result = 0.0f;
    for (x = a; x <= b - h; x += h) {
        result += expression.value();
    }
    result *= h;

    return true;
}

inline bool rectangles_right(double a, double b, unsigned n, exprtk::expression<double>& expression, double& result) {
    double& x = expression.get_symbol_table().get_variable("x")->ref();

    const double h = (b - a) / n;
    result = 0.0f;
    for (x = a + h; x <= b; x += h) {
        result += expression.value();
    }
    result *= h;

    return true;
}

inline bool trapezoid(double a, double b, unsigned n, exprtk::expression<double>& expression, double& result) {
    double& x = expression.get_symbol_table().get_variable("x")->ref();

    x = a;
    const double a2 = expression.value();

    x = b;
    const double b2 = expression.value();

    const double h = (b - a) / n;
    result = (a2 + b2) / 2.0;
    for (x = a + h; x <= b - h; x += h) {
        result += expression.value();
    }
    result *= h;

    return true;
}

inline bool simpson(double a, double b, unsigned n, exprtk::expression<double>& expression, double& result) {
    double& x = expression.get_symbol_table().get_variable("x")->ref();

    const double h = (b - a) / (2 * n);
    const double h2 = 2 * h;
    double odd = 0.0;
    for (x = a + h; x <= b - h; x += h2) {
        odd += expression.value();
    }
    double even = 0.0;
    for (x = a + h2; x <= b - h2; x += h2) {
        even += expression.value();
    }

    x = a;
    const double a2 = expression.value();

    x = b;
    const double b2 = expression.value();

    result = (h / 3.0) * (a2 + 4 * odd + 2 * even + b2);
    return true;
}

}

#endif 
