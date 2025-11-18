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
    struct Result {
        std::vector<std::pair<double, double>> data;
    };

    using CalculateResult = std::expected<Result, const char*>;
public:
    DiffMethodPageBase(const QString& title, QWidget* parent = nullptr);

protected:
    virtual CalculateResult calculateWithFixedStep(double x0, double y0, double end, int steps, char dependentVar, char independentVar, const std::string& expr) = 0;
    virtual CalculateResult calculateWithVarStep(const std::string& expr) = 0;

protected:
    void plot_function(double a, double b, exprtk::expression<double>& expression);

protected:
    void addBaseInputs();
    void addOutputs();

    QWidget* createEpsilonInputContainer();
    QWidget* createStepsInputContainer();

private:
    void setupUi(const QString& title);

    void setCalculateButtonCallback(std::function<void()> callback);

    void setError(const QString& errorText);
    void setResult(const QString& resultText);

    void setup_x_axis_line(double x_min, double x_max);
    void setup_y_axis_line(double y_min, double y_max);

    bool validate();

protected:
    QLineEdit* mExpressionEdit = nullptr;
    QLineEdit* mLowerBoundEdit = nullptr;
    QLineEdit* mUpperBoundEdit = nullptr;
    QLineEdit* mStepsAmountEdit = nullptr;
    QLineEdit* mEpsilonEdit = nullptr;
    QPushButton* mCalculateButton = nullptr;
    QLayout* mMainLayout = nullptr;
    QLabel* mResultLabel = nullptr;

    QLineEdit* mDependentVarEdit = nullptr;
    QLineEdit* mIndependentVarEdit = nullptr;
    QLineEdit* mStartValueEdit = nullptr;
    QLineEdit* mStepEdit = nullptr;

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

    QColor mFillColor = QColor("#75FF0000");
    QColor mBorderColor = Qt::red;
};

#endif
