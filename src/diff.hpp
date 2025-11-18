#ifndef DIFF_HPP_
#define DIFF_HPP_

#include "exprtk.hpp"

namespace diff {

inline std::vector<std::pair<double, double>> euler(double x0, double y0, double h, int n, double& x, double& y, exprtk::expression<double>& expression) {
    x = x0;
    y = y0;

    std::vector<std::pair<double, double>> data;
    data.reserve(n);

    while (n > 0) {
        data.push_back({x ,y});

        y += expression.value() * h;
        x += h;

        --n;
    }

    data.push_back({x, y});

    return data;
}

inline std::vector<std::pair<double, double>> rungekutta(double x0, double y0, double h, int n, double& x, double& y, exprtk::expression<double>& expression) {
    x = x0;
    y = y0;

    std::vector<std::pair<double, double>> data;
    data.reserve(n);

    double sx;
    double sy;

    while (n > 0) {
        sx = x;
        sy = y;

        double k1 = expression.value();

        x = sx + h / 2.0;
        y = sy + k1 / 2.0;
        double k2 = expression.value();

        x = sx + h / 2.0;
        y = sy + k2 / 2.0;
        double k3 = expression.value();

        x = sx + h;
        y = sy + h * k3;
        double k4 = expression.value();

        x = sx;
        y = sy;

        y += h * (k1 + 2.0*k2 + 2.0*k3 + k4) / 6.0;
        x += h;

        --n;
    }

    data.push_back({x, y});

    return data;
}

}

#endif 
