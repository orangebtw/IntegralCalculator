#ifndef UI_PAGES_INTEGRAL_RIGHT_RECTANGLES_TAB_HPP_
#define UI_PAGES_INTEGRAL_RIGHT_RECTANGLES_TAB_HPP_

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

#include "base.hpp"

class RightRectanglesPage final : public IntegralMethodPageBase {
    Q_OBJECT

public:
    RightRectanglesPage(QWidget* parent = nullptr) : IntegralMethodPageBase("Метод прямоугольников правых частей", parent) {
        initDefaultLayout();
    }

    ~RightRectanglesPage() = default;

private:
    virtual std::optional<CalculateResult> calculateWithFixedStep(double a, double b, unsigned n, const std::string& expr) override;
    virtual std::optional<CalculateResult> calculateWithVarStep(double a, double b, double eps, const std::string& expr) override;
};

#endif // UI_PAGES_INTEGRAL_RIGHT_RECTANGLES_TAB_HPP_

