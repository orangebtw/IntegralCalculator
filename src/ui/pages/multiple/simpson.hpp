#ifndef UI_PAGES_MULTIPLE_SIMPSON_HPP_
#define UI_PAGES_MULTIPLE_SIMPSON_HPP_

#include "base.hpp"

class MultipleSimpsonPage : public MultipleIntegralBase {
    Q_OBJECT
public:
    MultipleSimpsonPage(QWidget* parent = nullptr) : MultipleIntegralBase("Кратный интеграл (двойной интеграл методом Симпсона)", parent) {        
    }
    
protected:
    MultipleIntegralBase::CalculateResult calculate(double a, double b, double c, double d, int nx, int ny, const std::string& expr) override;
    
private:
    double inner_simpson(double x, double c, double d, int ny, exprtk::expression<double>& expr);
    double double_simpson(double a, double b, double c, double d, int nx, int ny, exprtk::expression<double>& expr);
};

#endif