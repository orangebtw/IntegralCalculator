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
#include <QButtonGroup>
#include <QRadioButton>
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

DiffMethodPageBase::DiffMethodPageBase(bool secondOrder, const QString& title, QWidget* parent) : QScrollArea(parent) {
    setupUi(title, secondOrder);
    setCalculateButtonCallback([this, secondOrder] {
        if (!validate()) return;
        
        QLocale locale = QLocale::system();

        CalculateResult result;

        int steps = mStepsAmountStr.toInt();

        if (!secondOrder) {
            std::string expr = mFirstExpressionStr.toStdString();
            double a = locale.toDouble(mLowerBoundStr);
            double b = locale.toDouble(mUpperBoundStr);
            char dependentVar = mFirstDependentVarStr.at(0).toLatin1();
            char independentVar = mIndependentVar.toLatin1();
    
            double startValue = locale.toDouble(mStartYStr);
            
            result = calculate(a, startValue, b, steps, dependentVar, independentVar, expr);
        } else if (secondOrder) {
            std::string expr1 = mFirstExpressionStr.toStdString();
            std::string expr2 = mSecondExpressionStr.toStdString();

            double x_start = locale.toDouble(mLowerBoundStr);
            double x_end = locale.toDouble(mUpperBoundStr);
            char dependentVar1 = mFirstDependentVarStr.at(0).toLatin1();
            char dependentVar2 = mSecondDependentVarStr.at(0).toLatin1();
            char independentVar = mIndependentVar.toLatin1();
    
            double y0 = locale.toDouble(mStartYStr);
            double dy0 = locale.toDouble(mStartDyStr);
            
            result = calculate2(x_start, y0, dy0, x_end, steps, dependentVar1, dependentVar2, independentVar, expr1, expr2);
        }

        if (result.has_value()) {
            auto& start = result->front();
            auto& end = result->back();

            double newMinX = std::min(std::get<0>(start), mMinX);
            double newMaxX = std::max(std::get<0>(end), mMaxX);

            double newMinY = std::min(std::get<1>(end), mMinY);
            double newMaxY = std::max(std::get<1>(end), mMaxY);
            if (secondOrder) {
                newMinY = std::min(std::get<2>(start), newMinY);
                newMaxY = std::max(std::get<2>(end), newMaxY);
            }

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

            mModel.reset(new QStandardItemModel(result->size(), secondOrder ? 3 : 2));
            if (secondOrder) {
                mModel->setHorizontalHeaderLabels({"x", "y", "y'"});
            } else {
                mModel->setHorizontalHeaderLabels({"x", "y"});
            }

            QLineSeries* series = new QLineSeries();
            series->setUseOpenGL(true);
            series->setName(QString("%1 шагов").arg(steps));

            for (int row = 0; row < result->size(); ++row) {
                double x = std::get<0>(result.value()[row]);
                double y = std::get<1>(result.value()[row]);

                mModel->setItem(row, 0, new QStandardItem(QString::number(x)));
                mModel->setItem(row, 1, new QStandardItem(QString::number(y)));

                series->append(QPointF(x, y));
            }

            if (secondOrder) {
                QLineSeries* series = new QLineSeries();
                series->setUseOpenGL(true);
                series->setName(QString("%1 шагов").arg(steps));

                for (int row = 0; row < result->size(); ++row) {
                    double x = std::get<0>(result.value()[row]);
                    double dy = std::get<2>(result.value()[row]);

                    mModel->setItem(row, 2, new QStandardItem(QString::number(dy)));

                    series->append(QPointF(x, dy));
                }

                mChart->addSeries(series);

                series->attachAxis(mAxisX);
                series->attachAxis(mAxisY);
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

void DiffMethodPageBase::setupUi(const QString& title, bool secondOrder) {
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

    if (secondOrder) {
        mMainLayout->addWidget(createSecondOrderInputs());
    } else {
        mMainLayout->addWidget(createFirstOrderInputs());
    }

    addOutputs();
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

QWidget* DiffMethodPageBase::createFirstOrderInputs() {
    VBoxWidget* container = new VBoxWidget(this);
    container->layout()->setAlignment(Qt::AlignTop);
    container->setSpacing(20);

    QDoubleValidator* doubleValidator = new QDoubleValidator();
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);

    HBoxWidget* lowerBoundContainer = new HBoxWidget();
    QLineEdit* lowerBoundEdit = new QLineEdit(mLowerBoundStr);
    lowerBoundEdit->setValidator(doubleValidator);
    lowerBoundContainer->addWidget(CreateLabel("Начало:", 12.0f));
    lowerBoundContainer->addWidget(lowerBoundEdit);
    lowerBoundContainer->setSpacing(5);
    BindVariable(lowerBoundEdit, &QLineEdit::textChanged, mLowerBoundStr);

    HBoxWidget* upperBoundContainer = new HBoxWidget();
    QLineEdit* upperBoundEdit = new QLineEdit(mUpperBoundStr);
    upperBoundEdit->setValidator(doubleValidator);
    upperBoundContainer->addWidget(CreateLabel("Конец:", 12.0f));
    upperBoundContainer->addWidget(upperBoundEdit);
    upperBoundContainer->setSpacing(5);
    BindVariable(upperBoundEdit, &QLineEdit::textChanged, mUpperBoundStr);

    QLineEdit* firstExpressionEdit = new QLineEdit(mFirstExpressionStr);
    SetFontSize(firstExpressionEdit, 14.0f);
    BindVariable(firstExpressionEdit, &QLineEdit::textChanged, mFirstExpressionStr);

    QLineEdit* firstDependentVarEdit = new QLineEdit(mFirstDependentVarStr);
    firstDependentVarEdit->setMaxLength(1);
    firstDependentVarEdit->setFixedWidth(25);
    firstDependentVarEdit->setAlignment(Qt::AlignCenter);
    // mDependentVarEdit->setFrame(false);
    SetFontSize(firstDependentVarEdit, 12.0f);
    BindVariable(firstDependentVarEdit, &QLineEdit::textChanged, mFirstDependentVarStr);
    
    QLineEdit* independentVarEdit = new QLineEdit(mIndependentVar);
    independentVarEdit->setMaxLength(1);
    independentVarEdit->setFixedWidth(25);
    independentVarEdit->setAlignment(Qt::AlignCenter);
    // independentVarEdit->setFrame(false);
    SetFontSize(independentVarEdit, 12.0f);

    QObject::connect(independentVarEdit, &QLineEdit::textChanged, [this](const QString& text) {
        mIndependentVar = text.at(0);
    });

    HBoxWidget* equationContainer = new HBoxWidget();
    equationContainer->setSpacing(10);

    VBoxWidget* lhsContainer = new VBoxWidget();
    lhsContainer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    HBoxWidget* topContainer = new HBoxWidget();
    topContainer->addWidget(CreateLabel("d", 18.0f));
    topContainer->addWidget(firstDependentVarEdit);
    
    HBoxWidget* bottomContainer = new HBoxWidget();
    bottomContainer->addWidget(CreateLabel("d", 18.0f));
    bottomContainer->addWidget(independentVarEdit);

    QWidget* line = new QWidget();
    line->setFixedHeight(2);
    line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    line->setStyleSheet("background-color: #000");

    lhsContainer->addWidget(topContainer);
    lhsContainer->addWidget(line);
    lhsContainer->addWidget(bottomContainer);

    equationContainer->addWidget(lhsContainer);
    equationContainer->addWidget(CreateLabel("=", 18.0f));
    equationContainer->addWidget(firstExpressionEdit);

    QLineEdit* startYEdit = new QLineEdit(mStartYStr);
    startYEdit->setValidator(doubleValidator);
    BindVariable(startYEdit, &QLineEdit::textChanged, mStartYStr);

    QLabel* startValueLabel = CreateLabel("y(0) =", 12.0f);

    QObject::connect(firstDependentVarEdit, &QLineEdit::textChanged, [startYEdit, startValueLabel](const QString& text) {
        if (!text.isEmpty()) {
            startValueLabel->setText(QString("%1(%2) =").arg(text).arg(startYEdit->text()));
        }
    });

    QObject::connect(lowerBoundEdit, &QLineEdit::textChanged, [firstDependentVarEdit, startValueLabel](const QString& text) {
        if (!text.isEmpty()) {
            startValueLabel->setText(QString("%1(%2) =").arg(firstDependentVarEdit->text()).arg(text));
        }
    });

    HBoxWidget* startYContainer = new HBoxWidget();
    startYContainer->setSpacing(5);
    startYContainer->addWidget(startValueLabel);
    startYContainer->addWidget(startYEdit);

    container->addWidget(equationContainer);
    container->addWidget(lowerBoundContainer);
    container->addWidget(upperBoundContainer);
    container->addWidget(startYContainer);
    container->addWidget(createStepsInputContainer());

    return container;
}

QWidget* DiffMethodPageBase::createSecondOrderInputs() {
    VBoxWidget* container = new VBoxWidget();
    container->layout()->setAlignment(Qt::AlignTop);
    container->setSpacing(20);

    QDoubleValidator* doubleValidator = new QDoubleValidator();
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);

    HBoxWidget* lowerBoundContainer = new HBoxWidget();
    QLineEdit* lowerBoundEdit = new QLineEdit(mLowerBoundStr);
    lowerBoundEdit->setValidator(doubleValidator);
    lowerBoundContainer->addWidget(CreateLabel("Начало:", 12.0f));
    lowerBoundContainer->addWidget(lowerBoundEdit);
    lowerBoundContainer->setSpacing(5);
    BindVariable(lowerBoundEdit, &QLineEdit::textChanged, mLowerBoundStr);

    HBoxWidget* upperBoundContainer = new HBoxWidget();
    QLineEdit* upperBoundEdit = new QLineEdit(mUpperBoundStr);
    upperBoundEdit->setValidator(doubleValidator);
    upperBoundContainer->addWidget(CreateLabel("Конец:", 12.0f));
    upperBoundContainer->addWidget(upperBoundEdit);
    upperBoundContainer->setSpacing(5);
    BindVariable(upperBoundEdit, &QLineEdit::textChanged, mUpperBoundStr);

    QLineEdit* firstExpressionEdit = new QLineEdit(mFirstExpressionStr);
    BindVariable(firstExpressionEdit, &QLineEdit::textChanged, mFirstExpressionStr);
    SetFontSize(firstExpressionEdit, 14.0f);

    QLineEdit* secondExpressionEdit = new QLineEdit(mSecondExpressionStr);
    BindVariable(secondExpressionEdit, &QLineEdit::textChanged, mSecondExpressionStr);
    SetFontSize(secondExpressionEdit, 14.0f);

    QLineEdit* firstDependentVarEdit = new QLineEdit(mFirstDependentVarStr);
    firstDependentVarEdit->setMaxLength(1);
    firstDependentVarEdit->setFixedWidth(25);
    firstDependentVarEdit->setAlignment(Qt::AlignCenter);
    // mDependentVarEdit->setFrame(false);
    SetFontSize(firstDependentVarEdit, 12.0f);
    BindVariable(firstDependentVarEdit, &QLineEdit::textChanged, mFirstExpressionStr);

    QLineEdit* secondDependentVarEdit = new QLineEdit(mSecondDependentVarStr);
    secondDependentVarEdit->setMaxLength(1);
    secondDependentVarEdit->setFixedWidth(25);
    secondDependentVarEdit->setAlignment(Qt::AlignCenter);
    // mDependentVarEdit->setFrame(false);
    SetFontSize(secondDependentVarEdit, 12.0f);
    BindVariable(secondDependentVarEdit, &QLineEdit::textChanged, mSecondDependentVarStr);

    QLineEdit* firstIndependentVarEdit = new QLineEdit(mIndependentVar);
    firstIndependentVarEdit->setMaxLength(1);
    firstIndependentVarEdit->setFixedWidth(25);
    firstIndependentVarEdit->setAlignment(Qt::AlignCenter);
    SetFontSize(firstIndependentVarEdit, 12.0f);

    QLineEdit* secondIndependentVarEdit = new QLineEdit(mIndependentVar);
    secondIndependentVarEdit->setMaxLength(1);
    secondIndependentVarEdit->setFixedWidth(25);
    secondIndependentVarEdit->setAlignment(Qt::AlignCenter);
    SetFontSize(secondIndependentVarEdit, 12.0f);

    QObject::connect(firstIndependentVarEdit, &QLineEdit::textChanged, [this, secondIndependentVarEdit](const QString& text){
        secondIndependentVarEdit->setText(text);
        if (!text.isEmpty()) {
            mIndependentVar = text.at(0);
        }

    });
    QObject::connect(secondIndependentVarEdit, &QLineEdit::textChanged, [this, firstIndependentVarEdit](const QString& text){
        firstIndependentVarEdit->setText(text);
        if (!text.isEmpty()) {
            mIndependentVar = text.at(0);
        }
    });

    HBoxWidget* firstEquationContainer = new HBoxWidget();
    firstEquationContainer->setSpacing(10);
    {
        VBoxWidget* lhsContainer = new VBoxWidget();
        lhsContainer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

        HBoxWidget* topContainer = new HBoxWidget();
        topContainer->addWidget(CreateLabel("d", 18.0f));
        topContainer->addWidget(firstDependentVarEdit);
        
        HBoxWidget* bottomContainer = new HBoxWidget();
        bottomContainer->addWidget(CreateLabel("d", 18.0f));
        bottomContainer->addWidget(firstIndependentVarEdit);

        QWidget* line = new QWidget();
        line->setFixedHeight(2);
        line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        line->setStyleSheet("background-color: #000");

        lhsContainer->addWidget(topContainer);
        lhsContainer->addWidget(line);
        lhsContainer->addWidget(bottomContainer);

        firstEquationContainer->addWidget(lhsContainer);
        firstEquationContainer->addWidget(CreateLabel("=", 18.0f));
        firstEquationContainer->addWidget(firstExpressionEdit);
    }

    HBoxWidget* secondEquationContainer = new HBoxWidget();
    secondEquationContainer->setSpacing(10);
    {
        VBoxWidget* lhsContainer = new VBoxWidget();
        lhsContainer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

        HBoxWidget* topContainer = new HBoxWidget();
        topContainer->addWidget(CreateLabel("d", 18.0f));
        topContainer->addWidget(secondDependentVarEdit);
        
        HBoxWidget* bottomContainer = new HBoxWidget();
        bottomContainer->addWidget(CreateLabel("d", 18.0f));
        bottomContainer->addWidget(secondIndependentVarEdit);

        QWidget* line = new QWidget();
        line->setFixedHeight(2);
        line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        line->setStyleSheet("background-color: #000");

        lhsContainer->addWidget(topContainer);
        lhsContainer->addWidget(line);
        lhsContainer->addWidget(bottomContainer);

        secondEquationContainer->addWidget(lhsContainer);
        secondEquationContainer->addWidget(CreateLabel("=", 18.0f));
        secondEquationContainer->addWidget(secondExpressionEdit);
    }

    QLineEdit* startYEdit = new QLineEdit(mStartYStr);
    startYEdit->setValidator(doubleValidator);
    BindVariable(startYEdit, &QLineEdit::textChanged, mStartYStr);

    QLineEdit* startDyEdit = new QLineEdit(mStartDyStr);
    startDyEdit->setValidator(doubleValidator);
    BindVariable(startDyEdit, &QLineEdit::textChanged, mStartDyStr);

    QLabel* startValueLabel = CreateLabel("y(0) =", 12.0f);
    QLabel* startDiffValueLabel = CreateLabel("z(0) =", 12.0f);

    QObject::connect(firstDependentVarEdit, &QLineEdit::textChanged, [startYEdit, startValueLabel](const QString& text) {
        if (!text.isEmpty()) {
            startValueLabel->setText(QString("%1(%2) =").arg(text).arg(startYEdit->text()));
        }
    });

    QObject::connect(secondDependentVarEdit, &QLineEdit::textChanged, [startDyEdit, startDiffValueLabel](const QString& text) {
        if (!text.isEmpty()) {
            startDiffValueLabel->setText(QString("%1(%2) =").arg(text).arg(startDyEdit->text()));
        }
    });

    QObject::connect(lowerBoundEdit, &QLineEdit::textChanged, [firstDependentVarEdit, secondDependentVarEdit, startValueLabel, startDiffValueLabel](const QString& text) {
        if (!text.isEmpty()) {
            startValueLabel->setText(QString("%1(%2) =").arg(firstDependentVarEdit->text()).arg(text));
            startDiffValueLabel->setText(QString("%1(%2) =").arg(secondDependentVarEdit->text()).arg(text));
        }
    });

    HBoxWidget* startYContainer = new HBoxWidget();
    startYContainer->setSpacing(5);
    startYContainer->addWidget(startValueLabel);
    startYContainer->addWidget(startYEdit);

    HBoxWidget* startDyContainer = new HBoxWidget();
    startDyContainer->setSpacing(5);
    startDyContainer->addWidget(startDiffValueLabel);
    startDyContainer->addWidget(startDyEdit);

    container->addWidget(firstEquationContainer);
    container->addWidget(secondEquationContainer);
    container->addWidget(lowerBoundContainer);
    container->addWidget(upperBoundContainer);
    container->addWidget(startYContainer);
    container->addWidget(startDyContainer);
    container->addWidget(createStepsInputContainer());

    return container;
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

    if (mFirstDependentVarStr.isEmpty()) {
        errorText = "Ошибка: введите зависимую переменную.";
    } /* else if (mIndependentVarStr.isEmpty()) {
        errorText = "Ошибка: введите независимую переменную.";
    } */ else if (mFirstExpressionStr.isEmpty()) {
        errorText = "Ошибка: введите функцию.";
    } else if (mLowerBoundStr.isEmpty()) {
        errorText = "Ошибка: введите начало.";
    } else if (mUpperBoundStr.isEmpty()) {
        errorText = "Ошибка: введите конец.";
    } else {
        if (mStepsAmountStr.isEmpty()) {
            errorText = "Ошибка: введите кол-во шагов.";
        } else {
            n = mStepsAmountStr.toInt();
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

QWidget* DiffMethodPageBase::createStepsInputContainer() {
    VBoxWidget* stepsContainer = new VBoxWidget();

    QIntValidator* validator = new QIntValidator(1, std::numeric_limits<int>::max());

    QLabel* stepsLabel = CreateLabel("Кол-во шагов:", 12.0f);
    QLineEdit* stepsAmountEdit = new QLineEdit(mStepsAmountStr);
    stepsAmountEdit->setValidator(validator);
    stepsContainer->addWidget(stepsLabel);
    stepsContainer->addWidget(stepsAmountEdit);
    stepsContainer->setContentMargins(0, 0, 0, 0);
    stepsContainer->setSpacing(5);

    BindVariable(stepsAmountEdit, &QLineEdit::textChanged, mStepsAmountStr);

    return stepsContainer;
}