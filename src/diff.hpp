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

    std::vector<double> tempVars(size);

    while (n > 0) {
        for (uint32_t i = 0; i < size; ++i) {
            tempVars[i] = vars[i];
        }

        for (uint32_t i = 0; i < size; ++i) {
            tempVars[i] = tempVars[i] + h * exprs[i].value();
        }

        for (uint32_t i = 0; i < size; ++i) {
            vars[i] = tempVars[i];
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

inline std::vector<std::vector<double>> rungekutta2(double h, int n, double& x, std::vector<double>& vars, const std::vector<exprtk::expression<double>>& exprs) {
    std::vector<std::vector<double>> data;
    data.reserve(n);

    std::vector<double> outVars;
    outVars.reserve(n);
    outVars.push_back(x);
    for (double var : vars) {
        outVars.push_back(var);
    }
    
    data.push_back(outVars);

    const uint32_t length = vars.size();

    double tempX;
    std::vector<double> tempVars(length);
    std::vector<double> k1(length);
    std::vector<double> k2(length);
    std::vector<double> k3(length);
    std::vector<double> k4(length);

    while (n > 0) {        
        tempX = x;
        for (uint32_t i = 0; i < length; ++i) {
            tempVars[i] = vars[i];
        }


        for (uint32_t i = 0; i < length; ++i) {
            k1[i] = exprs[i].value();
        }


        x = tempX + h / 2.0;
        for (uint32_t i = 0; i < length; ++i) {
            vars[i] = tempVars[i] + h / 2.0 * k1[i];
        }
        for (uint32_t i = 0; i < length; ++i) {
            k2[i] = exprs[i].value();
        }

        x = tempX + h / 2.0;
        for (uint32_t i = 0; i < length; ++i) {
            vars[i] = tempVars[i] + h / 2.0 * k2[i];
        }
        for (uint32_t i = 0; i < length; ++i) {
            k3[i] = exprs[i].value();
        }

        x = tempX + h;
        for (uint32_t i = 0; i < length; ++i) {
            vars[i] = tempVars[i] + h * k3[i];
        }
        for (uint32_t i = 0; i < length; ++i) {
            k4[i] = exprs[i].value();
        }

        x = tempX + h;
        for (uint32_t i = 0; i < length; ++i) {
            vars[i] = tempVars[i] + h * (k1[i] + 2.0*k2[i] + 2.0*k3[i] + k4[i]) / 6.0;
        }

        std::vector<double> outVars;
        outVars.reserve(n);
        outVars.push_back(x);
        for (double var : vars) {
            outVars.push_back(var);
        }
        
        data.push_back(outVars);
        --n;
    }

    return data;
}

}

#endif 
