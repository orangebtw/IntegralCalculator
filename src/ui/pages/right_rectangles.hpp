#ifndef TABS_RIGHT_RECTANGLES_TAB_HPP_
#define TABS_RIGHT_RECTANGLES_TAB_HPP_

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

#include "base.hpp"

class RightRectanglesPage final : public FixedStepsMethodBase {
    Q_OBJECT

public:
    RightRectanglesPage(QWidget* parent = nullptr) : FixedStepsMethodBase("Метод прямоугольников правых частей", parent) {
    }

    ~RightRectanglesPage() = default;

private:
    virtual std::optional<CalculateResult> calculate(double a, double b, unsigned n, const std::string& expr) override;
};

#endif // TABS_RIGHT_RECTANGLES_TAB_HPP_

