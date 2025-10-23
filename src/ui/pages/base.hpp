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
    MethodPageBase(const QString& title, QWidget* parent = nullptr) : QWidget(parent) {
        setupUi(title);
    }

protected:
    void setupUi(const QString& title);

    void setup_axis_lines(double x_min, double x_max, double y_min, double y_max);
    void plot_function(double a, double b, exprtk::expression<double>& expression, double& min, double& max);

    void addBaseInputs();
    void addOutputs();
    void addCalculateButton();
    void setCalculateButtonCallback(std::function<void()> callback);

    void setError(const QString& errorText);
    void setResult(const QString& resultText);

    bool validate();

protected:
    QLineEdit* mLowerBoundEdit = nullptr;
    QLineEdit* mUpperBoundEdit = nullptr;
    QPushButton* mCalculateButton = nullptr;
    QLineEdit* mExpressionEdit = nullptr;
    QLayout* mMainLayout = nullptr;
    QLabel* mResultLabel = nullptr;

    QChart* mChart = nullptr;
    QChartView* mChartView = nullptr;
    QValueAxis* mAxisX = nullptr;
    QValueAxis* mAxisY = nullptr;

    QColor mFillColor = QColor("#75FF0000");
    QColor mBorderColor = Qt::red;
};

class FixedStepsMethodBase : public MethodPageBase {
public:
    struct CalculateResult {
        double value;
    };

public:
    FixedStepsMethodBase(const QString& title, QWidget* parent = nullptr) : MethodPageBase(title, parent) {
        addBaseInputs();
        addStepsInput();
        addCalculateButton();
        addOutputs();
        setCalculateButtonCallback([this] {
            if (!validate()) return;

            QLocale locale = QLocale::system();

            std::string expr = mExpressionEdit->text().toStdString();
            double a = locale.toDouble(mLowerBoundEdit->text());
            double b = locale.toDouble(mUpperBoundEdit->text());
            int n = mStepsAmountEdit->text().toInt();

            auto result = calculate(a, b, n, expr);
            if (result.has_value()) {
                setResult(QString("Результат: %1").arg(result->value));
            } else {
                setError("Ошибка");
            }
        });
    }

protected:
    virtual std::optional<CalculateResult> calculate(double a, double b, unsigned n, const std::string& expr) = 0;

private:
    void addStepsInput();
    bool validate();

protected:
    QLineEdit* mStepsAmountEdit = nullptr;
};

class VariableStepMethodBase : public MethodPageBase {
public:
    struct CalculateResult {
        double value;
        int steps;
    };
public:
    VariableStepMethodBase(const QString& title, QWidget* parent = nullptr) : MethodPageBase(title, parent) {
        addBaseInputs();
        addEpsilonInput();
        addCalculateButton();
        addOutputs();
        setCalculateButtonCallback([this] {
            if (!validate()) return;

            QLocale locale = QLocale::system();

            std::string expr = mExpressionEdit->text().toStdString();
            double a = locale.toDouble(mLowerBoundEdit->text());
            double b = locale.toDouble(mUpperBoundEdit->text());
            double epsilon = locale.toDouble(mEpsilonEdit->text());

            auto result = calculate(a, b, epsilon, expr);
            if (result.has_value()) {
                setResult(QString("Результат: %1, с количеством шагов: %2").arg(result->value).arg(result->steps));
            } else {
                setError("Ошибка");
            }
        });
    }

protected:
    virtual std::optional<CalculateResult> calculate(double a, double b, double eps, const std::string& expr) = 0;

private:
    void addEpsilonInput();
    bool validate();

protected:
    QLineEdit* mEpsilonEdit = nullptr;
};

#endif
