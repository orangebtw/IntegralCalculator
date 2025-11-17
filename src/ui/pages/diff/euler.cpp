#include "euler.hpp"

#include "../../../exprtk.hpp"
#include "../../../diff.hpp"

std::optional<EulerPage::CalculateResult> EulerPage::calculateWithFixedStep(double x0, double y0, int steps, char dependentVar, char independentVar, const std::string& expr) {
    exprtk::expression<double> expression;

    double x;
    double y;

    exprtk::symbol_table<double> symbolTable;
    symbolTable.add_constants();
    symbolTable.add_variable(&dependentVar, y);
    symbolTable.add_variable(&independentVar, x);

    expression.register_symbol_table(symbolTable);

    exprtk::parser<double> parser;
    if (!parser.compile(expr, expression)) {
        return std::unexpected("неправильная функция");
    }

    return diff::euler(x0, y0, 0.01, steps, x, y, expression);
}

std::optional<EulerPage::CalculateResult> EulerPage::calculateWithVarStep(const std::string& expr) {
    return std::unexpected("не реализовано");
}
