#ifndef DIFF_HPP_
#define DIFF_HPP_

#include "exprtk.hpp"

namespace diff {

using Result = std::tuple<double, double, double>;

inline std::vector<Result> euler(double x0, double y0, double h, int n, double& x, double& y, exprtk::expression<double>& expression) {
    x = x0;
    y = y0;

    std::vector<Result> data;
    data.reserve(n);

    while (n > 0) {
        data.push_back({x, y, 0.0});

        y += expression.value() * h;
        x += h;

        --n;
    }

    data.push_back({x, y, 0.0});

    return data;
}

inline std::vector<Result> rungekutta(double x0, double y0, double h, int n, double& x, double& y, exprtk::expression<double>& expression) {
    x = x0;
    y = y0;

    std::vector<Result> data;
    data.reserve(n);

    double sx;
    double sy;

    while (n > 0) {
        data.push_back({x, y, 0.0});

        sx = x;
        sy = y;

        double k1 = expression.value();

        const double h_half = h / 2.0;

        x = sx + h_half;
        y = sy + h_half * k1;
        double k2 = expression.value();

        x = sx + h_half;
        y = sy + h_half * k2;
        double k3 = expression.value();

        x = sx + h;
        y = sy + h * k3;
        double k4 = expression.value();

        x = sx + h;
        y = sy + h * (k1 + 2.0*k2 + 2.0*k3 + k4) / 6.0;

        --n;
    }

    data.push_back({x, y, 0.0});

    return data;
}

inline std::vector<std::vector<double>> euler2(double h, int n, double& x, std::vector<double>& vars, const std::vector<exprtk::expression<double>>& exprs) {
    std::vector<std::vector<double>> data;
    data.reserve(n);

    std::vector<double> outVars;
    outVars.reserve(vars.size() + 1);

    outVars.push_back(x);
    for (auto var : vars) {
        outVars.push_back(var);
    }

    data.push_back(outVars);

    const uint32_t size = vars.size();

    while (n > 0) {
        for (uint32_t i = 0; i < size; ++i) {
            vars[i] = vars[i] + h * exprs[i].value();
        }
        x = x + h;

        std::vector<double> outVars;
        outVars.reserve(vars.size() + 1);

        outVars.push_back(x);
        for (auto var : vars) {
            outVars.push_back(var);
        }
        
        data.push_back(outVars);
        --n;
    }

    return data;
}

inline std::vector<Result> rungekutta2(double x0, double y0, double dy0, double h, int n, double& x, double& u, double& v, exprtk::expression<double>& expr1, exprtk::expression<double>& expr2) {
    x = x0;
    u = y0;
    v = dy0;

    std::vector<Result> data;
    data.reserve(n);
    
    data.push_back({x, u, v});

    double sx;
    double su;
    double sv;

    while (n > 0) {
        sx = x;
        su = u;
        sv = v;

        double k1_u = expr1.value();
        double k1_v = expr2.value();

        x = sx + h / 2.0;
        u = su + h / 2.0 * k1_u;
        v = sv + h / 2.0 * k1_v;
        double k2_u = expr1.value();
        double k2_v = expr2.value();

        x = sx + h / 2.0;
        u = su + h / 2.0 * k2_u;
        v = sv + h / 2.0 * k2_v;
        double k3_u = expr1.value();
        double k3_v = expr2.value();

        x = sx + h;
        u = su + h * k3_u;
        v = sv + h * k3_v;
        double k4_u = expr1.value();
        double k4_v = expr2.value();
        
        u = su + h * (k1_u + 2.0*k2_u + 2.0*k3_u + k4_u) / 6.0;
        v = sv + h * (k1_v + 2.0*k2_v + 2.0*k3_v + k4_v) / 6.0;
        x = sx + h;

        data.push_back({x, u, v});
        --n;
    }

    return data;
}

}

#endif 
