#include <QLineSeries>
#include <QAreaSeries>

#include "right_rectangles.hpp"

#include "../../../integral.hpp"
#include "../../../utils.hpp"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    using namespace QtCharts;
#endif

std::optional<RightRectanglesPage::CalculateResult> RightRectanglesPage::calculateWithFixedStep(double a, double b, unsigned n, const std::string& expr)  {
    mChart->removeAllSeries();

    double x = 0.0;
    
    exprtk::expression<double> expression;
    if (!utils::compile_expression(expression, expr, x)) {
        return std::nullopt;
    }

    plot_function(a, b, expression);

    const double h = (b - a) / n;
    for (double k = b; k >= a + h; k -= h) {
        x = k;
        QPointF p1(x, expression.value());
        x = k - h;
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

    double result = integral::rectangles_right(a, b, n, expression);
    return CalculateResult {
        .value = result
    };
}

std::optional<RightRectanglesPage::CalculateResult> RightRectanglesPage::calculateWithVarStep(double a, double b, double eps, const std::string& expr)  {
    mChart->removeAllSeries();

    double x = 0.0;
    
    exprtk::expression<double> expression;
    if (!utils::compile_expression(expression, expr, x)) {
        return std::nullopt;
    }

    plot_function(a, b, expression);

    int n;
    double result = integral::rectangles_right_variable(a, b, eps, expression, n);
    return CalculateResult {
        .value = result,
        .steps = n
    };
}