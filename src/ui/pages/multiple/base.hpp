#ifndef UI_PAGES_MULTIPLE_BASE_HPP_
#define UI_PAGES_MULTIPLE_BASE_HPP_

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QChart>
#include <QValueAxis>
#include <QLineSeries>
#include <QChartView>
#include <QCheckBox>

namespace exprtk {
    template <typename T>
    class expression;
}

class MultipleIntegralBase : public QWidget {
    Q_OBJECT
public:
    struct CalculateResultData {
        double value;
        int steps_x;
        int steps_y;
    };
    
    using CalculateResult = std::optional<CalculateResultData>;

public:
    MultipleIntegralBase(const QString& title, QWidget* parent = nullptr);
    
protected:
    virtual CalculateResult calculate(double a, double b, double c, double d, int nx, int ny, const std::string& expr) = 0;
    
protected:
    void setupUi(const QString& title);
    void addInputs();
    void addOutputs();
    void setCalculateButtonCallback(std::function<void()> callback);
    void setError(const QString& errorText);
    void setResult(const QString& resultText);
    bool validate();
    
    void plot_function_3d(double a, double b, double c, double d, exprtk::expression<double>& expression);
    
protected:
    QLineEdit* mExpressionEdit = nullptr;
    QLineEdit* mLowerBoundXEdit = nullptr;
    QLineEdit* mUpperBoundXEdit = nullptr;
    QLineEdit* mLowerBoundYEdit = nullptr;
    QLineEdit* mUpperBoundYEdit = nullptr;
    QLineEdit* mStepsXEdit = nullptr;
    QLineEdit* mStepsYEdit = nullptr;
    QPushButton* mCalculateButton = nullptr;
    QLabel* mResultLabel = nullptr;
    
    QLayout* mMainLayout = nullptr;
    QChart* mChart = nullptr;
    QChartView* mChartView = nullptr;
};

#endif