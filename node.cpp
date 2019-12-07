#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <algorithm>

#include "node.h"
#include "edge.h"
#include "graphwidget.h"

Node::Node(GraphWidget *graphWidget, int number)
    : graph(graphWidget),
      nodeNumber(number)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

Node::~Node() {
    foreach (Edge* edge, attachedEdges) {
        Node* connected_node = edge->getSource() == this ? edge->getDest() : edge->getSource();
        connected_node->deleteAttachedNode(connected_node);
        delete edge;
    }
    graph->removeItem(this);
}

QList<Edge*> Node::getAttachedEdges() {
    return attachedEdges;
}

int Node::getNumber() {
    return nodeNumber;
}

void Node::setNumber(int new_number) {
    nodeNumber = new_number;
    update();
}

bool Node::advance()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}

int Node::getAttachedEdgesCount() {
    return attachedEdges.size();
}

int Node::getAttachedNodesCount() {
    return attachedNodes.size();
}

QRectF Node::boundingRect() const
{

    return QRectF(-15, -15,
                  30, 30);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-15, -15, 30, 30);
    return path;
}

void Node::setDefaultColor() {
    setColor(Qt::red);
}


void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    Q_UNUSED(option)
    painter->setPen(Qt::SolidLine);
    painter->setBrush(nodeColor);
    painter->drawEllipse(-15, -15, 30, 30);
    painter->setFont(QFont("Times", 12, QFont::Bold));
    painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(nodeNumber));

}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        //graph->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    foreach (Edge* cur_edge, attachedEdges) {
        cur_edge->adjust();
    }
    QGraphicsItem::mouseMoveEvent(event);
}

QList<Node*> Node::getAttachedNodes() {
    return attachedNodes;
}

void Node::attachNode(Node* node) {
    attachedNodes << node;
}

void Node::setColor(Qt::GlobalColor new_color) {
    nodeColor = new_color;
    update();
}

void Node::attachEdge(Edge* new_edge) {
    attachedEdges << new_edge;
}

void Node::deleteAttachedNode(Node* node_to_delete) {
    auto it = std::find(attachedNodes.begin(),attachedNodes.end(), node_to_delete);
    if (it != attachedNodes.end()) {
        attachedNodes.erase(it);
    }
}

void Node::deleteAttachedEdge(Edge* edge_to_delete) {
    auto it = std::find(attachedEdges.begin(),attachedEdges.end(), edge_to_delete);
    if (it != attachedEdges.end()) {
        attachedEdges.erase(it);
    }
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        delete this;
        return;
    }
    setCursor(Qt::ClosedHandCursor);
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    update();
    foreach (Edge* cur_edge, attachedEdges) {
        cur_edge->adjust();
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event);
    graph->connectNode(this);
}
