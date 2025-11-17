#ifndef UI_PAGES_DIFF_RUNGE_KUTTA_PAGE_HPP_
#define UI_PAGES_DIFF_RUNGE_KUTTA_PAGE_HPP_

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QIntValidator>
#include <QButtonGroup>
#include <QRadioButton>

#include "base.hpp"

class RungeKuttaPage final : public DiffMethodPageBase {
    Q_OBJECT

public:
    RungeKuttaPage(QWidget* parent = nullptr) : DiffMethodPageBase("Метод Рунге-Кутта", parent) {
        addBaseInputs();
        addStepsInput(createStepsInputContainer(), createEpsilonInputContainer());
        addOutputs();
    }
    
    ~RungeKuttaPage() = default;

private:
    virtual std::optional<CalculateResult> calculateWithFixedStep(double x0, double y0, int steps, char dependentVar, char independentVar, const std::string& expr) override;
    virtual std::optional<CalculateResult> calculateWithVarStep(const std::string& expr) override;

private:
    QButtonGroup* mAlgorithmGroup = nullptr;
};

#endif
