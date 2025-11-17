#ifndef UI_UTILS_HPP_
#define UI_UTILS_HPP_

#include <QLabel>
#include <QString>

template <typename T>
inline void SetFontSize(T* widget, float fontSize) {
    QFont font = widget->font();
    font.setPointSizeF(fontSize);
    widget->setFont(font);
}

template <typename T>
inline void SetForegroundColor(T* widget, QColor color) {
    QPalette palette = widget->palette();
    palette.setColor(widget->foregroundRole(), color);
    widget->setPalette(palette);
}

template <typename T>
inline void SetColor(T* widget, QPalette::ColorRole role, QColor color) {
    QPalette palette = widget->palette();
    palette.setColor(role, color);
    widget->setPalette(palette);
}

inline QLabel* CreateLabel(const QString& text, float fontSize) {
    QLabel* label = new QLabel(text);
    label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    SetFontSize(label, fontSize);
    return label;
}

#endif // !UI_UTILS_HPP_
