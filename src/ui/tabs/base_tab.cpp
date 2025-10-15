#include "base_tab.hpp"

#include <QDoubleValidator>
#include <QIntValidator>
#include <QHBoxLayout>

void MethodTab::setupUi(const QString& title) {
    QDoubleValidator* doubleValidator = new QDoubleValidator();
    QIntValidator* intValidator = new QIntValidator();

    mLowerBoundEdit = new QLineEdit();
    mLowerBoundEdit->setPlaceholderText("Нижний предел");
    mLowerBoundEdit->setValidator(doubleValidator);

    mUpperBoundEdit = new QLineEdit();
    mUpperBoundEdit->setPlaceholderText("Верхний предел");
    mUpperBoundEdit->setValidator(doubleValidator);

    mStepsAmountEdit = new QLineEdit();
    mStepsAmountEdit->setPlaceholderText("Кол-во шагов");
    mStepsAmountEdit->setValidator(intValidator);

    mExpressionEdit = new QLineEdit();
    mExpressionEdit->setPlaceholderText("Подинтегральная функция");

    mCalculateButton = new QPushButton("Вычислить");

    mResultLabel = new QLabel();
    mResultLabel->setStyleSheet("QLabel{ font-size: 18px; }");

    QLabel* titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("QLabel{ font-size: 24px; }");
    titleLabel->setAlignment(Qt::AlignHCenter);

    QVBoxLayout* controlsLayout = new QVBoxLayout();
    controlsLayout->setAlignment(Qt::AlignTop);
    controlsLayout->setSpacing(10);
    controlsLayout->addWidget(titleLabel);
    controlsLayout->addWidget(mExpressionEdit);
    controlsLayout->addWidget(mLowerBoundEdit);
    controlsLayout->addWidget(mUpperBoundEdit);
    controlsLayout->addWidget(mStepsAmountEdit);
    controlsLayout->addWidget(mCalculateButton);
    controlsLayout->addWidget(mResultLabel);

    QWidget* controlsWidget = new QWidget();
    controlsWidget->setLayout(controlsLayout);

    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addWidget(controlsWidget);

    QObject::connect(mCalculateButton, &QPushButton::clicked, this, [this] {
        const double a = mLowerBoundEdit->text().toDouble();
        const double b = mUpperBoundEdit->text().toDouble();
        const unsigned n = mStepsAmountEdit->text().toUInt();
        const std::string expr = mExpressionEdit->text().toStdString();
        
        double result = 0.0;
        const bool success = calculate(a, b, n, expr, result);

        QPalette palette = mResultLabel->palette();

        if (success) {
            palette.setColor(mResultLabel->foregroundRole(), Qt::black);
            mResultLabel->setText(QString::number(result));
        } else {
            palette.setColor(mResultLabel->foregroundRole(), Qt::red);
            mResultLabel->setText("Ошибка.");
        }

        mResultLabel->setPalette(palette);
    });

    setLayout(mainLayout);
}