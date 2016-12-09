#pragma once

#include <QGraphicsItem>

class Ball;

class ForceItem : public QGraphicsItem
{
public:
    ForceItem(Ball *sourceBall, Ball *destBall);
    ~ForceItem();

    Ball *sourceBall() const;
    Ball *destBall() const;

    void adjust();

    enum { Type = UserType + 2 };

    int type() const Q_DECL_OVERRIDE { return Type; }

protected:
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

private:
    Ball *source, *dest;

    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
};

