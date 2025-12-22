#ifndef UI_WIDGETS_HTML_HEADER_VIEW
#define UI_WIDGETS_HTML_HEADER_VIEW

#include <QHeaderView>
#include <QPainter>
#include <QTextDocument>
#include <qnamespace.h>

class HtmlHeaderView : public QHeaderView { 
public:
    explicit HtmlHeaderView(Qt::Orientation orientation, QWidget *parent = nullptr) : QHeaderView(orientation, parent) {}

protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override {
        if (!rect.isValid())
            return;

        painter->save();

        QStyleOptionHeader opt;
        initStyleOptionForIndex(&opt, logicalIndex);
        opt.rect = rect;
        style()->drawControl(QStyle::CE_HeaderSection, &opt, painter, this);

        QString text = model()->headerData(logicalIndex, orientation(), Qt::DisplayRole).toString();
        QTextDocument doc;
        doc.setHtml(text);
        doc.setTextWidth(rect.width());

        painter->translate(rect.left(), rect.top());

        QRect clip(0, 0, opt.rect.width(), opt.rect.height());
        doc.drawContents(painter, clip);

        painter->restore();
    }
};

#endif