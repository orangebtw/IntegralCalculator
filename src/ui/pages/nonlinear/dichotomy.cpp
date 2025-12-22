#include "dichotomy.hpp"
#include <QLocale>
#include <QString>
#include <QButtonGroup>
#include <QRadioButton>
#include <QButtonGroup>
#include <QRadioButton>
#include <QHeaderView>
#include <qlineedit.h>
#include <qnamespace.h>

#include "../../widgets/gridwidget.hpp"
#include "../../utils.hpp"

#include "../../../exprtk.hpp"
#include "../../../utils.hpp"

NonlinearDichotomyMethodPage::NonlinearDichotomyMethodPage(QWidget* parent) : QWidget(parent) {
    setupUi("Численный метод решения нелинейных уравнений (метод Дихотомии)");
    setCalculateButtonCallback([this] {
        if (!validate()) return;

        const QLocale locale = QLocale::system();

        const double upperBound  = locale.toDouble(mUpperBoundEdit->text());
        const double lowerBound  = locale.toDouble(mLowerBoundEdit->text());
        const double epsilon = locale.toDouble(mEpsilonEdit->text());
        const std::string function = mFunctionEdit->text().toStdString();

        mModel.reset(new QStandardItemModel(1, 2));
        // mModel->setHorizontalHeaderLabels({ "Шаг", "x<sub>i</sub>", "f(x<sub>i</sub>)",  "|x_<sub>i</sub>|"});
        mModel->setHorizontalHeaderLabels({ "a", "b",  "c=(a+b)/2", "f(c)", "|b-a|"});

        const CalculateResult result = calculate(lowerBound, upperBound, epsilon, function);

        if (result.has_value()) {
            mTable->setModel(mModel.get());
            setResult(QString("Результат: %1").arg(result.value()));
        } else {
            setError(QString("Ошибка: %1").arg(result.error()));
        }
    });
}

void NonlinearDichotomyMethodPage::setupUi(const QString& title) {
    QDoubleValidator* doubleValidator = new QDoubleValidator();

    mResultLabel = new QLabel();
    SetFontSize(mResultLabel, 18.0f);

    mTable = new QTableView();
    // mTable->setHorizontalHeader(new HtmlHeaderView(Qt::Horizontal));
    mTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mTable->horizontalHeader()->setDefaultAlignment(Qt::AlignmentFlag::AlignCenter);

    mCalculateButton = new QPushButton("Вычислить");

    mFunctionEdit = new QLineEdit();

    mEpsilonEdit = new QLineEdit("0,001");
    mEpsilonEdit->setValidator(doubleValidator);
    SetFontSize(mEpsilonEdit, 12.0f);

    mLowerBoundEdit = new QLineEdit("-1");
    mLowerBoundEdit->setValidator(doubleValidator);
    SetFontSize(mLowerBoundEdit, 12.0f);

    mUpperBoundEdit = new QLineEdit("1");
    mUpperBoundEdit->setValidator(doubleValidator);
    SetFontSize(mUpperBoundEdit, 12.0f);

    mContainer = new GridWidget();
    mContainer->setHorizontalSpacing(10);
    mContainer->setVerticalSpacing(10);

    mContainer->addColumn({
        CreateLabel("f(x)", 18.0f),
        CreateLabel("Начало (a)", 18.0f),
        CreateLabel("Конец (b)", 18.0f),
        CreateLabel(QString("Точность (%1)").arg(QChar(0x03B5)), 18.0f),
    });

    mContainer->addColumn({
        CreateLabel("=", 18.0f),
        CreateLabel("=", 18.0f),
        CreateLabel("=", 18.0f),
        CreateLabel("=", 18.0f),
    });

    mContainer->addColumn({
        mFunctionEdit,
        mLowerBoundEdit,
        mUpperBoundEdit,
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
    // mMainLayout->addWidget(new HBoxWidget({
    //     .spacing = 5,
    //     .widgets = { CreateLabel("Начало (a):", 16.0f), mLowerBoundEdit }
    // }));
    // mMainLayout->addWidget(new HBoxWidget({
    //     .spacing = 5,
    //     .widgets = { CreateLabel("Конец (b):", 16.0f), mUpperBoundEdit }
    // }));
    mMainLayout->addWidget(mCalculateButton);
    mMainLayout->addWidget(mResultLabel);
    mMainLayout->addWidget(mTable);

    setLayout(mMainLayout);
}

void NonlinearDichotomyMethodPage::setCalculateButtonCallback(std::function<void()> callback) {
    QObject::connect(mCalculateButton, &QPushButton::clicked, callback);
}

void NonlinearDichotomyMethodPage::setError(const QString& errorText) {
    QPalette palette = mResultLabel->palette();
    palette.setColor(mResultLabel->foregroundRole(), Qt::red);
    mResultLabel->setPalette(palette);
    mResultLabel->setText(errorText);
    mResultLabel->show();
}

void NonlinearDichotomyMethodPage::setResult(const QString& resultText) {
    QPalette palette = mResultLabel->palette();
    palette.setColor(mResultLabel->foregroundRole(), Qt::black);
    mResultLabel->setPalette(palette);
    mResultLabel->setText(resultText);
    mResultLabel->show();
}

bool NonlinearDichotomyMethodPage::validate() {
    if (mFunctionEdit->text().isEmpty()) {
        setError("Ошибка: введите функцию.");
        return false;
    }

    if (mLowerBoundEdit->text().isEmpty()) {
        setError("Ошибка: введите начало (a).");
        return false;
    }

    if (mUpperBoundEdit->text().isEmpty()) {
        setError("Ошибка: введите конец (b).");
        return false;
    }

    if (mEpsilonEdit->text().isEmpty()) {
        setError("Ошибка: введите эпсилон.");
        return false;
    }

    return true;
}

NonlinearDichotomyMethodPage::CalculateResult NonlinearDichotomyMethodPage::calculate(double lowerBound, double upperBound, double epsilon, const std::string& function) {
    double x = 0.0;

    exprtk::expression<double> expression;
    if (!utils::compile_expression(expression, function, x)) {
        return std::unexpected("Не правильное выражение");
    }

    int i = 0;

    double a = lowerBound;
    double b = upperBound;

    while (true) {
        const double c = (a + b) / 2.0;

        x = c; double f_с = expression.value();

        double diff = std::abs(b - a);
        mModel->setItem(i, 0, new QStandardItem(QString::number(a)));
        mModel->setItem(i, 1, new QStandardItem(QString::number(b)));
        mModel->setItem(i, 2, new QStandardItem(QString::number(c)));
        mModel->setItem(i, 3, new QStandardItem(QString::number(f_с)));
        mModel->setItem(i, 4, new QStandardItem(QString::number(diff)));

        if (diff <= epsilon)
            break;

        x = a; double f_a = expression.value();

        if (f_a * f_с < 0.0) {
            b = c;
        } else {
            x = b; double f_b = expression.value();
            if (f_с * f_b < 0.0) {
                a = c;
            }
        }

        ++i;
    }

    return x;
}