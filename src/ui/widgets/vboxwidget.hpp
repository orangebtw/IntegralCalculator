#ifndef UI_WIDGETS_VBOX_WIDGET_HPP_
#define UI_WIDGETS_VBOX_WIDGET_HPP_

#include <QWidget>
#include <QVBoxLayout>
#include <initializer_list>
#include <qmargins.h>
#include <qnamespace.h>

class VBoxWidget : public QWidget {
    Q_OBJECT;
public:
    struct Descriptor {
        QMargins margins = QMargins(0, 0, 0, 0);
        int spacing = 0;
        Qt::Alignment alignment = Qt::AlignTop;
        std::initializer_list<QWidget*> widgets = {};
    };
public:
    VBoxWidget(QWidget* parent = nullptr) : QWidget(parent) {
        mLayout = new QVBoxLayout();
        mLayout->setContentsMargins(0, 0, 0, 0);
        mLayout->setSpacing(0);
        mLayout->setAlignment(Qt::AlignTop);
        setLayout(mLayout);
    }

    VBoxWidget(Descriptor desc, QWidget* parent = nullptr) : QWidget(parent) {
        mLayout = new QVBoxLayout();
        mLayout->setContentsMargins(desc.margins);
        mLayout->setSpacing(desc.spacing);
        mLayout->setAlignment(desc.alignment);
        setLayout(mLayout);

        for (QWidget* widget : desc.widgets) {
            addWidget(widget);
        }
    }

    void addWidget(QWidget* widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment()) {
        mLayout->addWidget(widget, stretch, alignment);
    }

    void removeWidget(QWidget* widget) {
        mLayout->removeWidget(widget);
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
