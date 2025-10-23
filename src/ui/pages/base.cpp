#include <QDoubleValidator>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QLineSeries>
#include <QChart>
#include <QChartView>
#include <QValueAxis>
#include <QLineEdit>
#include <qvalidator.h>

#include "base.hpp"
#include "../widgets/chartview.hpp"
#include "../widgets/hboxwidget.hpp"
#include "../utils.hpp"
#include "../../exprtk.hpp"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    using namespace QtCharts;
#endif

void MethodPageBase::setupUi(const QString& title) {
    mCalculateButton = new QPushButton("Вычислить");

    mResultLabel = new QLabel();
    SetFont(mResultLabel, 18.0f);

    QLabel* titleLabel = CreateLabel(title, 36.0f);
    titleLabel->setAlignment(Qt::AlignHCenter);

    mChart = new QChart();
    mChart->legend()->hide();
    mChart->setContentsMargins(0, 0, 0, 0);

    mAxisX = new QValueAxis();
    mAxisX->setLabelFormat("%i");
    mAxisX->setTickAnchor(0.0);
    mChart->addAxis(mAxisX, Qt::AlignBottom);
    
    mAxisY = new QValueAxis();
    mAxisY->setLabelFormat("%.2f");
    mAxisY->setMin(0);
    mChart->addAxis(mAxisY, Qt::AlignLeft);

    mChartView = new ChartView(mChart);
    mChartView->setRenderHint(QPainter::Antialiasing);
    mChartView->setRenderHint(QPainter::SmoothPixmapTransform);
    mChartView->setContentsMargins(0, 0, 0, 0);

    mMainLayout = new QVBoxLayout();
    mMainLayout->setAlignment(Qt::AlignTop);
    mMainLayout->setSpacing(10);
    mMainLayout->addWidget(titleLabel);

    setLayout(mMainLayout);
}

void MethodPageBase::addBaseInputs() {
    QDoubleValidator* validator = new QDoubleValidator();
    validator->setNotation(QDoubleValidator::StandardNotation);

    HBoxWidget* lowerBoundContainer = new HBoxWidget();
    QLabel* lowerBoundLabel = CreateLabel("Нижний предел:", 14.0f);
    mLowerBoundEdit = new QLineEdit();
    mLowerBoundEdit->setValidator(validator);
    lowerBoundContainer->addWidget(lowerBoundLabel);
    lowerBoundContainer->addWidget(mLowerBoundEdit);
    lowerBoundContainer->setSpacing(10);

    HBoxWidget* upperBoundContainer = new HBoxWidget();
    QLabel* upperBoundLabel = CreateLabel("Верхний предел:", 14.0f);
    mUpperBoundEdit = new QLineEdit();
    mUpperBoundEdit->setValidator(validator);
    upperBoundContainer->addWidget(upperBoundLabel);
    upperBoundContainer->addWidget(mUpperBoundEdit);
    upperBoundContainer->setSpacing(10);

    HBoxWidget* expressionContainer = new HBoxWidget();
    QLabel* expressionLabel = CreateLabel("Подинтегральная функция:", 14.0f);
    mExpressionEdit = new QLineEdit();
    expressionContainer->addWidget(expressionLabel);
    expressionContainer->addWidget(mExpressionEdit);
    expressionContainer->setSpacing(10);

    mMainLayout->addWidget(expressionContainer);
    mMainLayout->addWidget(lowerBoundContainer);
    mMainLayout->addWidget(upperBoundContainer);
}

void MethodPageBase::addOutputs() {
    mMainLayout->addWidget(mResultLabel);
    mMainLayout->addWidget(mChartView);
}

void MethodPageBase::addCalculateButton() {
    mMainLayout->addWidget(mCalculateButton);
}

void MethodPageBase::setCalculateButtonCallback(std::function<void()> callback) {
    QObject::connect(mCalculateButton, &QPushButton::clicked, callback);
}

void MethodPageBase::setError(const QString& errorText) {
    QPalette palette = mResultLabel->palette();
    palette.setColor(mResultLabel->foregroundRole(), Qt::red);
    mResultLabel->setPalette(palette);
    mResultLabel->setText(errorText);
}

void MethodPageBase::setResult(const QString& resultText) {
    QPalette palette = mResultLabel->palette();
    palette.setColor(mResultLabel->foregroundRole(), Qt::black);
    mResultLabel->setPalette(palette);
    mResultLabel->setText(resultText);
}

