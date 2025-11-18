#include <QDoubleValidator>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QLineSeries>
#include <QChart>
#include <QChartView>
#include <QValueAxis>
#include <QLineEdit>
#include <QStackedWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <qlineedit.h>
#include <qsizepolicy.h>

#include "base.hpp"
#include "../../widgets/chartview.hpp"
#include "../../widgets/hboxwidget.hpp"
#include "../../widgets/vboxwidget.hpp"
#include "../../utils.hpp"
#include "../../../exprtk.hpp"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    using namespace QtCharts;
#endif

DiffMethodPageBase::DiffMethodPageBase(const QString& title, QWidget* parent) : QScrollArea(parent) {
    setupUi(title);
    setCalculateButtonCallback([this] {
        if (!validate()) return;
        
        QLocale locale = QLocale::system();

        std::string expr = mExpressionEdit->text().toStdString();
        double a = locale.toDouble(mLowerBoundEdit->text());
        double b = locale.toDouble(mUpperBoundEdit->text());
        char dependentVar = mDependentVarEdit->text().at(0).toLatin1();
        char independentVar = mIndependentVarEdit->text().at(0).toLatin1();

        double startValue = locale.toDouble(mStartValueEdit->text());
        
        int steps = mStepsAmountEdit->text().toInt();
        auto result = calculateWithFixedStep(a, startValue, b, steps, dependentVar, independentVar, expr);

        if (result.has_value()) {
            auto& start = result->data.front();
            auto& end = result->data.back();

            double newMinX = std::min(start.first, mMinX);
            double newMaxX = std::max(end.first, mMaxX);

            double newMinY = std::min(start.second, mMinY);
            double newMaxY = std::max(end.second, mMaxY);

            if (newMinX != mMinX || newMaxX != mMaxX) {
                setup_x_axis_line(newMinX, newMaxX);
            }

            if (newMinY != mMinY || newMaxY != mMaxY) {
                setup_y_axis_line(newMinY, newMaxY);
            }

            mMinX = newMinX;
            mMinY = newMinY;
            mMaxX = newMaxX;
            mMaxY = newMaxY;

            mModel.reset(new QStandardItemModel(result->data.size(), 2));
            mModel->setHorizontalHeaderLabels({"x", "y"});

            QLineSeries* series = new QLineSeries();
            series->setUseOpenGL(true);
            series->setName(QString("%1 шагов").arg(steps));

            for (int row = 0; row < result->data.size(); ++row) {
                double x = result->data[row].first;
                double y = result->data[row].second;

                mModel->setItem(row, 0, new QStandardItem(QString::number(x)));
                mModel->setItem(row, 1, new QStandardItem(QString::number(y)));

                series->append(QPointF(x, y));
            }

            mChart->addSeries(series);

            series->attachAxis(mAxisX);
            series->attachAxis(mAxisY);

            mTable->setModel(mModel.get());
            mTable->horizontalHeader()->setStretchLastSection(true);
            mTable->horizontalHeader()->setDefaultAlignment(Qt::AlignmentFlag::AlignLeft);

        } else {
            setError(QString("Ошибка: %1.").arg(result.error()));
        }
    });
}

void DiffMethodPageBase::setupUi(const QString& title) {
    mMainLayout = new QVBoxLayout();
    QWidget* contents = new QWidget(this);
    contents->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    contents->setLayout(mMainLayout);
    setWidget(contents);
    setWidgetResizable(true);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    mCalculateButton = new QPushButton("Вычислить");

    mResultLabel = new QLabel();
    SetFontSize(mResultLabel, 18.0f);

    QLabel* titleLabel = CreateLabel(title, 36.0f);
    titleLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    titleLabel->setAlignment(Qt::AlignHCenter);

    mChart = new QChart();
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
    mChartView->setMinimumHeight(500);

    mTable = new QTableView(contents);
    mTable->setMinimumHeight(400);

    mMainLayout->setAlignment(Qt::AlignTop);
    mMainLayout->setSpacing(20);
    mMainLayout->addWidget(titleLabel);
}

void DiffMethodPageBase::addOutputs() {
    QPushButton* clearButton = new QPushButton("Очистить");
    
    QObject::connect(clearButton, &QPushButton::clicked, [this]() {
        mChart->removeAllSeries();
        mMinX = 0.0;
        mMinY = 0.0;
        mMaxX = 0.0;
        mMaxY = 0.0;
    });

    mMainLayout->addWidget(mCalculateButton);
    mMainLayout->addWidget(mResultLabel);
    mMainLayout->addWidget(mChartView);
    mMainLayout->addWidget(clearButton);
    mMainLayout->addWidget(mTable);
}

