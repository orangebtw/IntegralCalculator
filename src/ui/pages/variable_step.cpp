#include <QLineSeries>
#include <QAreaSeries>

#include "variable_step.hpp"

#include "../../integral.hpp"
#include "../../utils.hpp"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    using namespace QtCharts;
#endif

std::optional<VariableStepPage::CalculateResult> VariableStepPage::calculate(double a, double b, double epsilon, const std::string& expr) {
    mChart->removeAllSeries();

    double x = 0.0;
    
    exprtk::expression<double> expression;
    if (!utils::compile_expression(expression, expr, x)) {
        return std::nullopt;
    }

    double min, max;
    plot_function(a, b, expression, min, max);

    setup_axis_lines(a, b, min, max);

    mAxisX->setRange(a, b);
    mAxisY->setRange(min, max);

    int n;
    double result = integral::rectangles_left_variable(a, b, epsilon, expression, n);

    return CalculateResult {
        .value = result,
        .steps = n
    };
}
