#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDoubleValidator>
#include <QPushButton>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qvalidator.h>

#include "left_rectangles.hpp"

#include "../../integral.hpp"

LeftRectanglesTab::LeftRectanglesTab(QWidget* parent) : QWidget(parent) {
    setupUi();
}

void LeftRectanglesTab::setupUi() {
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

    QLabel* titleLabel = new QLabel("Метод прямоугольников левых частей");
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

    QObject::connect(m_calculate_button, &QPushButton::clicked, this, &LeftRectanglesTab::calculate);

    setLayout(mainLayout);
}

void LeftRectanglesTab::calculate() {
    double a = m_lower_bound_edit->text().toDouble();
    double b = m_upper_bound_edit->text().toDouble();
    unsigned n = m_steps_amount_edit->text().toUInt();
    
    double result = integral::rectangles_left(a, b, n);

    m_result_label->setText(QString::number(result));
}

