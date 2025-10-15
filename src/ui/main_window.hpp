#ifndef APP_HPP_
#define APP_HPP_

#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <memory>

enum class IntegralMethod : uint8_t {
    LeftRectangles = 0,
    RightRectangles,
    Trapezoid,
    Simpson,
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(); 
    ~MainWindow() {};

private:
    void setupUi();

private:
    QHBoxLayout* m_main_container_layout = nullptr;
    std::unique_ptr<QWidget>  m_content_widget = nullptr;
    QPushButton* m_left_rectangles_tab_button = nullptr;
    QPushButton* m_right_rectangles_tab_button = nullptr;
    QPushButton* m_trapezoid_tab_button = nullptr;
    QPushButton* m_simpson_tab_button = nullptr;
    IntegralMethod m_current_method = IntegralMethod::LeftRectangles;
};

#endif // APP_HPP_
