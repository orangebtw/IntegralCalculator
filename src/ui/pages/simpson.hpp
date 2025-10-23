#ifndef TABS_SIMPSON_TAB_HPP_
#define TABS_SIMPSON_TAB_HPP_

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

#include "base.hpp"

class SimpsonPage final : public FixedStepsMethodBase {
    Q_OBJECT

public:
    SimpsonPage(QWidget* parent = nullptr) : FixedStepsMethodBase("Метод Симпсона", parent) {
    }
    ~SimpsonPage() = default;

private:
    virtual std::optional<CalculateResult> calculate(double a, double b, unsigned n, const std::string& expr) override;
};

#endif // TABS_TRAPEZOID_TAB_HPP_
