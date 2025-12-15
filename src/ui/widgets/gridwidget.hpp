#ifndef UI_WIDGETS_GRID_WIDGET_HPP_
#define UI_WIDGETS_GRID_WIDGET_HPP_

#include <QWidget>
#include <QGridLayout>

class GridWidget : public QWidget {
    Q_OBJECT;

public:
    GridWidget(QWidget* parent = nullptr) : QWidget(parent) {
        mLayout = new QGridLayout();
        mLayout->setContentsMargins(0, 0, 0, 0);
        mLayout->setSpacing(0);
        setLayout(mLayout);
    }

    void addWidget(QWidget* widget, int row, int column, Qt::Alignment alignment = Qt::Alignment()) {
        mLayout->addWidget(widget, row, column, alignment);
    }

    void setContentMargins(int left, int top, int right, int bottom) {
        mLayout->setContentsMargins(left, top, right, bottom);
    }

    void setHorizontalSpacing(int spacing) {
        mLayout->setHorizontalSpacing(spacing);
    }

    int horizontalSpacing() const {
        return mLayout->horizontalSpacing();
    }

    void setVerticalSpacing(int spacing) {
        mLayout->setVerticalSpacing(spacing);
    }

    int verticalSpacing() const {
        return verticalSpacing();
    }

    void setSpacing(int spacing) {
        mLayout->setSpacing(spacing);
    }

    int spacing() const {
        return mLayout->spacing();
    }

    void setRowStretch(int row, int stretch) {
        mLayout->setRowStretch(row, stretch);
    }

    void setColumnStretch(int column, int stretch) {
        mLayout->setColumnStretch(column, stretch);
    }

    int rowStretch(int row) const {
        return mLayout->rowStretch(row);
    }

    int columnStretch(int column) const {
        return mLayout->columnStretch(column);
    }

    void setRowMinimumHeight(int row, int minSize) {
        mLayout->setRowMinimumHeight(row, minSize);
    }

    void setColumnMinimumWidth(int column, int minSize) {
        mLayout->setColumnMinimumWidth(column, minSize);
    }

    int rowMinimumHeight(int row) const {
        return mLayout->rowMinimumHeight(row);
    }

    int columnMinimumWidth(int column) const {
        return mLayout->columnMinimumWidth(column);
    }

    int columnCount() const {
        return mLayout->columnCount();
    }

    int rowCount() const {
        return mLayout->rowCount();
    }

    void hideRow(int row) {
        for (int col = 0; col < mLayout->columnCount(); ++col) {
            QLayoutItem* item = mLayout->itemAtPosition(row, col);
            if (item) {
                QWidget* widget = item->widget();
                if (widget) {
                    widget->hide();
                }
            }
        }
    }

    void showRow(int row) {
        for (int col = 0; col < mLayout->columnCount(); ++col) {
            QLayoutItem* item = mLayout->itemAtPosition(row, col);
            if (item) {
                QWidget* widget = item->widget();
                if (widget) {
                    widget->show();
                }
            }
        }
    }

private:
    QGridLayout* mLayout = nullptr;
};

#endif // !UI_WIDGETS_GRID_WIDGET_HPP_
