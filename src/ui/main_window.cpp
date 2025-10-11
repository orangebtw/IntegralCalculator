#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QSizePolicy>
#include <QPalette>
#include <QLabel>
#include <QBoxLayout>

#include "main_window.hpp"

#include "tabs/right_rectangles.hpp"
#include "tabs/trapezoid.hpp"
#include "tabs/left_rectangles.hpp"

MainWindow::MainWindow() : QMainWindow() {
    setWindowTitle("Калькулятор интегралов");
    setMinimumSize(800, 600);
    setupUi();
}

void MainWindow::setupUi() {
    m_main_container_layout = new QHBoxLayout();
    m_main_container_layout->setContentsMargins(0, 0, 0, 0);
    m_main_container_layout->setSpacing(0);

    m_left_rectangles_tab_button = new QPushButton();
    m_left_rectangles_tab_button->setText("Прямоугольники левых частей");
    m_left_rectangles_tab_button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_left_rectangles_tab_button->setFixedHeight(40);

    m_right_rectangles_tab_button = new QPushButton();
    m_right_rectangles_tab_button->setText("Прямоугольники правых частей");
    m_right_rectangles_tab_button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_right_rectangles_tab_button->setFixedHeight(40);

    m_trapezoid_tab_button = new QPushButton();
    m_trapezoid_tab_button->setText("Трапеция");
    m_trapezoid_tab_button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_trapezoid_tab_button->setFixedHeight(40);

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
    menuLayout->addWidget(m_left_rectangles_tab_button);
    menuLayout->addWidget(m_right_rectangles_tab_button);
    menuLayout->addWidget(m_trapezoid_tab_button);
    menuLayout->addStretch();

    menuContainer->setLayout(menuLayout);

    m_content_widget.reset(new LeftRectanglesTab()); 

    m_main_container_layout->addWidget(menuContainer);
    m_main_container_layout->addWidget(m_content_widget.get());

    QWidget* mainContainer = new QWidget();
    mainContainer->setLayout(m_main_container_layout);

    QObject::connect(m_left_rectangles_tab_button, &QPushButton::clicked, [this]() {
        m_content_widget.reset(new LeftRectanglesTab());
        m_main_container_layout->addWidget(m_content_widget.get());
    });

    QObject::connect(m_right_rectangles_tab_button, &QPushButton::clicked, [this]() {
        m_content_widget.reset(new RightRectanglesTab());
        m_main_container_layout->addWidget(m_content_widget.get());
    });

    QObject::connect(m_trapezoid_tab_button, &QPushButton::clicked, [this]() {
        m_content_widget.reset(new TrapezoidTab());
        m_main_container_layout->addWidget(m_content_widget.get());
    });

    setCentralWidget(mainContainer);
}
