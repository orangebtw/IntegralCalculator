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

class DiffMethodPageBase : public QWidget {
    Q_OBJECT
public:
    using CalculateResult = std::expected<double, const char*>;
public:
    DiffMethodPageBase(const QString& title, QWidget* parent = nullptr) : QWidget(parent) {
        setupUi(title);
        setCalculateButtonCallback([this] {
            if (!validate()) return;
            
            QLocale locale = QLocale::system();

            std::string expr = mExpressionEdit->text().toStdString();
            double a = locale.toDouble(mLowerBoundEdit->text());
            double b = locale.toDouble(mUpperBoundEdit->text());
            char dependentVar = mDependentVarEdit->text().at(0).toLatin1();
            char independentVar = mIndependentVarEdit->text().at(0).toLatin1();
            bool isVariableStep = (mVariableStepCheck->checkState() == Qt::Checked);

            double startDependent = locale.toDouble(mStartXEdit->text());
            double startIndependent = locale.toDouble(mStartYEdit->text());
            
            std::optional<CalculateResult> result;
            
            if (isVariableStep) {
                double epsilon = locale.toDouble(mEpsilonEdit->text());
                // result = calculateWithVarStep(a, b, epsilon, expr);
            } else {
                int steps = mStepsAmountEdit->text().toInt();
                result = calculateWithFixedStep(startDependent, startIndependent, steps, dependentVar, independentVar, expr);
            }

            if (result->has_value()) {
                setResult(QString("Результат: %1").arg(result->value()));
            } else {
                setError(QString("Ошибка: %1.").arg(result->error()));
            }
        });
    }

protected:
    virtual std::optional<CalculateResult> calculateWithFixedStep(double x0, double y0, int steps, char dependentVar, char independentVar, const std::string& expr) = 0;
    virtual std::optional<CalculateResult> calculateWithVarStep(const std::string& expr) = 0;

protected:
    void plot_function(double a, double b, exprtk::expression<double>& expression);

protected:
    void addBaseInputs();
    void addStepsInput(QWidget* stepsContainer, QWidget* epsilonContainer);
    void addOutputs();

    QWidget* createEpsilonInputContainer();
    QWidget* createStepsInputContainer();

private:
    void setupUi(const QString& title);

    void setCalculateButtonCallback(std::function<void()> callback);

    void setError(const QString& errorText);
    void setResult(const QString& resultText);

    void setup_axis_lines(double x_min, double x_max, double y_min, double y_max);

    bool validate();

protected:
    QLineEdit* mExpressionEdit = nullptr;
    QLineEdit* mLowerBoundEdit = nullptr;
    QLineEdit* mUpperBoundEdit = nullptr;
    QLineEdit* mStepsAmountEdit = nullptr;
    QLineEdit* mEpsilonEdit = nullptr;
    QCheckBox* mVariableStepCheck = nullptr;
    QPushButton* mCalculateButton = nullptr;
    QLayout* mMainLayout = nullptr;
    QLabel* mResultLabel = nullptr;

    QLineEdit* mDependentVarEdit = nullptr;
    QLineEdit* mIndependentVarEdit = nullptr;
    QLineEdit* mStartXEdit = nullptr;
    QLineEdit* mStartYEdit = nullptr;
    QLineEdit* mStepEdit = nullptr;

    QChart* mChart = nullptr;
    QChartView* mChartView = nullptr;
    QValueAxis* mAxisX = nullptr;
    QValueAxis* mAxisY = nullptr;

    QColor mFillColor = QColor("#75FF0000");
    QColor mBorderColor = Qt::red;
};

#endif
