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

#include "tabs/right_rectangles.hpp"
#include "tabs/trapezoid.hpp"
#include "tabs/left_rectangles.hpp"
#include "tabs/simpson.hpp"

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
        Simpson
    };
}

void MainWindow::setupUi() {
    QHBoxLayout* mainContainerLayout = new QHBoxLayout();
    mainContainerLayout->setContentsMargins(0, 0, 0, 0);
    mainContainerLayout->setSpacing(0);

    mLeftRectanglesTabButton = new QPushButton();
    mLeftRectanglesTabButton->setText("Прямоугольники левых частей");
    mLeftRectanglesTabButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mLeftRectanglesTabButton->setFixedHeight(40);
    mLeftRectanglesTabButton->setAutoFillBackground(true);

    mRightRectanglesTabButton = new QPushButton();
    mRightRectanglesTabButton->setText("Прямоугольники правых частей");
    mRightRectanglesTabButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mRightRectanglesTabButton->setFixedHeight(40);
    mRightRectanglesTabButton->setAutoFillBackground(true);

    mTrapezoidTabButton = new QPushButton();
    mTrapezoidTabButton->setText("Трапеция");
    mTrapezoidTabButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mTrapezoidTabButton->setFixedHeight(40);
    mTrapezoidTabButton->setAutoFillBackground(true);

    mSimpsonTabButton = new QPushButton();
    mSimpsonTabButton->setText("Симпсон");
    mSimpsonTabButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mSimpsonTabButton->setFixedHeight(40);
    mSimpsonTabButton->setAutoFillBackground(true);

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
    menuLayout->addWidget(mLeftRectanglesTabButton);
    menuLayout->addWidget(mRightRectanglesTabButton);
    menuLayout->addWidget(mTrapezoidTabButton);
    menuLayout->addWidget(mSimpsonTabButton);
    menuLayout->addStretch();

    menuContainer->setLayout(menuLayout);

    mContentWidget = new QStackedWidget();
    mContentWidget->insertWidget(MethodTabs::LeftRectangles, new LeftRectanglesTab());
    mContentWidget->insertWidget(MethodTabs::RightRectangles, new RightRectanglesTab());
    mContentWidget->insertWidget(MethodTabs::Trapezoid, new TrapezoidTab());
    mContentWidget->insertWidget(MethodTabs::Simpson, new SimpsonTab());

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

    QObject::connect(mLeftRectanglesTabButton, &QPushButton::clicked, [this] {
        mContentWidget->setCurrentIndex(MethodTabs::LeftRectangles);
    });

    QObject::connect(mRightRectanglesTabButton, &QPushButton::clicked, [this] {
        mContentWidget->setCurrentIndex(MethodTabs::RightRectangles);
    });

    QObject::connect(mTrapezoidTabButton, &QPushButton::clicked, [this] {
        mContentWidget->setCurrentIndex(MethodTabs::Trapezoid);
    });

    QObject::connect(mSimpsonTabButton, &QPushButton::clicked, [this] {
        mContentWidget->setCurrentIndex(MethodTabs::Simpson);
    });

    setCentralWidget(stackedWidget);
}
