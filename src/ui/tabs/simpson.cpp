#include <QLineSeries>
#include <QAreaSeries>
#include <QSplineSeries>

#include "simpson.hpp"

#include "../../integral.hpp"

static double parabola(QPointF p1, QPointF p2, QPointF p3, float x) {
    return p1.y() * ((x - p2.x())*(x - p3.x()))/((p1.x() - p2.x())*(p1.x() - p3.x())) +
           p2.y() * ((x - p1.x())*(x - p3.x()))/((p2.x() - p1.x())*(p2.x() - p3.x())) +
           p3.y() * ((x - p1.x())*(x - p2.x()))/((p3.x() - p1.x())*(p3.x() - p2.x()));
}

bool SimpsonTab::calculate(double a, double b, unsigned n, const std::string& expr, double& result)  {
    mChart->removeAllSeries();

    double x = 0.0;
    
    exprtk::expression<double> expression;
    if (!integral::detail::compile_expression(expression, expr, x)) {
        return false;
    }

    qreal max = std::numeric_limits<qreal>::min();
    qreal min = 0;
    
    {
        QLineSeries* lineSeries = new QLineSeries();

        for (x = a; x <= b; x += 0.001) {
            const qreal value = expression.value();
            if (value > max)
                max = value;
            if (value < min)
                min = value;
            lineSeries->append(x, value);
        }

        mChart->addSeries(lineSeries);

        lineSeries->attachAxis(mAxisX);
        lineSeries->attachAxis(mAxisY);
    }
    
    const double h = (b - a) / (2*n);
    for (double k = a; k <= b; k += 2*h) {
        x = k;
        QPointF p1 = QPointF(x, expression.value());
        x = k + h;
        QPointF p2 = QPointF(x, expression.value());
        x = k + 2*h;
        QPointF p3 = QPointF(x, expression.value());

        QLineSeries* lowerSeries = new QLineSeries();
        QLineSeries* upperSeries = new QLineSeries();

        for (double t = 0.0; t < 1.0; t += 0.001) {
            const double x = k + 2*h*t;
            lowerSeries->append(x, min);
            upperSeries->append(x, parabola(p1, p2, p3, x));
        }

        QAreaSeries* areaSeries = new QAreaSeries();
        areaSeries->setLowerSeries(lowerSeries);
        areaSeries->setUpperSeries(upperSeries);
        areaSeries->setColor(mFillColor);
        areaSeries->setBorderColor(mBorderColor);

        mChart->addSeries(areaSeries);

        areaSeries->attachAxis(mAxisX);
        areaSeries->attachAxis(mAxisY);
    }

    mAxisX->setRange(a, b);
    mAxisY->setRange(min, max);

    return integral::simpson(a, b, n, expression, result);
}