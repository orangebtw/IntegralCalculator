#ifndef APP_HPP_
#define APP_HPP_

#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStackedWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(); 
    ~MainWindow() {};

private:
    void setupUi();

private:
    QStackedWidget* mContentWidget = nullptr;
    QPushButton* mLeftRectanglesTabButton = nullptr;
    QPushButton* mRightRectanglesTabButton = nullptr;
    QPushButton* mTrapezoidTabButton = nullptr;
    QPushButton* mSimpsonTabButton = nullptr;
};

#endif // APP_HPP_
