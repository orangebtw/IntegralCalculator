#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <string>

namespace exprtk {
    template <typename T>
    class expression;
}

namespace utils {
    bool compile_expression(exprtk::expression<double>& expression, const std::string& expr, double& x);
}

#endif