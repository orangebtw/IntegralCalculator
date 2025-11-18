#ifndef UI_PAGES_DIFF_BASE_PAGE_HPP_
#define UI_PAGES_DIFF_BASE_PAGE_HPP_

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QChart>
#include <QValueAxis>
#include <QLineSeries>
#include <QBarSeries>
#include <QChartView>
#include <QtGlobal>
#include <QCheckBox>
#include <QTableWidget>
#include <QStandardItemModel>
#include <QScrollArea>
#include <qnamespace.h>

#include <expected>

#include "../../../diff.hpp"

namespace exprtk {
    template <typename T>
    class expression;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    using QtCharts::QChartView;
    using QtCharts::QChart;
    using QtCharts::QValueAxis;
#endif

class DiffMethodPageBase : public QScrollArea {
    Q_OBJECT
public:
    using CalculateResult = std::expected<std::vector<diff::Result>, const char*>;
public:
    DiffMethodPageBase(bool secondOrder, const QString& title, QWidget* parent = nullptr);

protected:
    virtual CalculateResult calculate(double x0, double y0, double end, int steps, char dependentVar, char independentVar, const std::string& expr) = 0;
    virtual CalculateResult calculate2(double x0, double y0, double dy0, double end, int steps, char dependentVar, char dependentVar2, char independentVar, const std::string& expr1, const std::string& expr2) = 0;

protected:
    QWidget* createFirstOrderInputs();
    QWidget* createSecondOrderInputs();
    void addOutputs();

    QWidget* createStepsInputContainer();

private:
    void setupUi(const QString& title, bool secondOrder);

    void setCalculateButtonCallback(std::function<void()> callback);

    void setError(const QString& errorText);
    void setResult(const QString& resultText);

    void setup_x_axis_line(double x_min, double x_max);
    void setup_y_axis_line(double y_min, double y_max);

    bool validate();

protected:
    QString mFirstExpressionStr = "";
    QString mSecondExpressionStr = "";
    QString mLowerBoundStr = "0";
    QString mUpperBoundStr = "1";
    QString mStepsAmountStr = "";

    QString mFirstDependentVarStr = "y";
    QString mSecondDependentVarStr = "z";
    QString mStartYStr = "0";
    QString mStartDyStr = "0";

    QChar mIndependentVar = 'x';

    QPushButton* mCalculateButton = nullptr;
    QLayout* mMainLayout = nullptr;
    QLabel* mResultLabel = nullptr;

    QTableView* mTable = nullptr;
    std::unique_ptr<QStandardItemModel> mModel = nullptr;

    QLineSeries* mAxisXSeries = nullptr;
    QLineSeries* mAxisYSeries = nullptr;

    QChart* mChart = nullptr;
    QChartView* mChartView = nullptr;
    QValueAxis* mAxisX = nullptr;
    QValueAxis* mAxisY = nullptr;
    double mMinX = 0.0;
    double mMaxX = 0.0;
    double mMinY = 0.0;
    double mMaxY = 0.0;
};

#endif
