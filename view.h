#pragma once

#include <QGraphicsView>
#include <mutex>


class Ball;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    GraphicsView(QWidget *parent = 0);

    QList<Ball*> ballsList();

    void showForces(bool _show);

    void addBall(const QPointF& _point,bool _showForces);

    void deleteItem(QGraphicsItem* ball);
    void calculateForces();


protected:
    void mousePressEvent(QMouseEvent* _event);
    void mouseDoubleClickEvent(QMouseEvent * event);

public slots:
    void shuffle();
    void zoomIn();
    void zoomOut();

protected:
    void timerEvent(QTimerEvent *event);
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;

    void scaleView(qreal scaleFactor);

private:
    bool m_showForces;
    std::mutex m_mutex;
};

