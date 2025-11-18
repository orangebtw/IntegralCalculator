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
    EulerPage(bool secondOrder, QWidget* parent = nullptr) : DiffMethodPageBase(secondOrder, "Метод Эйлера", parent) {
    }
    
    ~EulerPage() = default;

private:
    virtual CalculateResult calculate(double x0, double y0, double end, int steps, char dependentVar, char independentVar, const std::string& expr) override;
    virtual CalculateResult calculate2(double x0, double y0, double dy0, double end, int steps, char dependentVar, char dependentVar2, char independentVar, const std::string &expr1, const std::string &expr2) override;

private:
    QButtonGroup* mAlgorithmGroup = nullptr;
};

#endif
