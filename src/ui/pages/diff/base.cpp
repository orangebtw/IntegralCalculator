#include <QDoubleValidator>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QLineSeries>
#include <QChart>
#include <QChartView>
#include <QValueAxis>
#include <QLineEdit>
#include <QStackedWidget>
#include <qlineedit.h>
#include <qsizepolicy.h>

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

void DiffMethodPageBase::setupUi(const QString& title) {
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

void DiffMethodPageBase::addOutputs() {
    mMainLayout->addWidget(mCalculateButton);
    mMainLayout->addWidget(mResultLabel);
    mMainLayout->addWidget(mChartView);
}

void DiffMethodPageBase::addBaseInputs() {
    QDoubleValidator* validator = new QDoubleValidator();
    validator->setNotation(QDoubleValidator::StandardNotation);

    VBoxWidget* lowerBoundContainer = new VBoxWidget();
    mLowerBoundEdit = new QLineEdit();
    mLowerBoundEdit->setValidator(validator);
    lowerBoundContainer->addWidget(CreateLabel("Нижний предел:", 12.0f));
    lowerBoundContainer->addWidget(mLowerBoundEdit);
    lowerBoundContainer->setSpacing(5);

    VBoxWidget* upperBoundContainer = new VBoxWidget();
    mUpperBoundEdit = new QLineEdit();
    mUpperBoundEdit->setValidator(validator);
    upperBoundContainer->addWidget(CreateLabel("Верхний предел:", 12.0f));
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

    mDependentVarEdit = new QLineEdit("y");
    mDependentVarEdit->setMaxLength(1);
    mDependentVarEdit->setFixedWidth(25);
    mDependentVarEdit->setAlignment(Qt::AlignCenter);
    // mDependentVarEdit->setFrame(false);
    SetFontSize(mDependentVarEdit, 12.0f);
    
    mIndependentVarEdit = new QLineEdit("x");
    mIndependentVarEdit->setMaxLength(1);
    mIndependentVarEdit->setFixedWidth(25);
    mIndependentVarEdit->setAlignment(Qt::AlignCenter);
    // mIndependentVarEdit->setFrame(false);
    SetFontSize(mIndependentVarEdit, 12.0f);

    HBoxWidget* equationContainer = new HBoxWidget();
    equationContainer->setSpacing(10);

    VBoxWidget* lhsContainer = new VBoxWidget();
    lhsContainer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    HBoxWidget* topContainer = new HBoxWidget();
    topContainer->addWidget(CreateLabel("d", 18.0f));
    topContainer->addWidget(mDependentVarEdit);
    
    HBoxWidget* bottomContainer = new HBoxWidget();
    bottomContainer->addWidget(CreateLabel("d", 18.0f));
    bottomContainer->addWidget(mIndependentVarEdit);

    QWidget* line = new QWidget();
    line->setFixedHeight(2);
    line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    line->setStyleSheet("background-color: #000");

    lhsContainer->addWidget(topContainer);
    lhsContainer->addWidget(line);
    lhsContainer->addWidget(bottomContainer);

    equationContainer->addWidget(lhsContainer);
    equationContainer->addWidget(CreateLabel("=", 18.0f));
    equationContainer->addWidget(mExpressionEdit);

    mStartXEdit = new QLineEdit("0");
    mStartXEdit->setValidator(validator);

    mStartYEdit = new QLineEdit("0");
    mStartYEdit->setValidator(validator);

    mStepEdit = new QLineEdit("100");
    mStepEdit->setValidator(validator);

    QLabel* startDependentLabel = CreateLabel("Начальное x:", 12.0f);
    QLabel* startIndependentLabel = CreateLabel("Начальное y:", 12.0f);

    QObject::connect(mDependentVarEdit, &QLineEdit::textEdited, [startDependentLabel](const QString& text) {
        if (!text.isEmpty()) {
            startDependentLabel->setText(QString("Начальное %1:").arg(text));
        }
    });

    QObject::connect(mIndependentVarEdit, &QLineEdit::textEdited, [startIndependentLabel](const QString& text) {
        if (!text.isEmpty()) {
            startIndependentLabel->setText(QString("Начальное %1:").arg(text));
        }
    });

    HBoxWidget* startXContainer = new HBoxWidget();
    startXContainer->setSpacing(5);
    startXContainer->addWidget(startDependentLabel);
    startXContainer->addWidget(mStartXEdit);

    HBoxWidget* startYContainer = new HBoxWidget();
    startYContainer->setSpacing(5);
    startYContainer->addWidget(startIndependentLabel);
    startYContainer->addWidget(mStartYEdit);

    HBoxWidget* stepContainer = new HBoxWidget();
    stepContainer->setSpacing(5);
    stepContainer->addWidget(CreateLabel("Размер шага:", 12.0f));
    stepContainer->addWidget(mStepEdit);

    mMainLayout->addWidget(equationContainer);
    mMainLayout->addWidget(startXContainer);
    mMainLayout->addWidget(startYContainer);
    mMainLayout->addWidget(stepContainer);
    // mMainLayout->addWidget(lowerBoundContainer);
    // mMainLayout->addWidget(upperBoundContainer);
    mMainLayout->addWidget(variableStepContainer);
}

void DiffMethodPageBase::addStepsInput(QWidget* stepsContainer, QWidget* epsilonContainer) {
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

void DiffMethodPageBase::setCalculateButtonCallback(std::function<void()> callback) {
    QObject::connect(mCalculateButton, &QPushButton::clicked, callback);
}

void DiffMethodPageBase::setError(const QString& errorText) {
    QPalette palette = mResultLabel->palette();
    palette.setColor(mResultLabel->foregroundRole(), Qt::red);
    mResultLabel->setPalette(palette);
    mResultLabel->setText(errorText);
}

void DiffMethodPageBase::setResult(const QString& resultText) {
    QPalette palette = mResultLabel->palette();
    palette.setColor(mResultLabel->foregroundRole(), Qt::black);
    mResultLabel->setPalette(palette);
    mResultLabel->setText(resultText);
}

void DiffMethodPageBase::setup_axis_lines(double x_min, double x_max, double y_min, double y_max)  {
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

void DiffMethodPageBase::plot_function(double a, double b, exprtk::expression<double>& expression) {
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

bool DiffMethodPageBase::validate() {
    std::optional<const char*> errorText;

    int n = 0;

    if (mDependentVarEdit->text().isEmpty()) {
        errorText = "Ошибка: введите зависимую переменную.";
    } else if (mIndependentVarEdit->text().isEmpty()) {
        errorText = "Ошибка: введите независимую переменную.";
    } else if (mExpressionEdit->text().isEmpty()) {
        errorText = "Ошибка: введите функцию.";
    } else if (mStepEdit->text().isEmpty()) {
        errorText = "Ошибка: введите размер шага.";
    }
    /* else if (mLowerBoundEdit->text().isEmpty()) {
        errorText = "Ошибка: введите нижний предел.";
    } else if (mUpperBoundEdit->text().isEmpty()) {
        errorText = "Ошибка: введите верхний предел.";
    }  */
    else {
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

QWidget* DiffMethodPageBase::createEpsilonInputContainer() {
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

QWidget* DiffMethodPageBase::createStepsInputContainer() {
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