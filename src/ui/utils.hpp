#ifndef UI_UTILS_HPP_

#include <QLabel>
#include <QString>

template <typename T>
inline void SetFont(T* widget, float fontSize) {
    QFont font = widget->font();
    font.setPointSizeF(fontSize);
    widget->setFont(font);
}

inline QLabel* CreateLabel(const QString& text, float fontSize) {
    QLabel* label = new QLabel(text);
    SetFont(label, fontSize);
    return label;
}

#endif // !UI_UTILS_HPP_
