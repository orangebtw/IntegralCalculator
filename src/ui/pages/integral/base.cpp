#include <QDoubleValidator>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QLineSeries>
#include <QChart>
#include <QChartView>
#include <QValueAxis>
#include <QLineEdit>
#include <QStackedWidget>

#include "base.hpp"
#include "../../widgets/chartview.hpp"
#include "../../widgets/hboxwidget.hpp"
#include "../../widgets/vboxwidget.hpp"
#include "../../widgets/resizablestackedwidget.hpp"
#include "../../utils.hpp"
#include "../../../exprtk.hpp"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    using namespace QtCharts;
#endif

void IntegralMethodPageBase::setupUi(const QString& title) {
    mMainLayout = new QVBoxLayout();
    setLayout(mMainLayout);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    mCalculateButton = new QPushButton("Вычислить");

    mResultLabel = new QLabel();
    SetFontSize(mResultLabel, 18.0f);

    QLabel* titleLabel = CreateLabel(title, 36.0f);
    titleLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    titleLabel->setAlignment(Qt::AlignHCenter);
    titleLabel->setWordWrap(true);

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

    mMainLayout->setAlignment(Qt::AlignTop);
    mMainLayout->setSpacing(20);
    mMainLayout->addWidget(titleLabel);
}

void IntegralMethodPageBase::addOutputs() {
    mMainLayout->addWidget(mCalculateButton);
    mMainLayout->addWidget(mResultLabel);
    mMainLayout->addWidget(mChartView);
}

void IntegralMethodPageBase::addBaseInputs() {
    QDoubleValidator* validator = new QDoubleValidator();
    validator->setNotation(QDoubleValidator::StandardNotation);

    VBoxWidget* lowerBoundContainer = new VBoxWidget();
    QLabel* lowerBoundLabel = CreateLabel("Нижний предел:", 12.0f);
    mLowerBoundEdit = new QLineEdit();
    mLowerBoundEdit->setValidator(validator);
    lowerBoundContainer->addWidget(lowerBoundLabel);
    lowerBoundContainer->addWidget(mLowerBoundEdit);
    lowerBoundContainer->setSpacing(5);

    VBoxWidget* upperBoundContainer = new VBoxWidget();
    QLabel* upperBoundLabel = CreateLabel("Верхний предел:", 12.0f);
    mUpperBoundEdit = new QLineEdit();
    mUpperBoundEdit->setValidator(validator);
    upperBoundContainer->addWidget(upperBoundLabel);
    upperBoundContainer->addWidget(mUpperBoundEdit);
    upperBoundContainer->setSpacing(5);

    VBoxWidget* expressionContainer = new VBoxWidget();
    QLabel* expressionLabel = CreateLabel("Подинтегральная функция:", 12.0f);
    mExpressionEdit = new QLineEdit();
    expressionContainer->addWidget(expressionLabel);
    expressionContainer->addWidget(mExpressionEdit);
    expressionContainer->setSpacing(5);

    HBoxWidget* variableStepContainer = new HBoxWidget();
    QLabel* variableStepCheckLabel = CreateLabel("Переменный шаг:", 12.0f);
    mVariableStepCheck = new QCheckBox();
    mVariableStepCheck->setTristate(false);
    variableStepContainer->addWidget(variableStepCheckLabel);
    variableStepContainer->addWidget(mVariableStepCheck);
    variableStepContainer->setSpacing(10);
    variableStepContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    mMainLayout->addWidget(expressionContainer);
    mMainLayout->addWidget(lowerBoundContainer);
    mMainLayout->addWidget(upperBoundContainer);
    mMainLayout->addWidget(variableStepContainer);
}

QWidget* IntegralMethodPageBase::createEpsilonInputContainer() {
    VBoxWidget* epsilonInputContainer = new VBoxWidget();

    QDoubleValidator* validator = new QDoubleValidator();
    validator->setNotation(QDoubleValidator::StandardNotation);

    QLabel* epsilonLabel = CreateLabel("Эпсилон:", 12.0f);
    mEpsilonEdit = new QLineEdit();
    mEpsilonEdit->setValidator(validator);
    epsilonInputContainer->addWidget(epsilonLabel);
    epsilonInputContainer->addWidget(mEpsilonEdit);
    epsilonInputContainer->setContentMargins(0, 0, 0, 0);
    epsilonInputContainer->setSpacing(5);

    return epsilonInputContainer;
}

