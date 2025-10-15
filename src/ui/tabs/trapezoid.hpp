#ifndef UI_TABS_TRAPEZOID_TAB_HPP_
#define UI_TABS_TRAPEZOID_TAB_HPP_

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

#include "base_tab.hpp"

class TrapezoidTab : public MethodTab {
    Q_OBJECT

public:
    TrapezoidTab(QWidget* parent = nullptr) : MethodTab("Метод трапеций", parent) {
    }
    ~TrapezoidTab() = default;

private:
    virtual bool calculate(double a, double b, unsigned n, const std::string& expr, double& result) override;
};

#endif // TABS_TRAPEZOID_TAB_HPP_
