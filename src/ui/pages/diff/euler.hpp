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
    EulerPage(QWidget* parent = nullptr) : DiffMethodPageBase("Метод Эйлера", parent) {
        addBaseInputs();
        addOutputs();
    }
    
    ~EulerPage() = default;

private:
    virtual CalculateResult calculateWithFixedStep(double x0, double y0, double end, int steps, char dependentVar, char independentVar, const std::string& expr) override;
    virtual CalculateResult calculateWithVarStep(const std::string& expr) override;

private:
    QButtonGroup* mAlgorithmGroup = nullptr;
};

#endif
