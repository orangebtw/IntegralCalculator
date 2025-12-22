#include "newton.hpp"
#include <QLocale>
#include <QString>
#include <QButtonGroup>
#include <QRadioButton>
#include <QButtonGroup>
#include <QRadioButton>
#include <QHeaderView>
#include <qnamespace.h>

#include "../../widgets/gridwidget.hpp"
#include "../../utils.hpp"

#include "../../../exprtk.hpp"
#include "../../../utils.hpp"

NonlinearNewtonMethodPage::NonlinearNewtonMethodPage(QWidget* parent) : QWidget(parent) {
    setupUi("Численный метод решения нелинейных уравнений (метод Ньютона)");
    setCalculateButtonCallback([this] {
        if (!validate()) return;

        const QLocale locale = QLocale::system();

        const double start = locale.toDouble(mStartEdit->text());
        const double epsilon = locale.toDouble(mEpsilonEdit->text());
        const std::string function = mFunctionEdit->text().toStdString();
        const std::string derivative = mDerivativeEdit->text().toStdString();

        mModel.reset(new QStandardItemModel(1, 2));
        // mModel->setHorizontalHeaderLabels({ "Шаг", "x<sub>i</sub>", "f(x<sub>i</sub>)",  "|x_<sub>i</sub>|"});
        mModel->setHorizontalHeaderLabels({ "Шаг", "x_i", "f(x_i)",  "|x_i - x_{i-1}|"});
        
        const CalculateResult result = calculate(start, epsilon, function, derivative);

        if (result.has_value()) {
            mTable->setModel(mModel.get());
            setResult(QString("Результат: %1").arg(result.value()));
        } else {
            setError(QString("Ошибка: %1").arg(result.error()));
        }
    });
}

void NonlinearNewtonMethodPage::setupUi(const QString& title) {
    QDoubleValidator* doubleValidator = new QDoubleValidator();

    mResultLabel = new QLabel();
    SetFontSize(mResultLabel, 18.0f);

    mTable = new QTableView();
    // mTable->setHorizontalHeader(new HtmlHeaderView(Qt::Horizontal));
    mTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mTable->horizontalHeader()->setDefaultAlignment(Qt::AlignmentFlag::AlignCenter);

    mCalculateButton = new QPushButton("Вычислить");

    mFunctionEdit = new QLineEdit();
    mDerivativeEdit = new QLineEdit();
    
    mStartEdit = new QLineEdit();
    mStartEdit->setValidator(doubleValidator);
    SetFontSize(mStartEdit, 12.0f);

    mEpsilonEdit = new QLineEdit("0,001");
    mEpsilonEdit->setValidator(doubleValidator);
    SetFontSize(mEpsilonEdit, 12.0f);

    mContainer = new GridWidget();
    mContainer->setHorizontalSpacing(10);
    mContainer->setVerticalSpacing(10);

    mContainer->addColumn({
        CreateLabel("f(x)", 18.0f),
        CreateLabel("f'(x)", 18.0f),
        CreateLabel("x<sub>0</sub>", 18.0f),
        CreateLabel(QChar(0x03B5), 18.0f),
    });

    mContainer->addColumn({
        CreateLabel("=", 18.0f),
        CreateLabel("=", 18.0f),
        CreateLabel("=", 18.0f),
        CreateLabel("=", 18.0f),
    });

    mContainer->addColumn({
        mFunctionEdit,
        mDerivativeEdit,
        mStartEdit,
        mEpsilonEdit
    });

    QLabel* titleLabel = CreateLabel(title, 36.0f);
    titleLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    titleLabel->setAlignment(Qt::AlignHCenter);
    titleLabel->setWordWrap(true);
    
    mMainLayout = new QVBoxLayout();
    mMainLayout->setAlignment(Qt::AlignTop);
    mMainLayout->setSpacing(20);
    mMainLayout->addWidget(titleLabel);
    mMainLayout->addWidget(mContainer);
    mMainLayout->addWidget(mCalculateButton);
    mMainLayout->addWidget(mResultLabel);
    mMainLayout->addWidget(mTable);

    setLayout(mMainLayout);
}

void NonlinearNewtonMethodPage::setCalculateButtonCallback(std::function<void()> callback) {
    QObject::connect(mCalculateButton, &QPushButton::clicked, callback);
}

void NonlinearNewtonMethodPage::setError(const QString& errorText) {
    QPalette palette = mResultLabel->palette();
    palette.setColor(mResultLabel->foregroundRole(), Qt::red);
    mResultLabel->setPalette(palette);
    mResultLabel->setText(errorText);
    mResultLabel->show();
}

void NonlinearNewtonMethodPage::setResult(const QString& resultText) {
    QPalette palette = mResultLabel->palette();
    palette.setColor(mResultLabel->foregroundRole(), Qt::black);
    mResultLabel->setPalette(palette);
    mResultLabel->setText(resultText);
    mResultLabel->show();
}

bool NonlinearNewtonMethodPage::validate() {
    if (mFunctionEdit->text().isEmpty()) {
        setError("Ошибка: введите функцию.");
        return false;
    }

    if (mDerivativeEdit->text().isEmpty()) {
        setError("Ошибка: введите производную.");
        return false;
    }

    if (mStartEdit->text().isEmpty()) {
        setError("Ошибка: введите x<sub>0</sub>");
        return false;
    }

    if (mEpsilonEdit->text().isEmpty()) {
        setError("Ошибка: введите эпсилон.");
        return false;
    }

    return true;
}

NonlinearNewtonMethodPage::CalculateResult NonlinearNewtonMethodPage::calculate(double start, double epsilon, const std::string& function, const std::string& derivative) {
    double x = start;

    exprtk::expression<double> expression;
    if (!utils::compile_expression(expression, function, x)) {
        return std::unexpected("Не правильное выражение");
    }

    exprtk::expression<double> expressionDerivative;
    if (!utils::compile_expression(expressionDerivative, derivative, x)) {
        return std::unexpected("Не правильное выражение");
    }

    int i = 0;

    double prev_x = std::numeric_limits<double>::infinity();

    while (true) {
        double f = expression.value();
        double diff = std::abs(x - prev_x);
        mModel->setItem(i, 0, new QStandardItem(QString::number(i)));
        mModel->setItem(i, 1, new QStandardItem(QString::number(x)));
        mModel->setItem(i, 2, new QStandardItem(QString::number(f)));
        mModel->setItem(i, 3, new QStandardItem(QString::number(diff)));
        prev_x = x;
        x = x - f / expressionDerivative.value();
        if (diff <= epsilon)
            break;
        ++i;
    }

    return x;
}