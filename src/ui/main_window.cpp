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

#include "pages/right_rectangles.hpp"
#include "pages/trapezoid.hpp"
#include "pages/left_rectangles.hpp"
#include "pages/simpson.hpp"
#include "pages/variable_step.hpp"

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
        VariableStep
    };
}

QPushButton* MainWindow::createPageButton(const QString& title) {
    QPushButton* button = new QPushButton();
    button->setText(title);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    button->setFixedHeight(40);
    button->setAutoFillBackground(true);
    return button;
}

void MainWindow::setupUi() {
    QHBoxLayout* mainContainerLayout = new QHBoxLayout();
    mainContainerLayout->setContentsMargins(0, 0, 0, 0);
    mainContainerLayout->setSpacing(0);

    mLeftRectanglesPageButton = createPageButton("Прямоугольники левых частей");
    mRightRectanglesPageButton = createPageButton("Прямоугольники правых частей");
    mTrapezoidPageButton = createPageButton("Трапеция");
    mSimpsonPageButton = createPageButton("Симпсон");
    mVariableStepPageButton = createPageButton("Переменный шаг");

    QLabel* menuContainerTitle = new QLabel("Методы");
    QPalette titlePalette = menuContainerTitle->palette();
    titlePalette.setColor(menuContainerTitle->foregroundRole(), Qt::white);
    menuContainerTitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    menuContainerTitle->setPalette(titlePalette);
    menuContainerTitle->setStyleSheet("QLabel{ font-size: 24px; }");
    menuContainerTitle->setAlignment(Qt::AlignCenter);

    QWidget* menuContainer = new QWidget();
    QPalette menuContainerPalette = menuContainer->palette();
    menuContainerPalette.setColor(QPalette::Window, QColor("#212121"));

    menuContainer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    menuContainer->setAutoFillBackground(true);
    menuContainer->setPalette(menuContainerPalette);
    menuContainer->setMaximumWidth(250);

    QVBoxLayout* menuLayout = new QVBoxLayout();
    menuLayout->setContentsMargins(10, 0, 10, 0);
    menuLayout->setSpacing(20);
    menuLayout->addStretch();
    menuLayout->addWidget(menuContainerTitle);
    menuLayout->addWidget(mLeftRectanglesPageButton);
    menuLayout->addWidget(mRightRectanglesPageButton);
    menuLayout->addWidget(mTrapezoidPageButton);
    menuLayout->addWidget(mSimpsonPageButton);
    menuLayout->addWidget(mVariableStepPageButton);
    menuLayout->addStretch();

    menuContainer->setLayout(menuLayout);

    mContentWidget = new QStackedWidget();
    mContentWidget->insertWidget(MethodTabs::LeftRectangles, new LeftRectanglesPage());
    mContentWidget->insertWidget(MethodTabs::RightRectangles, new RightRectanglesPage());
    mContentWidget->insertWidget(MethodTabs::Trapezoid, new TrapezoidPage());
    mContentWidget->insertWidget(MethodTabs::Simpson, new SimpsonPage());
    mContentWidget->insertWidget(MethodTabs::VariableStep, new VariableStepPage());

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

    QObject::connect(mVariableStepPageButton, &QPushButton::clicked, [this] {
        mContentWidget->setCurrentIndex(MethodTabs::VariableStep);
    });

    setCentralWidget(stackedWidget);
}
