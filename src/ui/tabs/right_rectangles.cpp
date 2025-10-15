#include <QLineSeries>
#include <QAreaSeries>

#include "right_rectangles.hpp"

#include "../../integral.hpp"

bool RightRectanglesTab::calculate(double a, double b, unsigned n, const std::string& expr, double& result)  {
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

    x = b;
    QPointF prevPoint(x, expression.value());

    const double h = (b - a) / n;
    for (x = b; x >= a + h; x -= h) {
        QPointF point(x, expression.value());

        QLineSeries* lowerSeries = new QLineSeries();
        lowerSeries->append(prevPoint.x(), min);
        lowerSeries->append(point.x(), min);
        
        QLineSeries* upperSeries = new QLineSeries();
        upperSeries->append(prevPoint.x(), prevPoint.y());
        upperSeries->append(point.x(), prevPoint.y());

        QAreaSeries* areaSeries = new QAreaSeries();
        areaSeries->setLowerSeries(lowerSeries);
        areaSeries->setUpperSeries(upperSeries);
        areaSeries->setColor(mFillColor);
        areaSeries->setBorderColor(mBorderColor);

        mChart->addSeries(areaSeries);

        areaSeries->attachAxis(mAxisX);
        areaSeries->attachAxis(mAxisY);

        prevPoint = point;
    }

    mAxisX->setRange(a, b);
    mAxisY->setRange(min, max);

    return integral::rectangles_right(a, b, n, expression, result);
}