QWidget* IntegralMethodPageBase::createStepsInputContainer() {
    VBoxWidget* stepsContainer = new VBoxWidget();

    QIntValidator* validator = new QIntValidator(1, std::numeric_limits<int>::max());

    QLabel* stepsLabel = CreateLabel("Кол-во шагов:", 12.0f);
    mStepsAmountEdit = new QLineEdit();
    mStepsAmountEdit->setValidator(validator);
    stepsContainer->addWidget(stepsLabel);
    stepsContainer->addWidget(mStepsAmountEdit);
    stepsContainer->setContentMargins(0, 0, 0, 0);
    stepsContainer->setSpacing(5);

    return stepsContainer;
}

void IntegralMethodPageBase::addStepsInput(QWidget* stepsContainer, QWidget* epsilonContainer) {
    ResizableStackedWidget* stackedWidget = new ResizableStackedWidget();
    stackedWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    stackedWidget->addWidget(stepsContainer);
    stackedWidget->addWidget(epsilonContainer);

    mMainLayout->addWidget(stackedWidget);

    QObject::connect(mVariableStepCheck, &QCheckBox::checkStateChanged, [this, stackedWidget](Qt::CheckState state) {
        const bool checked = (state == Qt::Checked);
        stackedWidget->setCurrentIndex(checked);
    });
}

void IntegralMethodPageBase::setCalculateButtonCallback(std::function<void()> callback) {
    QObject::connect(mCalculateButton, &QPushButton::clicked, callback);
}

void IntegralMethodPageBase::setError(const QString& errorText) {
    QPalette palette = mResultLabel->palette();
    palette.setColor(mResultLabel->foregroundRole(), Qt::red);
    mResultLabel->setPalette(palette);
    mResultLabel->setText(errorText);
}

void IntegralMethodPageBase::setResult(const QString& resultText) {
    QPalette palette = mResultLabel->palette();
    palette.setColor(mResultLabel->foregroundRole(), Qt::black);
    mResultLabel->setPalette(palette);
    mResultLabel->setText(resultText);
}

void IntegralMethodPageBase::setup_axis_lines(double x_min, double x_max, double y_min, double y_max)  {
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

void IntegralMethodPageBase::plot_function(double a, double b, exprtk::expression<double>& expression) {
    double& x = expression.get_symbol_table().get_variable("x")->ref();
    double max = std::numeric_limits<qreal>::min();
    double min = 0;
    
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

    setup_axis_lines(a, b, min, max);

    mAxisX->setRange(a, b);
    mAxisY->setRange(min, max);
}

bool IntegralMethodPageBase::validate() {
    std::optional<const char*> errorText;

    int n = 0;

    if (mExpressionEdit->text().isEmpty()) {
        errorText = "Ошибка: введите подинтегральную функцию.";
    } else if (mLowerBoundEdit->text().isEmpty()) {
        errorText = "Ошибка: введите нижний предел.";
    } else if (mUpperBoundEdit->text().isEmpty()) {
        errorText = "Ошибка: введите верхний предел.";
    } else {
        const bool isVariableStep = (mVariableStepCheck->checkState() == Qt::Checked);

        if (isVariableStep) {
            if (mEpsilonEdit->text().isEmpty()) {
                errorText = "Ошибка: введите эпсилон.";
            }
        } else {
            if (mStepsAmountEdit->text().isEmpty()) {
                errorText = "Ошибка: введите кол-во шагов.";
            } else {
                n = mStepsAmountEdit->text().toInt();
                if (n <= 0) {
                    errorText = "Ошибка: кол-во шагов должно быть больше нуля.";
                }
            }
        }
    }

    const bool hasError = errorText.has_value();
    
    if (hasError) {
        setError(errorText.value());
    }

    return !hasError;
}