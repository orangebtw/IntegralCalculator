#ifndef TABS_LEFT_RECTANGLES_TAB_HPP_
#define TABS_LEFT_RECTANGLES_TAB_HPP_

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

#include "base_tab.hpp"

class LeftRectanglesTab : public MethodTab {
    Q_OBJECT

public:
    LeftRectanglesTab(QWidget* parent = nullptr) : MethodTab("Метод прямоугольников левых частей", parent) {
    }
    
    ~LeftRectanglesTab() = default;

private:
    virtual bool calculate(double a, double b, unsigned n, const std::string& expr, double& result) override;
};

#endif // TABS_LEFT_RECTANGLES_TAB_HPP_
