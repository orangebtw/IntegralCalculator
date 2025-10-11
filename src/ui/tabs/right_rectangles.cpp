#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QIntValidator>
#include <QDoubleValidator>

#include "right_rectangles.hpp"
#include "../../integral.hpp"

RightRectanglesTab::RightRectanglesTab(QWidget* parent) : QWidget(parent) {
    setupUi();
}

void RightRectanglesTab::setupUi() {
    QDoubleValidator* doubleValidator = new QDoubleValidator();
    QIntValidator* intValidator = new QIntValidator();

    m_lower_bound_edit = new QLineEdit();
    m_lower_bound_edit->setPlaceholderText("Нижний предел");
    m_lower_bound_edit->setValidator(doubleValidator);

    m_upper_bound_edit = new QLineEdit();
    m_upper_bound_edit->setPlaceholderText("Верхний предел");
    m_upper_bound_edit->setValidator(doubleValidator);

    m_steps_amount_edit = new QLineEdit();
    m_steps_amount_edit->setPlaceholderText("Кол-во шагов");
    m_steps_amount_edit->setValidator(intValidator);

    m_calculate_button = new QPushButton("Вычислить");

    m_result_label = new QLabel();
    m_result_label->setStyleSheet("QLabel{ font-size: 18px; }");

    QLabel* titleLabel = new QLabel("Метод прямоугольников правых частей");
    titleLabel->setStyleSheet("QLabel{ font-size: 24px; }");
    titleLabel->setAlignment(Qt::AlignHCenter);

    QVBoxLayout* controlsLayout = new QVBoxLayout();
    controlsLayout->setAlignment(Qt::AlignTop);
    controlsLayout->setSpacing(10);
    controlsLayout->addWidget(titleLabel);
    controlsLayout->addWidget(m_lower_bound_edit);
    controlsLayout->addWidget(m_upper_bound_edit);
    controlsLayout->addWidget(m_steps_amount_edit);
    controlsLayout->addWidget(m_calculate_button);
    controlsLayout->addWidget(m_result_label);

    QWidget* controlsWidget = new QWidget();
    controlsWidget->setLayout(controlsLayout);

    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addWidget(controlsWidget);

    QObject::connect(m_calculate_button, &QPushButton::clicked, this, &RightRectanglesTab::calculate);

    setLayout(mainLayout);
}

void RightRectanglesTab::calculate() {
    double a = m_lower_bound_edit->text().toDouble();
    double b = m_upper_bound_edit->text().toDouble();
    unsigned n = m_steps_amount_edit->text().toUInt();
    
    double result = integral::rectangles_right(a, b, n);

    m_result_label->setText(QString::number(result));
}
