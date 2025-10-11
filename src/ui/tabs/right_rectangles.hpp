#ifndef TABS_RIGHT_RECTANGLES_TAB_HPP_
#define TABS_RIGHT_RECTANGLES_TAB_HPP_

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class RightRectanglesTab : public QWidget {
    Q_OBJECT

public:
    RightRectanglesTab(QWidget* parent = nullptr);
    ~RightRectanglesTab() = default;

private:
    void setupUi();
    void calculate();

private:
    QLineEdit* m_lower_bound_edit = nullptr;
    QLineEdit* m_upper_bound_edit = nullptr;
    QLineEdit* m_steps_amount_edit = nullptr;
    QPushButton* m_calculate_button = nullptr;
    QLabel* m_result_label = nullptr;

};

#endif // TABS_RIGHT_RECTANGLES_TAB_HPP_

