#include <QDoubleValidator>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QLineSeries>
#include <QChart>
#include <QChartView>
#include <QValueAxis>
#include <QLineEdit>

#include "base_tab.hpp"
#include "../widgets/chartview.hpp"
#include "../widgets/hboxwidget.hpp"
#include "../utils.hpp"
#include "../../exprtk.hpp"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    using namespace QtCharts;
#endif

void MethodTab::setupUi(const QString& title) {
    QDoubleValidator* boundValidator = new QDoubleValidator();
    QIntValidator* stepsValidator = new QIntValidator(1, std::numeric_limits<int>::max());

    HBoxWidget* lowerBoundContainer = new HBoxWidget();
    QLabel* lowerBoundLabel = CreateLabel("Нижний предел:", 14.0f);
    mLowerBoundEdit = new QLineEdit();
    mLowerBoundEdit->setValidator(boundValidator);
    lowerBoundContainer->addWidget(lowerBoundLabel);
    lowerBoundContainer->addWidget(mLowerBoundEdit);
    lowerBoundContainer->setSpacing(10);

    HBoxWidget* upperBoundContainer = new HBoxWidget();
    QLabel* upperBoundLabel = CreateLabel("Верхний предел:", 14.0f);
    mUpperBoundEdit = new QLineEdit();
    mUpperBoundEdit->setValidator(boundValidator);
    upperBoundContainer->addWidget(upperBoundLabel);
    upperBoundContainer->addWidget(mUpperBoundEdit);
    upperBoundContainer->setSpacing(10);

    HBoxWidget* stepsAmountContainer = new HBoxWidget();
    QLabel* stepsAmountLabel = CreateLabel("Кол-во шагов:", 14.0f);
    mStepsAmountEdit = new QLineEdit();
    mStepsAmountEdit->setValidator(stepsValidator);
    stepsAmountContainer->addWidget(stepsAmountLabel);
    stepsAmountContainer->addWidget(mStepsAmountEdit);
    stepsAmountContainer->setSpacing(10);

    HBoxWidget* expressionContainer = new HBoxWidget();
    QLabel* expressionLabel = CreateLabel("Подинтегральная функция:", 14.0f);
    mExpressionEdit = new QLineEdit();
    expressionContainer->addWidget(expressionLabel);
    expressionContainer->addWidget(mExpressionEdit);
    expressionContainer->setSpacing(10);

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

    QVBoxLayout* controlsLayout = new QVBoxLayout();
    controlsLayout->setAlignment(Qt::AlignTop);
    controlsLayout->setSpacing(10);
    controlsLayout->addWidget(titleLabel);
    controlsLayout->addWidget(expressionContainer);
    controlsLayout->addWidget(lowerBoundContainer);
    controlsLayout->addWidget(upperBoundContainer);
    controlsLayout->addWidget(stepsAmountContainer);
    controlsLayout->addWidget(mCalculateButton);
    controlsLayout->addWidget(mResultLabel);
    controlsLayout->addWidget(mChartView);

    QWidget* controlsWidget = new QWidget();
    controlsWidget->setLayout(controlsLayout);

    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addWidget(controlsWidget);

    QObject::connect(mCalculateButton, &QPushButton::clicked, this, [this] {
        QPalette palette = mResultLabel->palette();
        palette.setColor(mResultLabel->foregroundRole(), Qt::black);

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
        
        if (errorText.has_value()) {
            palette.setColor(mResultLabel->foregroundRole(), Qt::red);
            mResultLabel->setPalette(palette);
            mResultLabel->setText(errorText.value());
            return;
        }

        const double a = mLowerBoundEdit->text().toDouble();
        const double b = mUpperBoundEdit->text().toDouble();
        const std::string expr = mExpressionEdit->text().toStdString();

        double result = 0.0;
        const bool success = calculate(a, b, n, expr, result);

        if (success) {
            palette.setColor(mResultLabel->foregroundRole(), Qt::black);
            mResultLabel->setText(QString("Результат: %1").arg(result));
        } else {
            mResultLabel->setText("Ошибка.");
        }

        mResultLabel->setPalette(palette);
    });

    setLayout(mainLayout);
}

void MethodTab::setup_axis_lines(double x_min, double x_max, double y_min, double y_max)  {
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

void MethodTab::plot_function(double a, double b, exprtk::expression<double>& expression, double& min, double& max) {
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
