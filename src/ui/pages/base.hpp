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
#include <QtGlobal>
#include <QCheckBox>
#include <qnamespace.h>

namespace exprtk {
    template <typename T>
    class expression;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    using QtCharts::QChartView;
    using QtCharts::QChart;
    using QtCharts::QValueAxis;
#endif

class MethodPageBase : public QWidget {
    Q_OBJECT
public:
    struct CalculateResult {
        double value;
        int steps;
    };
public:
    MethodPageBase(const QString& title, QWidget* parent = nullptr) : QWidget(parent) {
        setupUi(title);
        setCalculateButtonCallback([this] {
            if (!validate()) return;

            QLocale locale = QLocale::system();

            std::string expr = mExpressionEdit->text().toStdString();
            double a = locale.toDouble(mLowerBoundEdit->text());
            double b = locale.toDouble(mUpperBoundEdit->text());
            bool isVariableStep = (mVariableStepCheck->checkState() == Qt::Checked);
            
            std::optional<CalculateResult> result;
            
            if (isVariableStep) {
                double epsilon = locale.toDouble(mEpsilonEdit->text());
                result = calculateWithVarStep(a, b, epsilon, expr);
                setResult(QString("Результат: %1, с количеством шагов: %2").arg(result->value).arg(result->steps));
            } else {
                int steps = mStepsAmountEdit->text().toInt();
                result = calculateWithFixedStep(a, b, steps, expr);
                setResult(QString("Результат: %1").arg(result->value));
            }

            if (!result.has_value()) {
                setError("Ошибка");
            }
        });
    }

protected:
    virtual std::optional<CalculateResult> calculateWithFixedStep(double a, double b, unsigned n, const std::string& expr) = 0;
    virtual std::optional<CalculateResult> calculateWithVarStep(double a, double b, double eps, const std::string& expr) = 0;

protected:
    void plot_function(double a, double b, exprtk::expression<double>& expression);

protected:
    void addBaseInputs();
    void addStepsInput(QWidget* stepsContainer, QWidget* epsilonContainer);
    void addOutputs();

    QWidget* createEpsilonInputContainer();
    QWidget* createStepsInputContainer();

    void initDefaultLayout() {
        addBaseInputs();
        addStepsInput(createStepsInputContainer(), createEpsilonInputContainer());
        addOutputs();
    }

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

    QChart* mChart = nullptr;
    QChartView* mChartView = nullptr;
    QValueAxis* mAxisX = nullptr;
    QValueAxis* mAxisY = nullptr;

    QColor mFillColor = QColor("#75FF0000");
    QColor mBorderColor = Qt::red;
};

#endif
