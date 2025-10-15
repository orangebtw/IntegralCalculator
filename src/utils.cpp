#include "utils.hpp"

#include "exprtk.hpp"

bool utils::compile_expression(exprtk::expression<double>& expression, const std::string& expr, double& x) {
    exprtk::symbol_table<double> symbolTable;
    symbolTable.add_constants();
    symbolTable.add_variable("x", x);

    expression.register_symbol_table(symbolTable);

    exprtk::parser<double> parser;
    return parser.compile(expr, expression);
}