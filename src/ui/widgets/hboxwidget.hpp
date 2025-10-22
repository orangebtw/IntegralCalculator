#ifndef UI_WIDGETS_HBOX_WIDGET_HPP_
#define UI_WIDGETS_HBOX_WIDGET_HPP_

#include <QWidget>
#include <QHBoxLayout>

class HBoxWidget : public QWidget {
    Q_OBJECT;

public:
    HBoxWidget(QWidget* parent = nullptr) : QWidget(parent) {
        mLayout = new QHBoxLayout();
        setLayout(mLayout);
        setContentsMargins(0, 0, 0, 0);
        setSpacing(0);
    }

    void addWidget(QWidget* widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment()) {
        mLayout->addWidget(widget, stretch, alignment);
    }

    void setContentMargins(int left, int top, int right, int bottom) {
        mLayout->setContentsMargins(left, top, right, bottom);
    }

    void setSpacing(int spacing) {
        mLayout->setSpacing(spacing);
    }

private:
    QHBoxLayout* mLayout = nullptr;
};

#endif // !UI_WIDGETS_HBOX_WIDGET_HPP_
