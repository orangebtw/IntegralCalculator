#include <QLineSeries>
#include <QAreaSeries>
#include <QSplineSeries>

#include "simpson.hpp"

#include "../../integral.hpp"
#include "../../utils.hpp"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    using namespace QtCharts;
#endif

static double parabola(QPointF p1, QPointF p2, QPointF p3, float x) {
    return p1.y() * ((x - p2.x())*(x - p3.x()))/((p1.x() - p2.x())*(p1.x() - p3.x())) +
           p2.y() * ((x - p1.x())*(x - p3.x()))/((p2.x() - p1.x())*(p2.x() - p3.x())) +
           p3.y() * ((x - p1.x())*(x - p2.x()))/((p3.x() - p1.x())*(p3.x() - p2.x()));
}

std::optional<SimpsonPage::CalculateResult> SimpsonPage::calculateWithFixedStep(double a, double b, unsigned n, const std::string& expr) {
    mChart->removeAllSeries();

    double x = 0.0;
    
    exprtk::expression<double> expression;
    if (!utils::compile_expression(expression, expr, x)) {
        return std::nullopt;
    }

    plot_function(a, b, expression);
    
    const double h = (b - a) / (2*n);
    for (double k = a; k <= b; k += 2*h) {
        x = k;
        QPointF p1 = QPointF(x, expression.value());
        x = k + h;
        QPointF p2 = QPointF(x, expression.value());
        x = k + 2*h;
        QPointF p3 = QPointF(x, expression.value());

        QLineSeries* lowerSeries = new QLineSeries();
        lowerSeries->setUseOpenGL(true);

        QLineSeries* upperSeries = new QLineSeries();
        upperSeries->setUseOpenGL(true);

        for (double t = 0.0; t < 1.0; t += 0.001) {
            const double x = k + 2*h*t;
            const double value = parabola(p1, p2, p3, x);

            if (value >= 0) {
                lowerSeries->append(x, 0);
                upperSeries->append(x, value);
            } else {
                lowerSeries->append(x, value);
                upperSeries->append(x, 0);
            }
        }

        QAreaSeries* areaSeries = new QAreaSeries();
        areaSeries->setLowerSeries(lowerSeries);
        areaSeries->setUpperSeries(upperSeries);
        areaSeries->setColor(mFillColor);
        areaSeries->setBorderColor(mBorderColor);
        areaSeries->setUseOpenGL(true);

        mChart->addSeries(areaSeries);

        areaSeries->attachAxis(mAxisX);
        areaSeries->attachAxis(mAxisY);
    }

    double result = integral::simpson(a, b, n, expression);

    return CalculateResult {
        .value = result
    };
}


std::optional<SimpsonPage::CalculateResult> SimpsonPage::calculateWithVarStep(double a, double b, double eps, const std::string& expr) {
    mChart->removeAllSeries();

    double x = 0.0;
    
    exprtk::expression<double> expression;
    if (!utils::compile_expression(expression, expr, x)) {
        return std::nullopt;
    }

    plot_function(a, b, expression);

    int n;
    double result = integral::simpson_variable(a, b, eps, expression, n);

    return CalculateResult {
        .value = result,
        .steps = n
    };
}
