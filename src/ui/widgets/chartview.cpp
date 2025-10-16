#include <QApplication>

#include "chartview.hpp"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    using namespace QtCharts;
#endif

ChartView::ChartView(QChart* chart, QWidget *parent) : QChartView(chart, parent) {
    setDragMode(QGraphicsView::NoDrag);
    setMouseTracking(true);
}

void ChartView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        mLastMousePos = event->pos();
        event->accept();
    }

    QChartView::mousePressEvent(event);
}

void ChartView::wheelEvent(QWheelEvent* event) {
    if (chart()) {
        event->accept();

        static constexpr qreal FACTOR = 1.001;

        const QRectF plotArea = chart()->plotArea();
        const qreal val = std::pow(FACTOR, -event->angleDelta().y());

        const QPointF scenePos = chart()->mapToScene(event->position());
        const QPointF chartPos = chart()->mapFromScene(scenePos);
        
        const qreal newWidth = plotArea.width() * val;
        const qreal newHeight = plotArea.height() * val;

        if (event->angleDelta().y() > 0) {
            const QPointF newTopLeft = plotArea.topLeft() + (chartPos - plotArea.topLeft()) * (1 - val);
            chart()->zoomIn(QRectF(newTopLeft, QSizeF(newWidth, newHeight)));
        } else {
            const QPointF newTopLeft = plotArea.topLeft() - (chartPos - plotArea.topLeft()) * (val - 1);
            chart()->zoomIn(QRectF(newTopLeft, QSizeF(newWidth, newHeight)));
        }

        chart()->zoomIn(plotArea);
    }

    QChartView::wheelEvent(event);
}

void ChartView::keyPressEvent(QKeyEvent* event) {
    if (chart() && event->key() == Qt::Key_Escape) {
        event->accept();
        chart()->zoomReset();
    }
    QChartView::keyPressEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event) {
    if (chart() && event->buttons() & Qt::LeftButton) {
        event->accept();
        const QRectF bounds = chart()->plotArea();

        const auto dPos = event->pos() - mLastMousePos;

        chart()->scroll(-dPos.x(), dPos.y());

        mLastMousePos = event->pos();
    }

    QChartView::mouseMoveEvent(event);
}
