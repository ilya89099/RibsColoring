#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>
#include <QRectF>
#include <QPointF>

class Node;
class GraphWidget;

class Edge : public QGraphicsItem{
public:
    Edge(GraphWidget* new_graph, Node* source_node, Node* dest_node);
    ~Edge();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
    QRectF boundingRect() const;
    void adjust();
    enum { Type = UserType + 2 };
    int type() const { return Type; }


    Node* getSource();
    Node* getDest();
    void setColor(Qt::GlobalColor new_color);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);


private:

    Qt::GlobalColor edgeColor = Qt::black;
    Node *source, *dest;
    GraphWidget* graph;
    QPointF sourcePoint, destPoint;
};

#endif // EDGE_H
