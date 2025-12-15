#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QSizePolicy>
#include <QPalette>
#include <QLabel>
#include <QBoxLayout>
#include <QOpenGLWidget>
#include <QStackedWidget>
#include <QGuiApplication>
#include <QScreen>

#include "main_window.hpp"

#include "./pages/integral/right_rectangles.hpp"
#include "./pages/integral/trapezoid.hpp"
#include "./pages/integral/left_rectangles.hpp"
#include "./pages/integral/simpson.hpp"

#include "./pages/diff/euler.hpp"
#include "./pages/diff/rungekutta.hpp"

#include "./pages/approx/base.hpp"

MainWindow::MainWindow() : QMainWindow() {
    setWindowTitle("Калькулятор интегралов");
    setMinimumSize(800, 600);
    const QSize screenSize = QGuiApplication::primaryScreen()->geometry().size();
    resize(QSize(screenSize.width() * 0.9, screenSize.height() * 0.8));
    setupUi();
}

namespace MethodTabs {
    enum : uint8_t {
        LeftRectangles = 0,
        RightRectangles,
        Trapezoid,
        Simpson,
        Euler,
        RungeKutta,
        Approximation
    };
}

static QPushButton* CreatePageButton(const QString& title) {
    QPushButton* button = new QPushButton();
    button->setText(title);
    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    button->setFixedHeight(40);
    button->setStyleSheet(
        "QPushButton { background-color: white; border-radius: 5px; border: none; }"
        "QPushButton:hover { background-color: rgb(230, 230, 230); }"
    );
    return button;
}

static QLabel* CreateSectionLabel(const QString& title) {
    QLabel* label = new QLabel(title);
    label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    label->setStyleSheet("QLabel{ font-size: 24px; }");
    label->setAlignment(Qt::AlignCenter);
    SetForegroundColor(label, Qt::white);
    return label;
}

void MainWindow::setupUi() {
    QHBoxLayout* mainContainerLayout = new QHBoxLayout();
    mainContainerLayout->setContentsMargins(0, 0, 0, 0);
    mainContainerLayout->setSpacing(0);

    mLeftRectanglesPageButton = CreatePageButton("Прямоугольники левых частей");
    mRightRectanglesPageButton = CreatePageButton("Прямоугольники правых частей");
    mTrapezoidPageButton = CreatePageButton("Трапеция");
    mSimpsonPageButton = CreatePageButton("Симпсон");
    mEulerPageButton = CreatePageButton("Эйлер");
    mRungeKuttaPageButton = CreatePageButton("Рунге-Кутта");
    mApproxPageButton = CreatePageButton("Элементарные функции");

    QWidget* menuContainer = new QWidget();
    SetColor(menuContainer, QPalette::Window, QColor("#212121"));

    menuContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    menuContainer->setAutoFillBackground(true);
    menuContainer->setMinimumWidth(300);

    QVBoxLayout* menuLayout = new QVBoxLayout();
    menuLayout->setContentsMargins(10, 0, 10, 0);
    menuLayout->setSpacing(20);
    menuLayout->addStretch();
    menuLayout->addWidget(CreateSectionLabel("Интеграл"));
    menuLayout->addWidget(mLeftRectanglesPageButton);
    menuLayout->addWidget(mRightRectanglesPageButton);
    menuLayout->addWidget(mTrapezoidPageButton);
    menuLayout->addWidget(mSimpsonPageButton);
    menuLayout->addWidget(CreateSectionLabel("Дифференциал"));
    menuLayout->addWidget(mEulerPageButton);
    menuLayout->addWidget(mRungeKuttaPageButton);
    menuLayout->addWidget(CreateSectionLabel("Прибл. вычисление"));
    menuLayout->addWidget(mApproxPageButton);
    menuLayout->addStretch();

    menuContainer->setLayout(menuLayout);

    mContentWidget = new QStackedWidget();
    mContentWidget->insertWidget(MethodTabs::LeftRectangles, new LeftRectanglesPage());
    mContentWidget->insertWidget(MethodTabs::RightRectangles, new RightRectanglesPage());
    mContentWidget->insertWidget(MethodTabs::Trapezoid, new TrapezoidPage());
    mContentWidget->insertWidget(MethodTabs::Simpson, new SimpsonPage());
    mContentWidget->insertWidget(MethodTabs::Euler, new EulerPage());
    mContentWidget->insertWidget(MethodTabs::RungeKutta, new RungeKuttaPage());
    mContentWidget->insertWidget(MethodTabs::Approximation, new ApproxCalculationPage());

    mainContainerLayout->addWidget(menuContainer);
    mainContainerLayout->addWidget(mContentWidget);
    
    QWidget* mainContainer = new QWidget();
    mainContainer->setLayout(mainContainerLayout);

    // This is a hack. It's needed to initialize OpenGL at the start of the application, so later window is not recreated
    // when plotting a series with "useOpenGL" enabled.
    // Using QStackedWidget here to initialize OpenGL with "QOpenGLWidget" and keep it hidden forever.
    QStackedWidget* stackedWidget = new QStackedWidget();
    stackedWidget->addWidget(mainContainer);
    stackedWidget->addWidget(new QOpenGLWidget());

    QObject::connect(mLeftRectanglesPageButton, &QPushButton::clicked, [this] {
        mContentWidget->setCurrentIndex(MethodTabs::LeftRectangles);
    });

    QObject::connect(mRightRectanglesPageButton, &QPushButton::clicked, [this] {
        mContentWidget->setCurrentIndex(MethodTabs::RightRectangles);
    });

    QObject::connect(mTrapezoidPageButton, &QPushButton::clicked, [this] {
        mContentWidget->setCurrentIndex(MethodTabs::Trapezoid);
    });

    QObject::connect(mSimpsonPageButton, &QPushButton::clicked, [this] {
        mContentWidget->setCurrentIndex(MethodTabs::Simpson);
    });

    QObject::connect(mEulerPageButton, &QPushButton::clicked, [this] {
        mContentWidget->setCurrentIndex(MethodTabs::Euler);
    });

    QObject::connect(mRungeKuttaPageButton, &QPushButton::clicked, [this] {
        mContentWidget->setCurrentIndex(MethodTabs::RungeKutta);
    });

    QObject::connect(mApproxPageButton, &QPushButton::clicked, [this] {
        mContentWidget->setCurrentIndex(MethodTabs::Approximation);
    });

    setCentralWidget(stackedWidget);
}
