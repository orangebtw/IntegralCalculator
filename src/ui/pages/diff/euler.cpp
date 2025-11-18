#include "euler.hpp"

#include "../../../exprtk.hpp"
#include "../../../diff.hpp"

EulerPage::CalculateResult EulerPage::calculate(double x0, double y0, double end, int steps, char dependentVar, char independentVar, const std::string& expr) {
    exprtk::expression<double> expression;

    double x;
    double y;

    exprtk::symbol_table<double> symbolTable;
    symbolTable.add_constants();
    symbolTable.add_variable(std::string{independentVar}, x);
    symbolTable.add_variable(std::string{dependentVar}, y);

    expression.register_symbol_table(symbolTable);

    exprtk::parser<double> parser;
    if (!parser.compile(expr, expression)) {
        return std::unexpected("неправильная функция");
    }

    const double h = (end - x0) / steps;

    return diff::euler(x0, y0, h, steps, x, y, expression);
}

EulerPage::CalculateResult EulerPage::calculate2(double x0, double y0, double dy0, double end, int steps, char dependentVar1, char dependentVar2, char independentVar, const std::string &expr1, const std::string &expr2) {
    exprtk::expression<double> expression1;
    exprtk::expression<double> expression2;

    double x;
    double u;
    double v;

    exprtk::symbol_table<double> symbolTable;
    symbolTable.add_constants();
    symbolTable.add_variable(std::string{independentVar}, x);
    symbolTable.add_variable(std::string{dependentVar1}, u);
    symbolTable.add_variable(std::string{dependentVar2}, v);

    expression1.register_symbol_table(symbolTable);
    expression2.register_symbol_table(symbolTable);

    exprtk::parser<double> parser;
    if (!parser.compile(expr1, expression1)) {
        return std::unexpected("неправильная функция");
    }

    if (!parser.compile(expr2, expression2)) {
        return std::unexpected("неправильная функция");
    }

    const double h = (end - x0) / steps;

    return diff::euler2(x0, y0, dy0, h, steps, x, u, v, expression1, expression2);
}