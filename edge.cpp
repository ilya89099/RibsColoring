#include "edge.h"
#include "node.h"

#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include "graphwidget.h"

Edge::Edge(GraphWidget* new_graph,Node* source_node, Node* dest_node)
    : source(source_node), dest(dest_node), graph(new_graph) {
    setZValue(-2);
    source->attachEdge(this);
    dest->attachEdge(this);
    source->attachNode(dest);
    dest->attachNode(source);
    adjust();
}

Node* Edge::getSource() {
    return source;
}

Node* Edge::getDest() {
    return dest;
}

Edge::~Edge() {
    graph->removeItem(this);
    source->deleteAttachedEdge(this);
    dest->deleteAttachedEdge(this);

}

void Edge::adjust()
{
    if (!source || !dest)
        return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    //if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
    //} else {
    //    sourcePoint = destPoint = line.p1();
    //}
}

QRectF Edge::boundingRect() const
{
    if (!source || !dest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth);

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void Edge::setColor(Qt::GlobalColor new_color) {
    edgeColor = new_color;
    update();
}

void Edge::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        delete(this);
    }
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself
    painter->setPen(QPen(edgeColor, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);
}
