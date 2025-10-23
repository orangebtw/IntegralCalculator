#ifndef UI_WIDGETS_VBOX_WIDGET_HPP_
#define UI_WIDGETS_VBOX_WIDGET_HPP_

#include <QWidget>
#include <QVBoxLayout>

class VBoxWidget : public QWidget {
    Q_OBJECT;

public:
    VBoxWidget(QWidget* parent = nullptr) : QWidget(parent) {
        mLayout = new QVBoxLayout();
        mLayout->setContentsMargins(0, 0, 0, 0);
        mLayout->setSpacing(0);
        mLayout->setAlignment(Qt::AlignTop);
        setLayout(mLayout);
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
    QVBoxLayout* mLayout = nullptr;
};

#endif // !UI_WIDGETS_HBOX_WIDGET_HPP_
