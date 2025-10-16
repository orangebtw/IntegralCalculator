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
#include "../../exprtk.hpp"

using namespace QtCharts;

void MethodTab::setupUi(const QString& title) {
    QDoubleValidator* boundValidator = new QDoubleValidator();
    QIntValidator* stepsValidator = new QIntValidator(1, std::numeric_limits<int>::max());

    mLowerBoundEdit = new QLineEdit();
    mLowerBoundEdit->setPlaceholderText("Нижний предел");
    mLowerBoundEdit->setValidator(boundValidator);

    mUpperBoundEdit = new QLineEdit();
    mUpperBoundEdit->setPlaceholderText("Верхний предел");
    mUpperBoundEdit->setValidator(boundValidator);

    mStepsAmountEdit = new QLineEdit();
    mStepsAmountEdit->setPlaceholderText("Кол-во шагов");
    mStepsAmountEdit->setValidator(stepsValidator);

    mExpressionEdit = new QLineEdit();
    mExpressionEdit->setPlaceholderText("Подинтегральная функция");

    mCalculateButton = new QPushButton("Вычислить");

    mResultLabel = new QLabel();
    mResultLabel->setStyleSheet("QLabel{ font-size: 18px; }");

    QLabel* titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("QLabel{ font-size: 36px; }");
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
    controlsLayout->addWidget(mExpressionEdit);
    controlsLayout->addWidget(mLowerBoundEdit);
    controlsLayout->addWidget(mUpperBoundEdit);
    controlsLayout->addWidget(mStepsAmountEdit);
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
