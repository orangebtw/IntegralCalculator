#include "rungekutta.hpp"

#include "../../../exprtk.hpp"
#include "../../../diff.hpp"

DiffMethodPageBase::CalculateResult RungeKuttaPage::calculate(double x0, double end, int steps, char independentVar, const std::vector<double>& startValues, const std::vector<char>& dependentVars, const std::vector<std::string>& exprs) {
    const uint32_t n = exprs.size();

    std::vector<exprtk::expression<double>> expressions(n);

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

    return diff::rungekutta2(h, steps, x, vars, expressions);
}