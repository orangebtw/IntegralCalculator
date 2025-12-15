#ifndef UI_PAGES_APPROX_BASE_PAGE_HPP_
#define UI_PAGES_APPROX_BASE_PAGE_HPP_

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
#include <QButtonGroup>
#include <functional>
#include <qnamespace.h>

#include "../../widgets/gridwidget.hpp"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    using QtCharts::QChartView;
    using QtCharts::QChart;
    using QtCharts::QValueAxis;
#endif

class ApproxCalculationPage : public QWidget {
    Q_OBJECT
public:
    using CalculateResult = double;

    enum class FunctionType : uint8_t {
        EtoX = 0,
        Sin,
        Sqrt,
        InvSqrt
    };

public:
    ApproxCalculationPage(QWidget* parent = nullptr);

private:
    void setupUi(const QString& title);

    void setCalculateButtonCallback(std::function<void()> callback);

    void setError(const QString& errorText);
    void setResult(const QString& resultText);

    bool validate();

private:
    void functionTypeChanged(FunctionType functionType);

protected:
    QPushButton* mCalculateButton = nullptr;
    QLayout* mMainLayout = nullptr;
    QLabel* mResultLabel = nullptr;

    QLineEdit* mStartXEdit = nullptr;
    QLineEdit* mStartYEdit = nullptr;
    QLineEdit* mEpsilonEdit = nullptr;

    QButtonGroup* mFunctionGroup = nullptr;

    GridWidget* mContainer = nullptr;

    FunctionType mFunctionType = FunctionType::EtoX;
};

#endif
