#include "euler.hpp"

#include "../../../exprtk.hpp"
#include "../../../diff.hpp"

std::expected<std::vector<diff::Result>, const char*> EulerPage::calculate(double x0, double y0, double end, int steps, char dependentVar, char independentVar, const std::string& expr) {
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

EulerPage::CalculateResult EulerPage::calculate2(double x0, double end, int steps, char independentVar, const std::vector<double>& startValues, const std::vector<char>& dependentVars, const std::vector<std::string>& exprs) {
    const uint32_t n = exprs.size();

    std::vector<exprtk::expression<double>> expressions;
    expressions.reserve(n);

    std::vector<double> vars = startValues;

    double x = x0;

    exprtk::symbol_table<double> symbolTable;
    symbolTable.add_constants();
    symbolTable.add_variable(std::string{independentVar}, x);

    for (uint32_t i = 0; i < n; ++i) {
        symbolTable.add_variable(std::string{dependentVars[i]}, vars[i]);
    }

    exprtk::parser<double> parser;
    for (uint32_t i = 0; i < n; ++i) {
        expressions[i].register_symbol_table(symbolTable);

        if (!parser.compile(exprs[i], expressions[i])) {
            return std::unexpected("неправильная функция");
        }
    }

    const double h = (end - x0) / steps;

    return diff::euler2(h, steps, x, vars, expressions);
}