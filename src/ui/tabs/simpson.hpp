#ifndef TABS_SIMPSON_TAB_HPP_
#define TABS_SIMPSON_TAB_HPP_

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

#include "base_tab.hpp"

class SimpsonTab : public MethodTab {
    Q_OBJECT

public:
    SimpsonTab(QWidget* parent = nullptr) : MethodTab("Метод Симпсона", parent) {
    }
    ~SimpsonTab() = default;

private:
    virtual bool calculate(double a, double b, unsigned n, const std::string& expr, double& result) override;
};

#endif // TABS_TRAPEZOID_TAB_HPP_
