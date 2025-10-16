#ifndef UI_CHARTVIEW_HPP_
#define UI_CHARTVIEW_HPP_

#include <QChartView>
#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    using QtCharts::QChartView;
    using QtCharts::QChart;
#endif

class ChartView: public QChartView
{
    Q_OBJECT

public:
    ChartView(QChart* chart, QWidget *parent = 0);

protected:

    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;

private:
    QPointF mLastMousePos;
};

#endif
