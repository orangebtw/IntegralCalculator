#ifndef TABS_SIMPSON_TAB_HPP_
#define TABS_SIMPSON_TAB_HPP_

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

#include "base.hpp"

class SimpsonPage final : public MethodPageBase {
    Q_OBJECT

public:
    SimpsonPage(QWidget* parent = nullptr) : MethodPageBase("Метод Симпсона", parent) {
        initDefaultLayout();
    }
    ~SimpsonPage() = default;

private:
    virtual std::optional<CalculateResult> calculateWithFixedStep(double a, double b, unsigned n, const std::string& expr) override;
    virtual std::optional<CalculateResult> calculateWithVarStep(double a, double b, double eps, const std::string& expr) override;
};

#endif // TABS_TRAPEZOID_TAB_HPP_
