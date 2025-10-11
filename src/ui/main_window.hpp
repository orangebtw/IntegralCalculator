#ifndef APP_HPP_
#define APP_HPP_

#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <memory>

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
};

#endif // APP_HPP_
