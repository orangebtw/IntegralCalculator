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

class MethodTab : public QWidget {
    Q_OBJECT
public:
    MethodTab(const QString& title, QWidget* parent = nullptr) : QWidget(parent) {
        setupUi(title);
    }

protected:
    void setupUi(const QString& title);
    
    virtual bool calculate(double a, double b, unsigned n, const std::string& expr, double& result) = 0;

protected:
    QLineEdit* mLowerBoundEdit = nullptr;
    QLineEdit* mUpperBoundEdit = nullptr;
    QLineEdit* mStepsAmountEdit = nullptr;
    QLineEdit* mExpressionEdit = nullptr;
    QPushButton* mCalculateButton = nullptr;
    QLabel* mResultLabel = nullptr;

    QChart* mChart = nullptr;
    QChartView* mChartView = nullptr;
    QValueAxis* mAxisX = nullptr;
    QValueAxis* mAxisY = nullptr;

    QColor mFillColor = QColor("#75FF0000");
    QColor mBorderColor = Qt::red;
};

#endif