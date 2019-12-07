#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QtGui>
#include <QGraphicsView>
#include <QLabel>
#include <QRectF>

#include "node.h"

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    QLabel* debug_label;
    GraphWidget(QWidget *parent = 0);
    void connectNode(Node* node);
    void itemMoved();
    void removeItem(QGraphicsItem* item_to_delete);
    QVector<QVector<int>> getConnectivityMatrix();

public slots:
    void paintEdges();

protected:

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);


    //void keyPressEvent(QKeyEvent *event);
    //void timerEvent(QTimerEvent *event);
    //void wheelEvent(QWheelEvent *event);
    //void drawBackground(QPainter *painter, const QRectF &rect);

    //void scaleView(qreal scaleFactor);

private:
    QGraphicsScene* scene;
    int new_node_number = 1;
    Node* new_edge_source = nullptr;
    int timerId;
};

#endif
