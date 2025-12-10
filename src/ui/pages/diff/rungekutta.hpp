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
    virtual CalculateResult calculate(double x0, double end, int steps, char independentVar, const std::vector<double>& startValues, const std::vector<char>& dependentVars, const std::vector<std::string>& exprs) override;

private:
    QButtonGroup* mAlgorithmGroup = nullptr;
};

#endif