void MethodPageBase::setup_axis_lines(double x_min, double x_max, double y_min, double y_max)  {
    QPen pen = QPen();
    pen.setColor(QColor(Qt::black));
    pen.setWidth(1);

    QLineSeries* axisXSeries = new QLineSeries();
    axisXSeries->append(x_min, 0);
    axisXSeries->append(x_max, 0);
    axisXSeries->setPen(pen);
    axisXSeries->setUseOpenGL(true);

    QLineSeries* axisYSeries = new QLineSeries();
    axisYSeries->append(0, y_min);
    axisYSeries->append(0, y_max);
    axisYSeries->setPen(pen);
    axisYSeries->setUseOpenGL(true);

    mChart->addSeries(axisXSeries);
    mChart->addSeries(axisYSeries);

    axisXSeries->attachAxis(mAxisX);
    axisXSeries->attachAxis(mAxisY);
    axisYSeries->attachAxis(mAxisX);
    axisYSeries->attachAxis(mAxisY);
}

void MethodPageBase::plot_function(double a, double b, exprtk::expression<double>& expression, double& min, double& max) {
    double& x = expression.get_symbol_table().get_variable("x")->ref();
    max = std::numeric_limits<qreal>::min();
    min = 0;
    
    QLineSeries* lineSeries = new QLineSeries();
    lineSeries->setUseOpenGL(true);

    for (x = a; x <= b; x += 0.001) {
        const qreal value = expression.value();
        if (value > max)
            max = value;
        if (value < min)
            min = value;
        lineSeries->append(x, value);
    }

    mChart->addSeries(lineSeries);
    lineSeries->attachAxis(mAxisX);
    lineSeries->attachAxis(mAxisY);
}

void FixedStepsMethodBase::addStepsInput() {
    HBoxWidget* container = new HBoxWidget();

    QIntValidator* validator = new QIntValidator(1, std::numeric_limits<int>::max());

    QLabel* label = CreateLabel("Кол-во шагов:", 14.0f);
    mStepsAmountEdit = new QLineEdit();
    mStepsAmountEdit->setValidator(validator);
    container->addWidget(label);
    container->addWidget(mStepsAmountEdit);
    container->setSpacing(10);

    mMainLayout->addWidget(container);
}

bool FixedStepsMethodBase::validate() {
    std::optional<const char*> errorText;

    int n = 0;

    if (mExpressionEdit->text().isEmpty()) {
        errorText = "Ошибка: введите подинтегральную функцию.";
    } else if (mLowerBoundEdit->text().isEmpty()) {
        errorText = "Ошибка: введите нижний предел.";
    } else if (mUpperBoundEdit->text().isEmpty()) {
        errorText = "Ошибка: введите верхний предел.";
    } else if (mStepsAmountEdit->text().isEmpty()) {
        errorText = "Ошибка: введите кол-во шагов.";
    } else {
        n = mStepsAmountEdit->text().toInt();
        if (n <= 0) {
            errorText = "Ошибка: кол-во шагов должно быть больше нуля.";
        }
    }

    const bool hasError = errorText.has_value();
    
    if (hasError) {
        setError(errorText.value());
    }

    return !hasError;
}

void VariableStepMethodBase::addEpsilonInput() {
    HBoxWidget* container = new HBoxWidget();

    QDoubleValidator* validator = new QDoubleValidator();
    validator->setNotation(QDoubleValidator::StandardNotation);

    QLabel* label = CreateLabel("Эпсилон:", 14.0f);
    mEpsilonEdit = new QLineEdit();
    mEpsilonEdit->setValidator(validator);
    container->addWidget(label);
    container->addWidget(mEpsilonEdit);
    container->setSpacing(10);

    mMainLayout->addWidget(container);
}

bool VariableStepMethodBase::validate() {
    std::optional<const char*> errorText;

    int n = 0;

    if (mExpressionEdit->text().isEmpty()) {
        errorText = "Ошибка: введите подинтегральную функцию.";
    } else if (mLowerBoundEdit->text().isEmpty()) {
        errorText = "Ошибка: введите нижний предел.";
    } else if (mUpperBoundEdit->text().isEmpty()) {
        errorText = "Ошибка: введите верхний предел.";
    } else if (mEpsilonEdit->text().isEmpty()) {
        errorText = "Ошибка: введите кол-во шагов.";
    }

    const bool hasError = errorText.has_value();
    
    if (hasError) {
        setError(errorText.value());
    }

    return !hasError;
}