#include <QLineSeries>
#include <QAreaSeries>

#include "left_rectangles.hpp"

#include "../../integral.hpp"
#include "../../utils.hpp"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    using namespace QtCharts;
#endif

bool LeftRectanglesTab::calculate(double a, double b, unsigned n, const std::string& expr, double& result)  {
    mChart->removeAllSeries();

    double x = 0.0;
    
    exprtk::expression<double> expression;
    if (!utils::compile_expression(expression, expr, x)) {
        return false;
    }

    double min, max;
    plot_function(a, b, expression, min, max);

    const double h = (b - a) / n;
    for (double k = a; k <= b; k += h) {
        x = k;
        QPointF p1(x, expression.value());
        x = k + h;
        QPointF p2(x, expression.value());

        QLineSeries* lowerSeries = new QLineSeries();
        lowerSeries->setUseOpenGL(true);
        lowerSeries->append(p1.x(), 0);
        lowerSeries->append(p2.x(), 0);

        QLineSeries* upperSeries = new QLineSeries();
        upperSeries->setUseOpenGL(true);
        upperSeries->append(p1);
        upperSeries->append(p2.x(), p1.y());

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

    return integral::rectangles_left(a, b, n, expression, result);
}
