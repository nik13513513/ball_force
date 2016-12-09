#include "view.h"
#include "force_item.h"
#include "ball_item.h"

#include <cmath>

#include <QMouseEvent>

#include <QDebug>


GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent),
      m_showForces(true)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-200, -200, 400, 400);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);

    setMinimumSize(400, 400);
    setWindowTitle(tr("Ball demo"));

    Ball *node1 = new Ball();
    Ball *node2 = new Ball();
    Ball *node3 = new Ball();
    Ball *node4 = new Ball();

    scene->addItem(node1);
    scene->addItem(node2);
    scene->addItem(node3);
    scene->addItem(node4);

    scene->addItem(new ForceItem(node1, node2));
    scene->addItem(new ForceItem(node1, node3));
    scene->addItem(new ForceItem(node1, node4));

    scene->addItem(new ForceItem(node2, node3));
    scene->addItem(new ForceItem(node2, node4));

    scene->addItem(new ForceItem(node2, node4));

    scene->addItem(new ForceItem(node3, node4));

    node1->setPos(-50, -50);
    node2->setPos(0, -50);
    node3->setPos(50, -50);
    node4->setPos(-50, 0);

    startTimer(100);
}

QList<Ball *> GraphicsView::ballsList()
{
    QList<Ball*> balls;

    foreach (QGraphicsItem *item, scene()->items()) {
        if (Ball *node = qgraphicsitem_cast<Ball *>(item))
            balls << node;
    }

    return balls;
}

void GraphicsView::showForces(bool _show)
{
    m_showForces = _show;

    foreach (QGraphicsItem* item, items()) {
        ForceItem* fi = qgraphicsitem_cast<ForceItem*>(item);
        if (fi)
            fi->setVisible(_show);
    }
}

void GraphicsView::addBall(const QPointF &_point, bool _showForces)
{
    Ball* ball= new Ball();
    ball->setPos(_point);


    foreach (Ball* item, ballsList()) {
        QGraphicsItem* force = new ForceItem(item, ball);
        force->setVisible(_showForces);
        scene()->addItem(force);
    }

    scene()->addItem(ball);
}

void GraphicsView::deleteItem(QGraphicsItem *ball)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    qDebug()<<"delete ball";

    delete ball;
}

void GraphicsView::calculateForces()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    qDebug()<<"in calc";

    QList<Ball*> balls = ballsList();

    foreach (Ball* ball, balls)
            ball->calculateForces();

    qDebug()<<"out calc";
}

void GraphicsView::mousePressEvent(QMouseEvent *_event)
{
    if (_event->button()==Qt::RightButton){
        addBall(mapToScene(_event->pos()),m_showForces);
    }

    QGraphicsView::mousePressEvent(_event);
}

void GraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QGraphicsItem* item = qgraphicsitem_cast<Ball*>(scene()->itemAt(mapToScene(event->pos()),QTransform()));
    if (item)
        deleteItem(item);
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}

void GraphicsView::drawBackground(QPainter *painter, const QRectF& rect)
{
    int shadow_size = 5;

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + shadow_size, shadow_size, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + shadow_size, sceneRect.bottom(), sceneRect.width(), shadow_size);

    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);


    QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
                    sceneRect.width() - 4, sceneRect.height() - 4);
    QString message(tr("Лопать - двойной клик, новый шар - правая кнопка мыши"));

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->setPen(Qt::lightGray);
    painter->drawText(textRect.translated(2, 2), message);
    painter->setPen(Qt::black);
    painter->drawText(textRect, message);
}

void GraphicsView::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void GraphicsView::shuffle()
{
    foreach (QGraphicsItem *item, scene()->items()) {
        Ball* ball = qgraphicsitem_cast<Ball *>(item);
        if (ball)
            ball->reposition(QPointF(-150 + qrand() % 300, -150 + qrand() % 300));
    }
}

void GraphicsView::zoomIn()
{
    scaleView(qreal(1.2));
}

void GraphicsView::zoomOut()
{
    scaleView(1 / qreal(1.2));
}

void GraphicsView::timerEvent(QTimerEvent *event)
{
    QList<Ball* > balls = ballsList();

    foreach (Ball* ball, balls)
        ball->reposition();

}
