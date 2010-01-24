/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "graphicsview.h"
#include <math.h>
#include <QWheelEvent>
#include <QtCore/QDateTime>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>


GraphicsView::GraphicsView(QWidget* parent): QGraphicsView(parent)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 1000, 1000);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    QGraphicsRectItem *rect = scene->addRect(QRectF(0, 0, 100, 100));

    for (int i=0; i<100; ++i) {
        QGraphicsEllipseItem *e = scene->addEllipse(QRectF(0, 0, 10, 10));
        e->setPos(qrand() % 1000, qrand() % 1000);
    }
     Node *node1 = new Node(this);
     Node *node2 = new Node(this);
     Node *node3 = new Node(this);
     Node *node4 = new Node(this);
     Node *node6 = new Node(this);
     Node *node7 = new Node(this);
     Node *node8 = new Node(this);
     Node *node9 = new Node(this);
     scene->addItem(node1);
     scene->addItem(node2);
     scene->addItem(node3);
     scene->addItem(node4);
     scene->addItem(node6);
     scene->addItem(node7);
     scene->addItem(node8);
     scene->addItem(node9);
     scene->addItem(new Edge(node1, node2));
     scene->addItem(new Edge(node2, node3));
     scene->addItem(new Edge(node3, node6));
     scene->addItem(new Edge(node4, node1));
     scene->addItem(new Edge(node6, node9));
     scene->addItem(new Edge(node7, node4));
     scene->addItem(new Edge(node8, node7));
     scene->addItem(new Edge(node9, node8));

     node1->setPos(-50, -50);
     node2->setPos(0, -50);
     node3->setPos(50, -50);
     node4->setPos(-50, 0);
     node6->setPos(50, 0);
     node7->setPos(-50, 50);
     node8->setPos(0, 50);
     node9->setPos(50, 50);

    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    //setDragMode(QGraphicsView::ScrollHandDrag);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
}

GraphicsView::~GraphicsView()
{
}

void GraphicsView::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        translate(0, -20);
        break;
    case Qt::Key_Down:
        translate(0, 20);
        break;
    case Qt::Key_Left:
        translate(-20, 0);
        break;
    case Qt::Key_Right:
        translate(20, 0);
        break;
    case Qt::Key_Plus:
        scaleView(qreal(1.2));
        break;
    case Qt::Key_Minus:
        scaleView(1 / qreal(1.2));
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}


void GraphicsView::mousePressEvent(QMouseEvent* event)
{
    if (event->button()==Qt::MidButton) {
        QMouseEvent e(event->type(), event->pos(), event->globalPos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
        setDragMode(QGraphicsView::ScrollHandDrag);
        QGraphicsView::mousePressEvent(&e);
        //event->accept();
    }
    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button()==Qt::MidButton) {
        QMouseEvent e(event->type(), event->pos(), event->globalPos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
        QGraphicsView::mouseReleaseEvent(&e);
        setDragMode(QGraphicsView::NoDrag);
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    event->accept();
    scaleView(pow((double)2, event->delta() / 240.0));
}

void GraphicsView::scaleView(qreal scaleFactor) {
    qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}



 Node::Node(GraphicsView *graphicsView) {
     setFlag(ItemIsMovable);
     //setFlag(ItemSendsGeometryChanges);
     setCacheMode(DeviceCoordinateCache);
     setZValue(-1);
 }

 void Node::addEdge(Edge *edge)
 {
     edgeList << edge;
     edge->adjust();
 }

 QList<Edge *> Node::edges() const
 {
     return edgeList;
 }

  QRectF Node::boundingRect() const
 {
     qreal adjust = 2;
     return QRectF(-10 - adjust, -10 - adjust,
                   23 + adjust, 23 + adjust);
 }

 QPainterPath Node::shape() const
 {
     QPainterPath path;
     path.addEllipse(-10, -10, 20, 20);
     return path;
 }

 void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
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

 QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
 {
     switch (change) {
     case ItemPositionHasChanged:
         foreach (Edge *edge, edgeList)
             edge->adjust();
         break;
     default:
         break;
     };

     return QGraphicsItem::itemChange(change, value);
 }

 void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mousePressEvent(event);
 }

 void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mouseReleaseEvent(event);
 }


static const double Pi = 3.14159265358979323846264338327950288419717;
 static double TwoPi = 2.0 * Pi;

 Edge::Edge(Node *sourceNode, Node *destNode)
     : arrowSize(10)
 {
     setAcceptedMouseButtons(0);
     source = sourceNode;
     dest = destNode;
     source->addEdge(this);
     dest->addEdge(this);
     adjust();
 }

 Edge::~Edge()
 {
 }

 Node *Edge::sourceNode() const
 {
     return source;
 }

 void Edge::setSourceNode(Node *node)
 {
     source = node;
     adjust();
 }

 Node *Edge::destNode() const
 {
     return dest;
 }

 void Edge::setDestNode(Node *node)
 {
     dest = node;
     adjust();
 }

 void Edge::adjust()
 {
     if (!source || !dest)
         return;

     QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
     qreal length = line.length();

     prepareGeometryChange();

     if (length > qreal(20.)) {
         QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
         sourcePoint = line.p1() + edgeOffset;
         destPoint = line.p2() - edgeOffset;
     } else {
         sourcePoint = destPoint = line.p1();
     }
 }

 QRectF Edge::boundingRect() const
 {
     if (!source || !dest)
         return QRectF();

     qreal penWidth = 1;
     qreal extra = (penWidth + arrowSize) / 2.0;

     return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                       destPoint.y() - sourcePoint.y()))
         .normalized()
         .adjusted(-extra, -extra, extra, extra);
 }

 void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
 {
     if (!source || !dest)
         return;

     QLineF line(sourcePoint, destPoint);
     if (qFuzzyCompare(line.length(), qreal(0.)))
         return;

     // Draw the line itself
     painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
     painter->drawLine(line);

     // Draw the arrows
     double angle = ::acos(line.dx() / line.length());
     if (line.dy() >= 0)
         angle = TwoPi - angle;

     QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize,
                                                   cos(angle + Pi / 3) * arrowSize);
     QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                                   cos(angle + Pi - Pi / 3) * arrowSize);
     QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                                               cos(angle - Pi / 3) * arrowSize);
     QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                               cos(angle - Pi + Pi / 3) * arrowSize);

     painter->setBrush(Qt::black);
     painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
     painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
 }


#include "moc_graphicsview.cpp"
