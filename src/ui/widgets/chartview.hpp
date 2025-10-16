#ifndef UI_CHARTVIEW_HPP_
#define UI_CHARTVIEW_HPP_

#include <QChartView>

class ChartView: public QtCharts::QChartView
{
    Q_OBJECT

public:
    ChartView(QtCharts::QChart* chart, QWidget *parent = 0);

protected:

    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;

private:
    QPointF mLastMousePos;
};

#endif
