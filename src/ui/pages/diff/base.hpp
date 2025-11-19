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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    using QtCharts::QChartView;
    using QtCharts::QChart;
    using QtCharts::QValueAxis;
#endif

struct EquationInputWidget {
    QLineEdit* expressionEdit;
    QLineEdit* dependentVarEdit;
    QLineEdit* independentVarEdit;
    QLineEdit* startValueEdit;
    QLabel* startValueLabel;
    QWidget* equationContainer;
    QWidget* startValueContainer;
};

class DiffMethodPageBase : public QScrollArea {
    Q_OBJECT
public:
    using CalculateResult = std::expected<std::vector<std::vector<double>>, const char*>;
public:
    DiffMethodPageBase(bool secondOrder, const QString& title, QWidget* parent = nullptr);

protected:
    virtual std::expected<std::vector<diff::Result>, const char*> calculate(double x0, double y0, double end, int steps, char dependentVar, char independentVar, const std::string& expr) = 0;
    virtual CalculateResult calculate2(double x0, double end, int steps, char independentVar, const std::vector<double>& startValues, const std::vector<char>& dependentVars, const std::vector<std::string>& exprs) = 0;

protected:
    QWidget* createFirstOrderInputs();
    QWidget* createSecondOrderInputs();
    void addOutputs();

    QWidget* createStepsInputContainer();

    EquationInputWidget createEquationInput(const QString& dependentVarStr, QString& independentVarStr, QWidget* parent = nullptr);

private:
    void setupUi(const QString& title, bool secondOrder);

    void setCalculateButtonCallback(std::function<void()> callback);

    void setError(const QString& errorText);
    void setResult(const QString& resultText);

    void setup_x_axis_line(double x_min, double x_max);
    void setup_y_axis_line(double y_min, double y_max);

    bool validate();

protected:
    QString mLowerBoundStr = "0";
    QString mUpperBoundStr = "1";
    QString mStepsAmountStr = "";

    QString mIndependentVarStr = "x";

    std::vector<EquationInputWidget> mEquationWidgets;

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
