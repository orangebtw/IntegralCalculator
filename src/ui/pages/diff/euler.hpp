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
    virtual std::expected<std::vector<diff::Result>, const char*> calculate(double x0, double y0, double end, int steps, char dependentVar, char independentVar, const std::string& expr) override;
    virtual CalculateResult calculate2(double x0, double end, int steps, char independentVar, const std::vector<double>& startValues, const std::vector<char>& dependentVars, const std::vector<std::string>& exprs) override;

private:
    QButtonGroup* mAlgorithmGroup = nullptr;
};

#endif
