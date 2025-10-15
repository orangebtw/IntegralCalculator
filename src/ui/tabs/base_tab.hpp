#ifndef UI_TABS_BASE_TAB_HPP_
#define UI_TABS_BASE_TAB_HPP_

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class MethodTab : public QWidget {
    Q_OBJECT
public:
    MethodTab(const QString& title, QWidget* parent = nullptr) : QWidget(parent) {
        setupUi(title);
    }

protected:
    void setupUi(const QString& title);
    
    virtual bool calculate(double a, double b, unsigned n, const std::string& expr, double& result) = 0;

protected:
    QLineEdit* mLowerBoundEdit = nullptr;
    QLineEdit* mUpperBoundEdit = nullptr;
    QLineEdit* mStepsAmountEdit = nullptr;
    QLineEdit* mExpressionEdit = nullptr;
    QPushButton* mCalculateButton = nullptr;
    QLabel* mResultLabel = nullptr;
};

#endif