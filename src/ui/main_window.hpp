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
    QPushButton* createPageButton(const QString& title);

private:
    QStackedWidget* mContentWidget = nullptr;
    QPushButton* mLeftRectanglesPageButton = nullptr;
    QPushButton* mRightRectanglesPageButton = nullptr;
    QPushButton* mTrapezoidPageButton = nullptr;
    QPushButton* mSimpsonPageButton = nullptr;
};

#endif // APP_HPP_
