#ifndef UI_PAGES_INTEGRAL_LEFT_RECTANGLES_TAB_HPP_
#define UI_PAGES_INTEGRAL_LEFT_RECTANGLES_TAB_HPP_

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QIntValidator>
#include <QButtonGroup>
#include <QRadioButton>

#include "base.hpp"

#include "../../widgets/vboxwidget.hpp"
#include "../../utils.hpp"

class LeftRectanglesPage final : public IntegralMethodPageBase {
    Q_OBJECT

public:
    LeftRectanglesPage(QWidget* parent = nullptr) : IntegralMethodPageBase("Метод прямоугольников левых частей", parent) {
        addBaseInputs();

        VBoxWidget* stepsContainer = new VBoxWidget();
        QIntValidator* stepsValidator = new QIntValidator(1, std::numeric_limits<int>::max());

        QLabel* stepsLabel = CreateLabel("Кол-во шагов:", 12.0f);
        mStepsAmountEdit = new QLineEdit();
        mStepsAmountEdit->setValidator(stepsValidator);
        stepsContainer->addWidget(stepsLabel);
        stepsContainer->addWidget(mStepsAmountEdit);
        stepsContainer->setContentMargins(0, 0, 0, 0);
        stepsContainer->setSpacing(5);

        VBoxWidget* epsilonInputContainer = new VBoxWidget();
        QDoubleValidator* epsilonValidator = new QDoubleValidator();
        epsilonValidator->setNotation(QDoubleValidator::StandardNotation);

        mAlgorithmGroup = new QButtonGroup();
        QRadioButton* algorithm1Button = new QRadioButton("Алгоритм 1");
        QRadioButton* algorithm2Button = new QRadioButton("Алгоритм 2");
        
        algorithm1Button->setChecked(true);

        SetFontSize(algorithm1Button, 12.0f);
        SetFontSize(algorithm2Button, 12.0f);

        mAlgorithmGroup->addButton(algorithm1Button, 0);
        mAlgorithmGroup->addButton(algorithm2Button, 1);

        VBoxWidget* container = new VBoxWidget();
        container->addWidget(algorithm1Button);
        container->addWidget(algorithm2Button);
        container->setContentMargins(0, 5, 0, 5);

        mEpsilonEdit = new QLineEdit();
        mEpsilonEdit->setValidator(epsilonValidator);
        epsilonInputContainer->addWidget(CreateLabel("Эпсилон:", 12.0f));
        epsilonInputContainer->addWidget(mEpsilonEdit);
        epsilonInputContainer->addWidget(container);
        epsilonInputContainer->setContentMargins(0, 0, 0, 0);
        epsilonInputContainer->setSpacing(5);

        addStepsInput(stepsContainer, epsilonInputContainer);
        addOutputs();
    }
    
    ~LeftRectanglesPage() = default;

private:
    virtual std::optional<CalculateResult> calculateWithFixedStep(double a, double b, unsigned n, const std::string& expr) override;
    virtual std::optional<CalculateResult> calculateWithVarStep(double a, double b, double eps, const std::string& expr) override;

private:
    QButtonGroup* mAlgorithmGroup = nullptr;
};

#endif // UI_PAGES_INTEGRAL_LEFT_RECTANGLES_TAB_HPP_
