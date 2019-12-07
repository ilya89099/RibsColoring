#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>

class Edge;
class GraphWidget;
class QGraphicsSceneMouseEvent;

class Node : public QGraphicsItem
{
public:
    Node(GraphWidget *graphWidget, int number);
    ~Node();

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    bool advance();

    void attachNode(Node* node);
    QList<Node*> getAttachedNodes();
    void deleteAttachedNode(Node* node_to_delete);
    int getAttachedNodesCount();

    void attachEdge(Edge* new_edge);
    QList<Edge*> getAttachedEdges();
    void deleteAttachedEdge(Edge* edge_to_delete);
    int getAttachedEdgesCount();

    void setColor(Qt::GlobalColor new_color);
    void setDefaultColor();
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int getNumber();
    void setNumber(int new_number);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:

    Qt::GlobalColor nodeColor = Qt::red;
    QPointF newPos;
    GraphWidget *graph;
    int nodeNumber;
    QList<Edge*> attachedEdges;
    QList<Node*> attachedNodes;
};

#endif
