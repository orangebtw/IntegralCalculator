#ifndef UI_PAGES_DIFF_EULER_PAGE_HPP_
#define UI_PAGES_DIFF_EULER_PAGE_HPP_

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QIntValidator>
#include <QButtonGroup>
#include <QRadioButton>

#include "base.hpp"

class EulerPage final : public DiffMethodPageBase {
    Q_OBJECT

public:
    EulerPage(QWidget* parent = nullptr) : DiffMethodPageBase("Метод эйлера", parent) {
        addBaseInputs();
        addStepsInput(createStepsInputContainer(), createEpsilonInputContainer());
        addOutputs();
    }
    
    ~EulerPage() = default;

private:
    virtual std::optional<CalculateResult> calculateWithFixedStep(double x0, double y0, int steps, char dependentVar, char independentVar, const std::string& expr) override;
    virtual std::optional<CalculateResult> calculateWithVarStep(const std::string& expr) override;

private:
    QButtonGroup* mAlgorithmGroup = nullptr;
};

#endif
