#include "rungekutta.hpp"

#include "../../../exprtk.hpp"
#include "../../../diff.hpp"

RungeKuttaPage::CalculateResult RungeKuttaPage::calculateWithFixedStep(double x0, double y0, double end, int steps, char dependentVar, char independentVar, const std::string& expr) {
    exprtk::expression<double> expression;

    double x;
    double y;

    exprtk::symbol_table<double> symbolTable;
    symbolTable.add_constants();
    symbolTable.add_variable(std::string{dependentVar}, y);
    symbolTable.add_variable(std::string{independentVar}, x);

    expression.register_symbol_table(symbolTable);

    exprtk::parser<double> parser;
    if (!parser.compile(expr, expression)) {
        return std::unexpected("неправильная функция");
    }

    const double h = (end - x0) / steps;

    return Result {
        .data = diff::rungekutta(x0, y0, h, steps, x, y, expression)
    };
}

RungeKuttaPage::CalculateResult RungeKuttaPage::calculateWithVarStep(const std::string& expr) {
    return std::unexpected("не реализовано");
}
