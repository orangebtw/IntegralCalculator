#include "base.hpp"
#include <QLocale>
#include <QString>
#include <QButtonGroup>
#include <QRadioButton>
#include <qbuttongroup.h>
#include <qradiobutton.h>

#include "../../widgets/hboxwidget.hpp"
#include "../../widgets/gridwidget.hpp"
#include "../../widgets/vboxwidget.hpp"
#include "../../utils.hpp"

#include "../../../approx.hpp"

ApproxCalculationPage::ApproxCalculationPage(QWidget* parent) : QWidget(parent) {
    setupUi("Приближенное вычисление элементарных функций");
    setCalculateButtonCallback([this] {
        if (!validate()) return;

        QLocale locale = QLocale::system();

        double startX  = locale.toDouble(mStartXEdit->text());
        double startY  = locale.toDouble(mStartYEdit->text());
        double epsilon = locale.toDouble(mEpsilonEdit->text());

        double result = 1337.0;

        switch (mFunctionType) {
        case FunctionType::EtoX:
            result = approx::e(startX);
            break;
        case FunctionType::Sin:
            result = approx::sin(startX);
            break;
        case FunctionType::Sqrt:
            result = approx::sqrt(startX, startY, epsilon);
            break;
        case FunctionType::InvSqrt:
            result = approx::inv_sqrt(startX, startY, epsilon);
            break;
        }

        setResult(QString("Результат: %1").arg(result));
    });
}

void ApproxCalculationPage::setupUi(const QString& title) {
    QDoubleValidator* doubleValidator = new QDoubleValidator();

    mResultLabel = new QLabel();
    SetFontSize(mResultLabel, 18.0f);

    mCalculateButton = new QPushButton("Вычислить");
    
    mStartXEdit = new QLineEdit();
    mStartXEdit->setValidator(doubleValidator);
    SetFontSize(mStartXEdit, 12.0f);

    mStartYEdit = new QLineEdit();
    mStartYEdit->setValidator(doubleValidator);
    SetFontSize(mStartYEdit, 12.0f);

    mEpsilonEdit = new QLineEdit("0,001");
    mEpsilonEdit->setValidator(doubleValidator);
    SetFontSize(mEpsilonEdit, 12.0f);
    
    QRadioButton* eFuncButton = new QRadioButton("e^x");
    SetFontSize(eFuncButton, 14.0f);

    QRadioButton* sinFuncButton = new QRadioButton("sin(x)");
    SetFontSize(sinFuncButton, 14.0f);

    QRadioButton* sqrtFuncButton = new QRadioButton("sqrt(x)");
    SetFontSize(sqrtFuncButton, 14.0f);

    QRadioButton* invSqrtFuncButton = new QRadioButton("1/sqrt(x)");
    SetFontSize(invSqrtFuncButton, 14.0f);
    
    VBoxWidget* functionSelectionContainer = new VBoxWidget();
    functionSelectionContainer->addWidget(CreateLabel("Элементарная функция:", 18.0f));
    functionSelectionContainer->addWidget(eFuncButton);
    functionSelectionContainer->addWidget(sinFuncButton);
    functionSelectionContainer->addWidget(sqrtFuncButton);
    functionSelectionContainer->addWidget(invSqrtFuncButton);

    eFuncButton->setChecked(true);

    mFunctionGroup = new QButtonGroup();
    mFunctionGroup->addButton(eFuncButton, EnumValue(FunctionType::EtoX));
    mFunctionGroup->addButton(sinFuncButton, EnumValue(FunctionType::Sin));
    mFunctionGroup->addButton(sqrtFuncButton, EnumValue(FunctionType::Sqrt));
    mFunctionGroup->addButton(invSqrtFuncButton, EnumValue(FunctionType::InvSqrt));

    mContainer = new GridWidget();
    mContainer->setHorizontalSpacing(10);
    mContainer->setVerticalSpacing(10);

    mContainer->addWidget(CreateLabel("x", 18.0f), 0, 0);
    mContainer->addWidget(CreateLabel("y<sub>0</sub>", 18.0f), 1, 0);
    mContainer->addWidget(CreateLabel(QChar(0x03B5), 18.0f), 2, 0);

    mContainer->addWidget(CreateLabel("=", 18.0f), 0, 1);
    mContainer->addWidget(CreateLabel("=", 18.0f), 1, 1);
    mContainer->addWidget(CreateLabel("=", 18.0f), 2, 1);

    mContainer->addWidget(mStartXEdit, 0, 2);
    mContainer->addWidget(mStartYEdit, 1, 2);
    mContainer->addWidget(mEpsilonEdit, 2, 2);

    if (mFunctionType == FunctionType::EtoX || mFunctionType == FunctionType::Sin) {
        mContainer->hideRow(1);
    } else {
        mContainer->showRow(1);
    }

    QObject::connect(mFunctionGroup, &QButtonGroup::idToggled, [this](int id, bool checked) {
        if (!checked)
            return;

        switch (id) {
            case EnumValue(FunctionType::EtoX):
                functionTypeChanged(FunctionType::EtoX);
                break;
            case EnumValue(FunctionType::Sin):
                functionTypeChanged(FunctionType::Sin);
                break;
            case EnumValue(FunctionType::Sqrt):
                functionTypeChanged(FunctionType::Sqrt);
                break;
            case EnumValue(FunctionType::InvSqrt):
                functionTypeChanged(FunctionType::InvSqrt);
                break;
            default:
                throw;
        }
    });

    QLabel* titleLabel = CreateLabel(title, 36.0f);
    titleLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    titleLabel->setAlignment(Qt::AlignHCenter);
    
    mMainLayout = new QVBoxLayout();
    mMainLayout->setAlignment(Qt::AlignTop);
    mMainLayout->setSpacing(20);
    mMainLayout->addWidget(titleLabel);
    mMainLayout->addWidget(functionSelectionContainer);
    mMainLayout->addWidget(mContainer);
    mMainLayout->addWidget(mCalculateButton);
    mMainLayout->addWidget(mResultLabel);

    setLayout(mMainLayout);
}

void ApproxCalculationPage::setCalculateButtonCallback(std::function<void()> callback) {
    QObject::connect(mCalculateButton, &QPushButton::clicked, callback);
}

void ApproxCalculationPage::setError(const QString& errorText) {
    QPalette palette = mResultLabel->palette();
    palette.setColor(mResultLabel->foregroundRole(), Qt::red);
    mResultLabel->setPalette(palette);
    mResultLabel->setText(errorText);
    mResultLabel->show();
}

void ApproxCalculationPage::setResult(const QString& resultText) {
    QPalette palette = mResultLabel->palette();
    palette.setColor(mResultLabel->foregroundRole(), Qt::black);
    mResultLabel->setPalette(palette);
    mResultLabel->setText(resultText);
    mResultLabel->show();
}

bool ApproxCalculationPage::validate() {
    if (mStartXEdit->text().isEmpty()) {
        setError("Ошибка: введите X.");
        return false;
    }
    if ((mFunctionType != FunctionType::EtoX && mFunctionType != FunctionType::Sin) && mStartYEdit->text().isEmpty()) {
        setError("Ошибка: введите y<sub>0</sub>.");
        return false;
    }
    if (mEpsilonEdit->text().isEmpty()) {
        setError("Ошибка: введите эпсилон.");
        return false;
    }

    return true;
}

void ApproxCalculationPage::functionTypeChanged(FunctionType functionType) {
    mFunctionType = functionType;

    if (functionType == FunctionType::EtoX || functionType == FunctionType::Sin) {
        mContainer->hideRow(1);
    } else {
        mContainer->showRow(1);
    }
}