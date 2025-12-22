#include "simpson.hpp"
#include "../../../exprtk.hpp"

MultipleSimpsonPage::MultipleSimpsonPage(QWidget* parent) : MultipleIntegralBase("Кратный интеграл (двойной интеграл методом Симпсона)", parent) {}

MultipleIntegralBase::CalculateResult MultipleSimpsonPage::calculate(double a, double b, double c, double d, int nx, int ny, const std::string& expr_str) {
    try {
        typedef exprtk::symbol_table<double> symbol_table_t;
        typedef exprtk::expression<double> expression_t;
        typedef exprtk::parser<double> parser_t;
        
        symbol_table_t symbol_table;
        double x = 0.0, y = 0.0;
        symbol_table.add_variable("x", x);
        symbol_table.add_variable("y", y);
        symbol_table.add_constants();
        
        expression_t expression;
        expression.register_symbol_table(symbol_table);
        
        parser_t parser;
        if (!parser.compile(expr_str, expression)) {
            setError("Ошибка парсинга выражения");
            return std::nullopt;
        }
        
        // Убедимся, что шаги четные
        if (nx % 2 != 0) nx++;
        if (ny % 2 != 0) ny++;
        
        // Вычисляем интеграл
        double result = double_simpson(a, b, c, d, nx, ny, expression);
        
        return CalculateResultData{result, nx, ny};
        
    } catch (const std::exception& e) {
        setError(QString("Ошибка вычисления: %1").arg(e.what()));
        return std::nullopt;
    } catch (...) {
        setError("Неизвестная ошибка вычисления");
        return std::nullopt;
    }
}

double MultipleSimpsonPage::inner_simpson(double x_val, double c, double d, int ny, exprtk::expression<double>& expr) {
    double& x = expr.get_symbol_table().get_variable("x")->ref();
    double& y = expr.get_symbol_table().get_variable("y")->ref();
    
    x = x_val;
    
    double hy = (d - c) / ny;
    double sum = 0.0;
    
    y = c;
    double f_c = expr.value();
    y = d;
    double f_d = expr.value();
    
    sum = f_c + f_d;
    
    for (int j = 1; j < ny; j++) {
        y = c + j * hy;
        double value = expr.value();
        if (j % 2 == 0) {
            sum += 2.0 * value;
        } else {
            sum += 4.0 * value;
        }
    }
    
    double result = sum * hy / 3.0;
    return result;
}

double MultipleSimpsonPage::double_simpson(double a, double b, double c, double d, int nx, int ny, exprtk::expression<double>& expr) {
    double& x = expr.get_symbol_table().get_variable("x")->ref();
    double& y = expr.get_symbol_table().get_variable("y")->ref();
    
    double hx = (b - a) / nx;
    
    // Вычисляем F(x_i) для каждого узла x_i
    std::vector<double> F(nx + 1);
    for (int i = 0; i <= nx; i++) {
        x = a + i * hx;
        F[i] = inner_simpson(x, c, d, ny, expr);
    }
    
    // Внешний Симпсон по x
    double sum = F[0] + F[nx];
    
    for (int i = 1; i < nx; i++) {
        if (i % 2 == 0) {
            sum += 2.0 * F[i];
        } else {
            sum += 4.0 * F[i];
        }
    }
    
    return sum * hx / 3.0;
}