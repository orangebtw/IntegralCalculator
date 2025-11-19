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
#include <algorithm>
#include <qlineedit.h>
#include <qsizepolicy.h>

#include "base.hpp"
#include "../../widgets/chartview.hpp"
#include "../../widgets/hboxwidget.hpp"
#include "../../widgets/vboxwidget.hpp"
#include "../../utils.hpp"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    using namespace QtCharts;
#endif

DiffMethodPageBase::DiffMethodPageBase(bool secondOrder, const QString& title, QWidget* parent) : QScrollArea(parent) {
    setupUi(title, secondOrder);
    setCalculateButtonCallback([this, secondOrder] {
        if (!validate()) return;
        
        QLocale locale = QLocale::system();

        int steps = mStepsAmountStr.toInt();

        std::vector<std::string> exprs;
        exprs.reserve(mEquationWidgets.size());
        for (const auto& widget : mEquationWidgets) {
            exprs.push_back(widget.expressionEdit->text().toStdString());
        }

        std::vector<char> dependentVars;
        dependentVars.reserve(mEquationWidgets.size());
        for (const auto& widget : mEquationWidgets) {
            dependentVars.push_back(widget.dependentVarEdit->text().at(0).toLatin1());
        };

        std::vector<double> startValues;
        startValues.reserve(mEquationWidgets.size());
        for (const auto& widget : mEquationWidgets) {
            startValues.push_back(locale.toDouble(widget.startValueEdit->text()));
        };

        double x_start = locale.toDouble(mLowerBoundStr);
        double x_end = locale.toDouble(mUpperBoundStr);
        char independentVar = mIndependentVarStr.at(0).toLatin1();
        
        auto result = calculate2(x_start, x_end, steps, independentVar, startValues, dependentVars, exprs);

        if (result.has_value()) {
            auto& start = result->front();
            auto& end = result->back();

            double newMinX = std::min(start[0], mMinX);
            double newMaxX = std::max(end[0], mMaxX);

            double newMinY = std::min(start[1], mMinY);
            for (auto& var : start) {
                newMinY = std::min(var, newMinY);
            }

            double newMaxY = std::max(end[1], mMaxY);
            for (auto& var : end) {
                newMaxY = std::min(var, newMaxY);
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

            QStringList headerLabels;
            headerLabels.append("x");
            for (auto var : dependentVars) {
                headerLabels.append(QString(var));
            }

            mModel.reset(new QStandardItemModel(result->size(), dependentVars.size() + 1));
            mModel->setHorizontalHeaderLabels(headerLabels);

            const uint32_t rowLength = start.size();

            for (int row = 0; row < result->size(); ++row) {
                auto& data = result.value()[row];
                for (int i = 0; i < rowLength; ++i) {
                    mModel->setItem(row, i, new QStandardItem(QString::number(data[i])));
                }
            }

            for (int i = 1; i < rowLength; ++i) {
                QLineSeries* series = new QLineSeries();
                series->setUseOpenGL(true);
                series->setName(QString("%1 шагов").arg(steps));

                for (int row = 0; row < result->size(); ++row) {
                    auto& data = result.value()[row];
                    series->append(data[0], data[i]);
                }

                mChart->addSeries(series);

                series->attachAxis(mAxisX);
                series->attachAxis(mAxisY);
            }

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
    mMainLayout->addWidget(CreateLabel("График", 22.0f, QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred)));
    mMainLayout->addWidget(mChartView);
    mMainLayout->addWidget(clearButton);
    mMainLayout->addWidget(CreateLabel("Таблица", 22.0f, QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred)));
    mMainLayout->addWidget(mTable);
}

EquationInputWidget DiffMethodPageBase::createEquationInput(const QString& dependentVarStr, QString& independentVarStr, QWidget* parent) {
    QLineEdit* expressionEdit = new QLineEdit();
    SetFontSize(expressionEdit, 14.0f);

    QLineEdit* dependentVarEdit = new QLineEdit(dependentVarStr);
    dependentVarEdit->setMaxLength(1);
    dependentVarEdit->setFixedWidth(25);
    dependentVarEdit->setAlignment(Qt::AlignCenter);
    // mDependentVarEdit->setFrame(false);
    SetFontSize(dependentVarEdit, 12.0f);
    
    QLineEdit* independentVarEdit = new QLineEdit(independentVarStr);
    independentVarEdit->setMaxLength(1);
    independentVarEdit->setFixedWidth(25);
    independentVarEdit->setAlignment(Qt::AlignCenter);
    // independentVarEdit->setFrame(false);
    SetFontSize(independentVarEdit, 12.0f);

    QObject::connect(independentVarEdit, &QLineEdit::textChanged, [&independentVarStr](const QString& text) {
        if (!text.isEmpty()) {
            independentVarStr = text.at(0);
        }
    });

    HBoxWidget* equationContainer = new HBoxWidget(parent);
    equationContainer->setSpacing(10);

    VBoxWidget* lhsContainer = new VBoxWidget();
    lhsContainer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    HBoxWidget* topContainer = new HBoxWidget();
    topContainer->addWidget(CreateLabel("d", 18.0f));
    topContainer->addWidget(dependentVarEdit);
    
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
    equationContainer->addWidget(expressionEdit);

    QDoubleValidator* doubleValidator = new QDoubleValidator();
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);

    QLineEdit* startValueEdit = new QLineEdit();
    startValueEdit->setValidator(doubleValidator);

    QLabel* startValueLabel = CreateLabel(QString("%1(0) =").arg(dependentVarStr), 12.0f);

    HBoxWidget* startValueContainer = new HBoxWidget();
    startValueContainer->setSpacing(5);
    startValueContainer->addWidget(startValueLabel);
    startValueContainer->addWidget(startValueEdit);

    return EquationInputWidget {
        .dependentVarEdit = dependentVarEdit,
        .independentVarEdit = independentVarEdit,
        .expressionEdit = expressionEdit,
        .startValueEdit = startValueEdit,
        .startValueLabel = startValueLabel,
        .equationContainer = equationContainer,
        .startValueContainer = startValueContainer
    };
}

void BindEquationToStartValue(const std::vector<EquationInputWidget>& equationInputList, size_t index, const QLineEdit* xStartEdit) {
    QLabel* startValueLabel = equationInputList[index].startValueLabel;

    QObject::connect(equationInputList[index].dependentVarEdit, &QLineEdit::textChanged, [xStartEdit, index, startValueLabel, &equationInputList](const QString& text) {
        if (!text.isEmpty()) {
            startValueLabel->setText(QString("%1(%2) =").arg(text).arg(xStartEdit->text()));
        }
    });

    QObject::connect(equationInputList[index].independentVarEdit, &QLineEdit::textChanged, [xStartEdit, index, &equationInputList](const QString& text) {
        if (!text.isEmpty()) {
            for (size_t i = 0; i < equationInputList.size(); ++i) {
                if (i == index) continue;
                const EquationInputWidget& widget = equationInputList[i];
                widget.independentVarEdit->setText(text);
            }
        }
    });
}

QWidget* DiffMethodPageBase::createFirstOrderInputs() {
    VBoxWidget* container = new VBoxWidget(this);
    container->layout()->setAlignment(Qt::AlignTop);
    container->setSpacing(20);

    VBoxWidget* equationListContainer = new VBoxWidget(this);
    equationListContainer->setSpacing(10);

    VBoxWidget* startValueListContainer = new VBoxWidget(this);
    startValueListContainer->setSpacing(10);

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

    EquationInputWidget equationWidget = createEquationInput("y", mIndependentVarStr);
    equationListContainer->addWidget(equationWidget.equationContainer);
    startValueListContainer->addWidget(equationWidget.startValueContainer);
    mEquationWidgets.push_back(equationWidget);

    BindEquationToStartValue(mEquationWidgets, 0, lowerBoundEdit);

    QPushButton* addEquationButton = new QPushButton("Добавить");
    // SetFontSize(addEquationButton, 14.0f);

    QObject::connect(addEquationButton, &QPushButton::clicked, [this, lowerBoundEdit, equationListContainer, startValueListContainer]() {
        const size_t index = mEquationWidgets.size();
        
        EquationInputWidget equationWidget = createEquationInput("z", mIndependentVarStr);
        mEquationWidgets.push_back(equationWidget);

        equationListContainer->addWidget(equationWidget.equationContainer);
        startValueListContainer->addWidget(equationWidget.startValueContainer);

        BindEquationToStartValue(mEquationWidgets, index, lowerBoundEdit);
    });

    QObject::connect(lowerBoundEdit, &QLineEdit::textChanged, [this, startValueListContainer](const QString& text) {
        if (!text.isEmpty()) {
            const size_t n = mEquationWidgets.size();

            for (size_t i = 0; i < n; ++i) {
                const EquationInputWidget& equationWidget = mEquationWidgets[i];
                equationWidget.startValueLabel->setText(QString("%1(%2) =").arg(equationWidget.dependentVarEdit->text()).arg(text));
            }
        }
    });

    container->addWidget(equationListContainer);
    container->addWidget(addEquationButton);
    container->addWidget(startValueListContainer);
    container->addWidget(lowerBoundContainer);
    container->addWidget(upperBoundContainer);
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

    EquationInputWidget firstEquationWidget = createEquationInput("y", mIndependentVarStr);
    mEquationWidgets.push_back(firstEquationWidget);

    EquationInputWidget secondEquationWidget = createEquationInput("z", mIndependentVarStr);
    mEquationWidgets.push_back(secondEquationWidget);

    BindEquationToStartValue(mEquationWidgets, 0, lowerBoundEdit);
    BindEquationToStartValue(mEquationWidgets, 1, lowerBoundEdit);

    // QObject::connect(secondEquationWidget.dependentVarEdit, &QLineEdit::textChanged, [startDyEdit, startDiffValueLabel](const QString& text) {
    //     if (!text.isEmpty()) {
    //         startDiffValueLabel->setText(QString("%1(%2) =").arg(text).arg(startDyEdit->text()));
    //     }
    // });

    // QObject::connect(lowerBoundEdit, &QLineEdit::textChanged, [firstEquationWidget, secondEquationWidget, startValueLabel, startDiffValueLabel](const QString& text) {
    //     if (!text.isEmpty()) {
    //         startValueLabel->setText(QString("%1(%2) =").arg(firstEquationWidget.dependentVarEdit->text()).arg(text));
    //         startDiffValueLabel->setText(QString("%1(%2) =").arg(secondEquationWidget.dependentVarEdit->text()).arg(text));
    //     }
    // });

    container->addWidget(firstEquationWidget.equationContainer);
    container->addWidget(secondEquationWidget.equationContainer);
    container->addWidget(lowerBoundContainer);
    container->addWidget(upperBoundContainer);
    container->addWidget(firstEquationWidget.startValueContainer);
    container->addWidget(secondEquationWidget.startValueContainer);
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

bool DiffMethodPageBase::validate() {
    std::optional<QString> errorText;

    for (uint32_t i = 0; i < mEquationWidgets.size(); ++i) {
        const EquationInputWidget& widget = mEquationWidgets[i];
        if (widget.dependentVarEdit->text().isEmpty()) {
            errorText = QString("Ошибка: введите зависимую переменную для %1 уравнения.").arg(i+1);
        } else if (widget.expressionEdit->text().isEmpty()) {
            errorText = QString("Ошибка: введите функцию для %1 уравнения.").arg(i+1);
        }

        if (errorText)
            break;
    }

    if (!errorText.has_value()) {
        if (mIndependentVarStr.isEmpty()) {
            errorText = "Ошибка: введите независимую переменную.";
        } else if (mLowerBoundStr.isEmpty()) {
            errorText = "Ошибка: введите начало.";
        } else if (mUpperBoundStr.isEmpty()) {
            errorText = "Ошибка: введите конец.";
        } else {
            if (mStepsAmountStr.isEmpty()) {
                errorText = "Ошибка: введите кол-во шагов.";
            } else {
                int n = mStepsAmountStr.toInt();
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