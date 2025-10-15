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
        lineSeries->setUseOpenGL(true);
        
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

    const double h = (b - a) / n;
    for (double k = b; k >= a + h; k -= h) {
        x = k;
        QPointF p1(x, expression.value());
        x = k - h;
        QPointF p2(x, expression.value());

        QLineSeries* lowerSeries = new QLineSeries();
        lowerSeries->append(p1.x(), 0);
        lowerSeries->append(p1.x(), 0);
        lowerSeries->setUseOpenGL(true);
        
        QLineSeries* upperSeries = new QLineSeries();
        upperSeries->append(p1);
        upperSeries->append(p2.x(), p1.y());
        upperSeries->setUseOpenGL(true);

        if (p2.y() < 0) {
            std::swap(lowerSeries, upperSeries);
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

    setup_axis_lines(a, b, min, max);

    mAxisX->setRange(a, b);
    mAxisY->setRange(min, max);

    return integral::rectangles_right(a, b, n, expression, result);
}