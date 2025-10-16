#ifndef UI_TABS_BASE_TAB_HPP_
#define UI_TABS_BASE_TAB_HPP_

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QChart>
#include <QValueAxis>
#include <QLineSeries>
#include <QBarSeries>
#include <QChartView>

namespace exprtk {
    template <typename T>
    class expression;
}

class MethodTab : public QWidget {
    Q_OBJECT
public:
    MethodTab(const QString& title, QWidget* parent = nullptr) : QWidget(parent) {
        setupUi(title);
    }

protected:
    void setupUi(const QString& title);

    void setup_axis_lines(double x_min, double x_max, double y_min, double y_max);
    void plot_function(double a, double b, exprtk::expression<double>& expression, double& min, double& max);
    
    virtual bool calculate(double a, double b, unsigned n, const std::string& expr, double& result) = 0;

protected:
    QLineEdit* mLowerBoundEdit = nullptr;
    QLineEdit* mUpperBoundEdit = nullptr;
    QLineEdit* mStepsAmountEdit = nullptr;
    QLineEdit* mExpressionEdit = nullptr;
    QPushButton* mCalculateButton = nullptr;
    QLabel* mResultLabel = nullptr;

    QtCharts::QChart* mChart = nullptr;
    QtCharts::QChartView* mChartView = nullptr;
    QtCharts::QValueAxis* mAxisX = nullptr;
    QtCharts::QValueAxis* mAxisY = nullptr;

    QColor mFillColor = QColor("#75FF0000");
    QColor mBorderColor = Qt::red;
};

#endif
