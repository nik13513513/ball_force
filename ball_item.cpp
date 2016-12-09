#include "force_item.h"
#include "ball_item.h"
#include "view.h"

#include <QGraphicsScene>
#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QObject>
#include <QStyleOption>
#include <QPropertyAnimation>


Ball::Ball():QObject(),QGraphicsItem()
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

Ball::~Ball()
{
    QList<ForceItem *> tempList = m_forceList;

    qDeleteAll(tempList);
}

void Ball::addForce(ForceItem *edge)
{
    m_forceList << edge;
    edge->adjust();
}

void Ball::removeForce(ForceItem *edge)
{
    m_forceList.removeAll(edge);
}

QList<ForceItem *> Ball::forces() const
{
    return m_forceList;
}

void Ball::calculateForces()
{
    if (!scene() || scene()->mouseGrabberItem() == this) {
        m_newPos = pos();
        return;
    }

    qreal xdelta = 0;
    qreal ydelta = 0;

    foreach (ForceItem *edge, m_forceList) {
        QPointF otherCoord;
        if (edge->sourceBall() == this)
            otherCoord = edge->destBall()->pos();
        else
            otherCoord = edge->sourceBall()->pos();

        QPointF this_coord = pos();

        m_r = sqrt(pow(otherCoord.x() -  this_coord.x(), 2) + pow(otherCoord.y() - this_coord.y(), 2));

        if (m_r < 0.001)
             return;

         double force = 1/m_r - 1/(m_r*m_r);

         double time = 1000/50;

         xdelta += (force*((otherCoord.x() - this_coord.x())/m_r)*time);
         ydelta += (force*((otherCoord.y() - this_coord.y())/m_r)*time);
     }

    if (qAbs(xdelta) < 0.1 && qAbs(ydelta) < 0.1)
        xdelta = ydelta = 0;

    QRectF sceneRect = scene()->sceneRect();
    m_newPos = pos() + QPointF(xdelta, ydelta);
    m_newPos.setX(qMin(qMax(m_newPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
    m_newPos.setY(qMin(qMax(m_newPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));
}

void Ball::reposition()
{
    if (qFuzzyCompare(m_newPos.x() ,pos().x())&&
        qFuzzyCompare(m_newPos.y() ,pos().y()))
        return;

    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");

    animation->setEndValue(m_newPos);
    animation->setDuration(100);
    animation->setEasingCurve(QEasingCurve::Linear);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Ball::reposition(const QPointF &_pos)
{
    m_newPos = _pos;
    reposition();
}

QRectF Ball::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

QPainterPath Ball::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void Ball::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7, -7, 20, 20);

    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
    } else {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }
    painter->setBrush(gradient);

    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);
}

QVariant Ball::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (ForceItem *force, m_forceList)
            force->adjust();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Ball::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();

    QGraphicsItem::mousePressEvent(event);
}

void Ball::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();

    QGraphicsItem::mouseReleaseEvent(event);
}

double Ball::r() const
{
    return m_r;
}