void DiffMethodPageBase::addBaseInputs() {
    QDoubleValidator* doubleValidator = new QDoubleValidator();
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);

    HBoxWidget* lowerBoundContainer = new HBoxWidget();
    mLowerBoundEdit = new QLineEdit("0");
    mLowerBoundEdit->setValidator(doubleValidator);
    lowerBoundContainer->addWidget(CreateLabel("Начало:", 12.0f));
    lowerBoundContainer->addWidget(mLowerBoundEdit);
    lowerBoundContainer->setSpacing(5);

    HBoxWidget* upperBoundContainer = new HBoxWidget();
    mUpperBoundEdit = new QLineEdit("1");
    mUpperBoundEdit->setValidator(doubleValidator);
    upperBoundContainer->addWidget(CreateLabel("Конец:", 12.0f));
    upperBoundContainer->addWidget(mUpperBoundEdit);
    upperBoundContainer->setSpacing(5);

    mExpressionEdit = new QLineEdit();

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

    mStartValueEdit = new QLineEdit("0");
    mStartValueEdit->setValidator(doubleValidator);

    mStepEdit = new QLineEdit();
    mStepEdit->setValidator(doubleValidator);

    QLabel* startValueLabel = CreateLabel("y(0) =", 12.0f);

    QObject::connect(mDependentVarEdit, &QLineEdit::textEdited, [this, startValueLabel](const QString& text) {
        if (!text.isEmpty()) {
            startValueLabel->setText(QString("%1(%2) =").arg(text).arg(mStartValueEdit->text()));
        }
    });

    QObject::connect(mLowerBoundEdit, &QLineEdit::textEdited, [this, startValueLabel](const QString& text) {
        if (!text.isEmpty()) {
            startValueLabel->setText(QString("%1(%2) =").arg(mDependentVarEdit->text()).arg(text));
        }
    });

    HBoxWidget* startYContainer = new HBoxWidget();
    startYContainer->setSpacing(5);
    startYContainer->addWidget(startValueLabel);
    startYContainer->addWidget(mStartValueEdit);

    HBoxWidget* stepContainer = new HBoxWidget();
    stepContainer->setSpacing(5);
    stepContainer->addWidget(CreateLabel("Размер шага:", 12.0f));
    stepContainer->addWidget(mStepEdit);

    mMainLayout->addWidget(equationContainer);
    mMainLayout->addWidget(lowerBoundContainer);
    mMainLayout->addWidget(upperBoundContainer);
    mMainLayout->addWidget(startYContainer);
    mMainLayout->addWidget(createStepsInputContainer());
    // mMainLayout->addWidget(stepContainer);
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

void DiffMethodPageBase::setup_x_axis_line(double x_min, double x_max) {
    QPen pen = QPen();
    pen.setColor(QColor(Qt::black));
    pen.setWidth(1);

    if (mChart->series().contains(mAxisXSeries)) {
        mChart->removeSeries(mAxisXSeries);
    }

    mAxisXSeries = new QLineSeries();
    mAxisXSeries->append(x_min, 0);
    mAxisXSeries->append(x_max, 0);
    mAxisXSeries->setPen(pen);
    mAxisXSeries->setUseOpenGL(true);

    mChart->addSeries(mAxisXSeries);

    mAxisXSeries->attachAxis(mAxisX);
    mAxisXSeries->attachAxis(mAxisY);
}


void DiffMethodPageBase::setup_y_axis_line(double y_min, double y_max) {
    QPen pen = QPen();
    pen.setColor(QColor(Qt::black));
    pen.setWidth(1);
    
    if (mChart->series().contains(mAxisYSeries)) {
        mChart->removeSeries(mAxisYSeries);
    }

    mAxisYSeries = new QLineSeries();
    mAxisYSeries->append(0, y_min);
    mAxisYSeries->append(0, y_max);
    mAxisYSeries->setPen(pen);
    mAxisYSeries->setUseOpenGL(true);

    mChart->addSeries(mAxisYSeries);

    mAxisYSeries->attachAxis(mAxisX);
    mAxisYSeries->attachAxis(mAxisY);
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

    setup_x_axis_line(a, b);
    setup_y_axis_line(min, max);

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
    } /* else if (mStepEdit->text().isEmpty()) {
        errorText = "Ошибка: введите размер шага.";
    } */ else if (mLowerBoundEdit->text().isEmpty()) {
        errorText = "Ошибка: введите начало.";
    } else if (mUpperBoundEdit->text().isEmpty()) {
        errorText = "Ошибка: введите конец.";
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