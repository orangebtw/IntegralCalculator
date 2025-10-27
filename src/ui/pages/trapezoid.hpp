#ifndef UI_TABS_TRAPEZOID_TAB_HPP_
#define UI_TABS_TRAPEZOID_TAB_HPP_

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

#include "base.hpp"

class TrapezoidPage final : public MethodPageBase {
    Q_OBJECT

public:
    TrapezoidPage(QWidget* parent = nullptr) : MethodPageBase("Метод трапеций", parent) {
        initDefaultLayout();
    }
    ~TrapezoidPage() = default;

private:
    virtual std::optional<CalculateResult> calculateWithFixedStep(double a, double b, unsigned n, const std::string& expr) override;
    virtual std::optional<CalculateResult> calculateWithVarStep(double a, double b, double eps, const std::string& expr) override;
};

#endif // TABS_TRAPEZOID_TAB_HPP_
