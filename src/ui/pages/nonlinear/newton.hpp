#ifndef UI_PAGES_NONLINEAR_NEWTON_PAGE_HPP_
#define UI_PAGES_NONLINEAR_NEWTON_PAGE_HPP_

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
#include <expected>
#include <functional>
#include <qnamespace.h>

#include "../../widgets/gridwidget.hpp"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    using QtCharts::QChartView;
    using QtCharts::QChart;
    using QtCharts::QValueAxis;
#endif

class NonlinearNewtonMethodPage : public QWidget {
    Q_OBJECT
public:
    using CalculateResult = std::expected<double, const char*>;

public:
    NonlinearNewtonMethodPage(QWidget* parent = nullptr);

private:
    void setupUi(const QString& title);

    void setCalculateButtonCallback(std::function<void()> callback);

    void setError(const QString& errorText);
    void setResult(const QString& resultText);

    bool validate();

    CalculateResult calculate(double start, double epsilon, const std::string& function, const std::string& derivative);

protected:
    QPushButton* mCalculateButton = nullptr;
    QLayout* mMainLayout = nullptr;
    QLabel* mResultLabel = nullptr;

    QLineEdit* mFunctionEdit = nullptr;
    QLineEdit* mDerivativeEdit = nullptr;

    QLineEdit* mStartEdit = nullptr;
    QLineEdit* mEpsilonEdit = nullptr;

    GridWidget* mContainer = nullptr;

    QTableView* mTable = nullptr;
    std::unique_ptr<QStandardItemModel> mModel = nullptr;
};

#endif
