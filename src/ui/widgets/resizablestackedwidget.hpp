#ifndef UI_WIDGETS_RESIZABLE_STACKED_WIDGET_HPP_
#define UI_WIDGETS_RESIZABLE_STACKED_WIDGET_HPP_

#include <QStackedWidget>
#include <qstackedwidget.h>

class ResizableStackedWidget : public QStackedWidget {
public:
    ResizableStackedWidget(QWidget* parent = nullptr) : QStackedWidget(parent) {
    }
    
    virtual QSize sizeHint() const override {
        return count() > 0 ? currentWidget()->sizeHint() : QWidget::sizeHint();
    }

    virtual QSize minimumSizeHint() const override {
        return count() > 0 ? currentWidget()->minimumSizeHint() : QWidget::minimumSizeHint();
    }
};

#endif