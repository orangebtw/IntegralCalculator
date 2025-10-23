#ifndef TABS_VARIABLE_STEP_TAB_HPP_
#define TABS_VARIABLE_STEP_TAB_HPP_

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

#include "base.hpp"

class VariableStepPage : public VariableStepMethodBase {
    Q_OBJECT
public:
    using VariableStepMethodBase::CalculateResult;

public:
    VariableStepPage(QWidget* parent = nullptr) : VariableStepMethodBase("Алгоритм с переменным шагом", parent) {
    }
    
    ~VariableStepPage() = default;

private:
    virtual std::optional<CalculateResult> calculate(double a, double b, double eps, const std::string& expr) override;
};

#endif // TABS_LEFT_RECTANGLES_TAB_HPP_
