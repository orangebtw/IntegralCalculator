#ifndef INTEGRAL_HPP_ 
#define INTEGRAL_HPP_ 

#include <string>

#include "exprtk.hpp"

namespace integral {

inline double rectangles_left(double a, double b, unsigned n, exprtk::expression<double>& expression) {
    double& x = expression.get_symbol_table().get_variable("x")->ref();

    const double h = (b - a) / n;
    double result = 0.0f;
    for (x = a; x <= b - h; x += h) {
        result += expression.value();
    }
    result *= h;
    return result;
}

inline double rectangles_right(double a, double b, unsigned n, exprtk::expression<double>& expression) {
    double& x = expression.get_symbol_table().get_variable("x")->ref();

    const double h = (b - a) / n;
    double result = 0.0f;
    for (x = a + h; x <= b; x += h) {
        result += expression.value();
    }
    result *= h;
    return result;
}

inline double trapezoid(double a, double b, unsigned n, exprtk::expression<double>& expression) {
    double& x = expression.get_symbol_table().get_variable("x")->ref();

    x = a;
    const double a2 = expression.value();

    x = b;
    const double b2 = expression.value();

    const double h = (b - a) / n;
    double result = (a2 + b2) / 2.0;
    for (x = a + h; x <= b - h; x += h) {
        result += expression.value();
    }
    result *= h;
    return result;
}

inline double simpson(double a, double b, unsigned n, exprtk::expression<double>& expression) {
    double& x = expression.get_symbol_table().get_variable("x")->ref();

    const double h = (b - a) / (2 * n);
    const double h2 = 2 * h;
    double odd = 0.0;
    for (x = a + h; x <= b - h; x += h2) {
        odd += expression.value();
    }
    double even = 0.0;
    for (x = a + h2; x <= b - h2; x += h2) {
        even += expression.value();
    }

    x = a;
    const double a2 = expression.value();

    x = b;
    const double b2 = expression.value();

    return (h / 3.0) * (a2 + b2 + 4.0 * odd + 2.0 * even);
}

inline double rectangles_left_variable(double a, double b, double eps, exprtk::expression<double>& expression, int& n) {
    n = 2;

    double i1 = 0.0;
    double i2 = rectangles_left(a, b, n, expression);

    while (fabs(i1 - i2) > eps) {
        n *= 2;

        i1 = i2;
        i2 = rectangles_left(a, b, n, expression);
    }

    return i2;
}

inline double rectangles_left_variable2(double a, double b, double eps, exprtk::expression<double>& expression, int& n) {
    double& x = expression.get_symbol_table().get_variable("x")->ref();

    double fa = 1.0 / (2.0 + a);
    double fb = 1.0 / (2.0 + b);

    n = 2;

    double hv = (b - a) / n;
    x = a + hv;
    double s2 = expression.value();
    double s1 = 0.0;
    double i2 = hv * (s2 + (fa + fb) / 2.0);

    double r = INFINITY;

    double i1 = i2;
    double hs = hv / 2.0;
    double hd = hv;

    while (r > eps) {
        s1 += s2;
        s2 = 0.0;
        i1 = i2;
        hs = hv / 2.0;
        hd = hv;
        hv = hs;
        x = a + hs;
        while (x <= (b - hv)) {
            s2 += expression.value();
            x += hd;
        }
        i2 = hv * (s2 + s1 + ((fa + fb) / 2.0));
        r = fabs(i1 - i2);
        n += n;
    }

    return i2;
}

inline double rectangles_right_variable(double a, double b, double eps, exprtk::expression<double>& expression, int& n) {
    n = 2;

    double i1 = 0.0;
    double i2 = rectangles_right(a, b, n, expression);

    while (fabs(i1 - i2) > eps) {
        n *= 2;

        i1 = i2;
        i2 = rectangles_right(a, b, n, expression);
    }

    return i2;
}

inline double trapezoid_variable(double a, double b, double eps, exprtk::expression<double>& expression, int& n) {
    n = 2;

    double i1 = 0.0;
    double i2 = trapezoid(a, b, n, expression);

    while (fabs(i1 - i2) > eps) {
        n *= 2;

        i1 = i2;
        i2 = trapezoid(a, b, n, expression);
    }

    return i2;
}

inline double simpson_variable(double a, double b, double eps, exprtk::expression<double>& expression, int& n) {
    n = 2;

    double i1 = 0.0;
    double i2 = simpson(a, b, n, expression);

    while (fabs(i1 - i2) > eps) {
        n *= 2;

        i1 = i2;
        i2 = simpson(a, b, n, expression);
    }

    return i2;
}

}

#endif 
