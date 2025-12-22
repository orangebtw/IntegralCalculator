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
    QPushButton* mLeftRectanglesPageButton = nullptr;
    QPushButton* mRightRectanglesPageButton = nullptr;
    QPushButton* mTrapezoidPageButton = nullptr;
    QPushButton* mSimpsonPageButton = nullptr;
    QPushButton* mEulerPageButton = nullptr;
    QPushButton* mRungeKuttaPageButton = nullptr;
    QPushButton* mApproxPageButton = nullptr;
    QPushButton* mMultipleSimpsonPageButton = nullptr;
    QPushButton* mNonlinearNewtonPageButton = nullptr;
    QPushButton* mNonlinearDichotomyPageButton = nullptr;
    QPushButton* mNonlinearSecantPageButton = nullptr;
};

#endif // APP_HPP_
