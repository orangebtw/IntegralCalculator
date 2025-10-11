#ifndef TABS_TRAPEZOID_TAB_HPP_
#define TABS_TRAPEZOID_TAB_HPP_

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class TrapezoidTab : public QWidget {
    Q_OBJECT

public:
    TrapezoidTab(QWidget* parent = nullptr);
    ~TrapezoidTab() = default;
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

#endif // TABS_TRAPEZOID_TAB_HPP_
