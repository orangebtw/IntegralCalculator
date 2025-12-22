#include <QDoubleValidator>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QChart>
#include <QChartView>
#include <QValueAxis>

#include "base.hpp"
#include "../../widgets/hboxwidget.hpp"
#include "../../widgets/vboxwidget.hpp"
#include "../../utils.hpp"

MultipleIntegralBase::MultipleIntegralBase(const QString& title, QWidget* parent) : QWidget(parent) {
    setupUi(title);
}

void MultipleIntegralBase::setupUi(const QString& title) {
    mMainLayout = new QVBoxLayout();
    setLayout(mMainLayout);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    
    mCalculateButton = new QPushButton("Вычислить");
    mResultLabel = new QLabel();
    SetFontSize(mResultLabel, 18.0f);
    
    QLabel* titleLabel = CreateLabel(title, 36.0f);
    titleLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    titleLabel->setAlignment(Qt::AlignHCenter);
    
    mChart = new QChart();
    mChart->legend()->hide();
    mChart->setContentsMargins(0, 0, 0, 0);
    mChartView = new QChartView(mChart);
    mChartView->setRenderHint(QPainter::Antialiasing);
    
    mMainLayout->setAlignment(Qt::AlignTop);
    mMainLayout->setSpacing(20);
    mMainLayout->addWidget(titleLabel);
    
    addInputs();
    addOutputs();
    
    setCalculateButtonCallback([this] {
        if (!validate()) return;
        
        QLocale locale = QLocale::system();
        
        std::string expr = mExpressionEdit->text().toStdString();
        double a = locale.toDouble(mLowerBoundXEdit->text());
        double b = locale.toDouble(mUpperBoundXEdit->text());
        double c = locale.toDouble(mLowerBoundYEdit->text());
        double d = locale.toDouble(mUpperBoundYEdit->text());
        int nx = mStepsXEdit->text().toInt();
        int ny = mStepsYEdit->text().toInt();
        
        auto result = calculate(a, b, c, d, nx, ny, expr);
        
        if (result.has_value()) {
            setResult(QString("Результат: %1\nШагов по x: %2, по y: %3")
                      .arg(result->value)
                      .arg(result->steps_x)
                      .arg(result->steps_y));
        } else {
            setError("Ошибка вычисления");
        }
    });
}

