#ifndef DIFF_HPP_
#define DIFF_HPP_

#include "exprtk.hpp"

namespace diff {

inline double euler(double x0, double y0, double h, int n, double& x, double& y, exprtk::expression<double>& expression) {
    x = x0;
    y = y0;

    while (n > 0) {
        y += expression.value() * h;
        x += h;

        --n;
    }

    return y;
}

inline double rungekutta(double x0, double y0, double h, int n, double& x, double& y, exprtk::expression<double>& expression) {
    x = x0;
    y = y0;

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

    return y;
}

}

#endif 
