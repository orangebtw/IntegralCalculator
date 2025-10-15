#ifndef TABS_RIGHT_RECTANGLES_TAB_HPP_
#define TABS_RIGHT_RECTANGLES_TAB_HPP_

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

#include "base_tab.hpp"
#include "../../integral.hpp"

class RightRectanglesTab : public MethodTab {
    Q_OBJECT

public:
    RightRectanglesTab(QWidget* parent = nullptr) : MethodTab("Метод прямоугольников правых частей", parent) {
    }

    ~RightRectanglesTab() = default;

private:
    virtual bool calculate(double a, double b, unsigned n, const std::string& expr, double& result) override {
        return integral::rectangles_right(a, b, n, expr, result);
    }
};

#endif // TABS_RIGHT_RECTANGLES_TAB_HPP_