void MultipleIntegralBase::addInputs() {
    QDoubleValidator* doubleValidator = new QDoubleValidator();
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    
    QIntValidator* intValidator = new QIntValidator(1, 10000);
    
    // Функция
    VBoxWidget* exprContainer = new VBoxWidget();
    QLabel* exprLabel = CreateLabel("Функция f(x,y):", 12.0f);
    mExpressionEdit = new QLineEdit();
    mExpressionEdit->setPlaceholderText("например: sin(x+y)");
    exprContainer->addWidget(exprLabel);
    exprContainer->addWidget(mExpressionEdit);
    exprContainer->setSpacing(5);
    
    // Пределы по x
    HBoxWidget* xBoundsContainer = new HBoxWidget();
    VBoxWidget* lowerXContainer = new VBoxWidget();
    QLabel* lowerXLabel = CreateLabel("x от:", 12.0f);
    mLowerBoundXEdit = new QLineEdit();
    mLowerBoundXEdit->setValidator(doubleValidator);
    lowerXContainer->addWidget(lowerXLabel);
    lowerXContainer->addWidget(mLowerBoundXEdit);
    
    VBoxWidget* upperXContainer = new VBoxWidget();
    QLabel* upperXLabel = CreateLabel("x до:", 12.0f);
    mUpperBoundXEdit = new QLineEdit();
    mUpperBoundXEdit->setValidator(doubleValidator);
    upperXContainer->addWidget(upperXLabel);
    upperXContainer->addWidget(mUpperBoundXEdit);
    
    xBoundsContainer->addWidget(lowerXContainer);
    xBoundsContainer->addWidget(upperXContainer);
    xBoundsContainer->setSpacing(20);
    
    // Пределы по y
    HBoxWidget* yBoundsContainer = new HBoxWidget();
    VBoxWidget* lowerYContainer = new VBoxWidget();
    QLabel* lowerYLabel = CreateLabel("y от:", 12.0f);
    mLowerBoundYEdit = new QLineEdit();
    mLowerBoundYEdit->setValidator(doubleValidator);
    lowerYContainer->addWidget(lowerYLabel);
    lowerYContainer->addWidget(mLowerBoundYEdit);
    
    VBoxWidget* upperYContainer = new VBoxWidget();
    QLabel* upperYLabel = CreateLabel("y до:", 12.0f);
    mUpperBoundYEdit = new QLineEdit();
    mUpperBoundYEdit->setValidator(doubleValidator);
    upperYContainer->addWidget(upperYLabel);
    upperYContainer->addWidget(mUpperBoundYEdit);
    
    yBoundsContainer->addWidget(lowerYContainer);
    yBoundsContainer->addWidget(upperYContainer);
    yBoundsContainer->setSpacing(20);
    
    // Шаги
    HBoxWidget* stepsContainer = new HBoxWidget();
    VBoxWidget* stepsXContainer = new VBoxWidget();
    QLabel* stepsXLabel = CreateLabel("Шагов по x (четное):", 12.0f);
    mStepsXEdit = new QLineEdit();
    mStepsXEdit->setValidator(intValidator);
    stepsXContainer->addWidget(stepsXLabel);
    stepsXContainer->addWidget(mStepsXEdit);
    
    VBoxWidget* stepsYContainer = new VBoxWidget();
    QLabel* stepsYLabel = CreateLabel("Шагов по y (четное):", 12.0f);
    mStepsYEdit = new QLineEdit();
    mStepsYEdit->setValidator(intValidator);
    stepsYContainer->addWidget(stepsYLabel);
    stepsYContainer->addWidget(mStepsYEdit);
    
    stepsContainer->addWidget(stepsXContainer);
    stepsContainer->addWidget(stepsYContainer);
    stepsContainer->setSpacing(20);
    
    mMainLayout->addWidget(exprContainer);
    mMainLayout->addWidget(xBoundsContainer);
    mMainLayout->addWidget(yBoundsContainer);
    mMainLayout->addWidget(stepsContainer);
}

void MultipleIntegralBase::addOutputs() {
    mMainLayout->addWidget(mCalculateButton);
    mMainLayout->addWidget(mResultLabel);
    mMainLayout->addWidget(mChartView);
}

void MultipleIntegralBase::setCalculateButtonCallback(std::function<void()> callback) {
    QObject::connect(mCalculateButton, &QPushButton::clicked, callback);
}

void MultipleIntegralBase::setError(const QString& errorText) {
    QPalette palette = mResultLabel->palette();
    palette.setColor(mResultLabel->foregroundRole(), Qt::red);
    mResultLabel->setPalette(palette);
    mResultLabel->setText(errorText);
}

void MultipleIntegralBase::setResult(const QString& resultText) {
    QPalette palette = mResultLabel->palette();
    palette.setColor(mResultLabel->foregroundRole(), Qt::black);
    mResultLabel->setPalette(palette);
    mResultLabel->setText(resultText);
}

bool MultipleIntegralBase::validate() {
    if (mExpressionEdit->text().isEmpty()) {
        setError("Ошибка: введите функцию f(x,y)");
        return false;
    }
    if (mLowerBoundXEdit->text().isEmpty() || mUpperBoundXEdit->text().isEmpty()) {
        setError("Ошибка: введите пределы по x");
        return false;
    }
    if (mLowerBoundYEdit->text().isEmpty() || mUpperBoundYEdit->text().isEmpty()) {
        setError("Ошибка: введите пределы по y");
        return false;
    }
    if (mStepsXEdit->text().isEmpty() || mStepsYEdit->text().isEmpty()) {
        setError("Ошибка: введите количество шагов");
        return false;
    }
    
    int nx = mStepsXEdit->text().toInt();
    int ny = mStepsYEdit->text().toInt();
    
    if (nx <= 0 || ny <= 0) {
        setError("Ошибка: количество шагов должно быть больше 0");
        return false;
    }
    
    if (nx % 2 != 0 || ny % 2 != 0) {
        setError("Предупреждение: количество шагов желательно делать четным для метода Симпсона");
    }
    
    return true;
}