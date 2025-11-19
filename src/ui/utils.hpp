#ifndef UI_UTILS_HPP_
#define UI_UTILS_HPP_

#include <QLabel>
#include <QString>
#include <qsizepolicy.h>

template <typename T>
inline void SetFontSize(T* widget, float fontSize) {
    QFont font = widget->font();
    font.setPointSizeF(fontSize);
    widget->setFont(font);
}

template <typename T>
inline void SetColor(T* widget, QPalette::ColorRole role, QColor color) {
    QPalette palette = widget->palette();
    palette.setColor(role, color);
    widget->setPalette(palette);
}

template <typename T>
inline void SetForegroundColor(T* widget, QColor color) {
    SetColor(widget, widget->foregroundRole(), color);
}

inline QLabel* CreateLabel(const QString& text, float fontSize, QSizePolicy sizePolicy = QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed)) {
    QLabel* label = new QLabel(text);
    label->setSizePolicy(sizePolicy);
    label->setAlignment(Qt::AlignCenter);
    SetFontSize(label, fontSize);
    return label;
}

template <typename Func1, typename T>
inline void BindVariable(const typename QtPrivate::FunctionPointer<Func1>::Object *sender, Func1 signal, T& variable) {
    QObject::connect(sender, signal, [&variable](const T& var) {
        variable = var;
    });
}

#endif // !UI_UTILS_HPP_
