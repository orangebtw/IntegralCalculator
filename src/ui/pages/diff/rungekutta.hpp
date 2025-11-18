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
    }
    
    ~RungeKuttaPage() = default;

private:
    virtual CalculateResult calculate(double x0, double y0, double end, int steps, char dependentVar, char independentVar, const std::string& expr) override;
    virtual CalculateResult calculate2(double x0, double y0, double dy0, double end, int steps, char dependentVar, char dependentVar2, char independentVar, const std::string &expr1, const std::string &expr2) override;

private:
    QButtonGroup* mAlgorithmGroup = nullptr;
};

#endif
