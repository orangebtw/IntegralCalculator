#ifndef APPROX_HPP_
#define APPROX_HPP_

#include <array>
#include <cstdlib>

namespace approx {

inline double e(double x) {
    std::array<double, 8> A;
    A[0] = 1.0;

    double k = 1.0;
    for (std::size_t i = 1; i < A.size(); ++i) {
        k *= i;
        A[i] = 1.0 / k;
    }

    double S = 0.0;
    for (std::size_t i = 0; i < A.size(); ++i) {
        S += A[i] * x;
        x *= x;
    }

    return S;
}

inline double sin(double x) {
    std::array<double, 4> A;
    A[0] = 1.0;

    double q = -1.0;
    double k = 1.0;
    for (std::size_t i = 1; i < A.size(); ++i) {
        double w = 2.0 * i;
        k *= w * (w + 1.0);
        A[i] = q * 1.0 / (k );
        q *= -1.0;
    }

    double S = 0.0;
    for (std::size_t i = 0; i < A.size(); ++i) {
        S += A[i] * x;
        x *= x*x;
    }

    return S;
}

inline double sqrt(double x, double y0, double eps) {
    double y = y0;
    double prev_y;

    do {
        prev_y = y;
        y = (y + x/y) / 2.0;
    } while (std::abs(y - prev_y) > eps);

    return y;
}

inline double inv_sqrt(double x, double y0, double eps) {
    double y = y0;
    double prev_y;

    do {
        prev_y = y;
        y = y / 2.0 * (3.0 - x * y*y);
    } while (std::abs(y - prev_y) > eps);

    return y;
}

}

#endif