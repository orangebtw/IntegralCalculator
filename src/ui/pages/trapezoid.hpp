#ifndef UI_TABS_TRAPEZOID_TAB_HPP_
#define UI_TABS_TRAPEZOID_TAB_HPP_

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

#include "base.hpp"

class TrapezoidPage final : public FixedStepsMethodBase {
    Q_OBJECT

public:
    TrapezoidPage(QWidget* parent = nullptr) : FixedStepsMethodBase("Метод трапеций", parent) {
    }
    ~TrapezoidPage() = default;

private:
    virtual std::optional<CalculateResult> calculate(double a, double b, unsigned n, const std::string& expr) override;
};

#endif // TABS_TRAPEZOID_TAB_HPP_
