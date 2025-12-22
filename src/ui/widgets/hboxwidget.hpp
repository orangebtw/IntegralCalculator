#ifndef UI_WIDGETS_HBOX_WIDGET_HPP_
#define UI_WIDGETS_HBOX_WIDGET_HPP_

#include <QWidget>
#include <QHBoxLayout>

class HBoxWidget : public QWidget {
    Q_OBJECT;

public:

    struct Descriptor {
        QMargins margins = QMargins(0, 0, 0, 0);
        int spacing = 0;
        Qt::Alignment alignment = Qt::AlignTop;
        std::initializer_list<QWidget*> widgets = {};
    };

public:
    HBoxWidget(Descriptor desc, QWidget* parent = nullptr) : QWidget(parent) {
        mLayout = new QHBoxLayout();
        mLayout->setContentsMargins(desc.margins);
        mLayout->setSpacing(desc.spacing);
        mLayout->setAlignment(desc.alignment);
        setLayout(mLayout);
        for (QWidget* widget : desc.widgets) {
            addWidget(widget);
        }
    }

    HBoxWidget(QWidget* parent = nullptr) : QWidget(parent) {
        mLayout = new QHBoxLayout();
        mLayout->setContentsMargins(0, 0, 0, 0);
        mLayout->setSpacing(0);
        mLayout->setAlignment(Qt::AlignLeft);
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
    QHBoxLayout* mLayout = nullptr;
};

#endif // !UI_WIDGETS_HBOX_WIDGET_HPP_
