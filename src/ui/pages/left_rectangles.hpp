#ifndef TABS_LEFT_RECTANGLES_TAB_HPP_
#define TABS_LEFT_RECTANGLES_TAB_HPP_

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

#include "base.hpp"

class LeftRectanglesPage final : public FixedStepsMethodBase {
    Q_OBJECT

public:
    LeftRectanglesPage(QWidget* parent = nullptr) : FixedStepsMethodBase("Метод прямоугольников левых частей", parent) {
    }
    
    ~LeftRectanglesPage() = default;

private:
    virtual std::optional<CalculateResult> calculate(double a, double b, unsigned n, const std::string& expr) override;
};

#endif // TABS_LEFT_RECTANGLES_TAB_HPP_
