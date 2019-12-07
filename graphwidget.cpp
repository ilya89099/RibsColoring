#include "graphwidget.h"
#include "node.h"
#include "edge.h"
#include <QtGui>
#include <QPair>
#include <fstream>
#include <vector>
#include <set>
#include <math.h>

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent), timerId(0)
{
    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-200, -200, 400, 400);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setMinimumSize(400, 400);
    setWindowTitle(tr("Окраска графа"));
//    Node* node1 = new Node(this, new_node_number++);
//    Node* node2 = new Node(this, new_node_number++);
//    scene->addItem(node1);
//    scene->addItem(node2);
//    node1->setPos(-30,10);
//    node2->setPos(130,0);
//    Edge* edge = new Edge(this,node1,node2);
//    scene->addItem(edge);
}

//void GraphWidget::itemMoved()
//{
//    if (!timerId)
//        timerId = startTimer(1000 / 25);
//}

//void GraphWidget::keyPressEvent(QKeyEvent *event)
//{

//}

void GraphWidget::removeItem(QGraphicsItem* item_to_delete) {
    scene->removeItem(item_to_delete);
    if (qgraphicsitem_cast<Node*>(item_to_delete) != nullptr) {
        int number = qgraphicsitem_cast<Node*>(item_to_delete)->getNumber();
        new_node_number--;
        foreach (QGraphicsItem* item, scene->items()) {
            Node* casted_item = qgraphicsitem_cast<Node*>(item);
            if (casted_item != nullptr) {
                if (casted_item->getNumber() > number) {
                    casted_item->setNumber(casted_item->getNumber() - 1);
                }
            }
        }
    }
}

void GraphWidget::connectNode(Node* node) {
    if (new_edge_source == nullptr) {
        new_edge_source = node;
        node->setColor(Qt::blue);
        return;
    }
    if (new_edge_source == node) {
        new_edge_source->setDefaultColor();
        return;
    }
    foreach (Node* cur_node, new_edge_source->getAttachedNodes()) {
        if (cur_node == node) {
            return;
        }
    }
    new_edge_source->setDefaultColor();
    Edge* new_edge = new Edge(this, new_edge_source, node);
    scene->addItem(new_edge);
    new_edge_source = nullptr;

}



std::vector<std::vector<bool>> getGraphMatrix(int size, QList<Edge*> edges) {
    std::vector<std::vector<bool>> matrix(size, std::vector<bool>(size, false));
    foreach(Edge* edge, edges) {
        matrix[edge->getSource()->getNumber()-1][edge->getDest()->getNumber()-1] = true;
        matrix[edge->getDest()->getNumber()-1][edge->getSource()->getNumber()-1] = true;
    }
    return matrix;
}

struct node {
    int node_number;
    int power = 0;
    int color = -1;
};

std::vector<int> getNodePaint(std::vector<std::vector<bool>> matrix) {
    std::vector<node> powers(matrix.size());
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = i; j < matrix.size(); ++j) {
            powers[i].node_number = i;
            powers[j].node_number = j;
            if (matrix[i][j]) {
                powers[i].power++;
                powers[j].power++;
            }
        }
    }
    std::sort(powers.begin(), powers.end(), [](node a, node b){return a.power > b.power;});
//    for (int i = 0; i < powers.size(); ++i) {
//        std::cout << powers[i].node_number << " " << powers[i].power << std::endl;
//    }
    int last_color = 0;
    std::set<int> same_painted;
    std::vector<int> result;
    for (int i = 0; i < powers.size(); ++i) {
        if (powers[i].color == -1) {
            powers[i].color = last_color;
            same_painted.insert(powers[i].node_number);
        } else {
            continue;
        }
        for (int j = i; j < powers.size(); j++) {
            if (powers[j].color != -1) {
                continue;
            }
            bool connected = false;
            for (int k : same_painted) {
                if (matrix[powers[j].node_number][k]) {
                    connected = true;
                    break;
                }
            }
            if (!connected) {
                same_painted.insert(powers[j].node_number);
                powers[j].color = powers[i].color;
            }
        }
        last_color++;
        same_painted.clear();
    }

    std::sort(powers.begin(), powers.end(), [](node a, node b){return a.node_number < b.node_number;});
    for (int i = 0; i < powers.size(); ++i) {
        result.push_back(powers[i].color);
    }
    return result;
}

std::vector<std::pair<int, int>> getMatrixPairEdges(std::vector<std::vector<bool>> input_matrix) {
    std::vector<std::pair<int, int>> edges_for_matrix;
    for (int i = 0; i < input_matrix.size(); ++i) {
        for (int k = i; k < input_matrix.size(); ++k) {
            if (input_matrix[i][k]) {
                edges_for_matrix.push_back({i,k});
            }
        }
    }
    return edges_for_matrix;
}

std::vector<std::vector<bool>> translateGraphMatrix(std::vector<std::vector<bool>> input_matrix) {
    std::vector<std::pair<int, int>> edges_for_matrix = getMatrixPairEdges(input_matrix);
//    for (int i = 0; i < edges_for_matrix.size(); ++i) {
//        std::cout << "{" << edges_for_matrix[i].first << " " << edges_for_matrix[i].second << "} ";
//    }
//    std::cout << std::endl;
    std::vector<std::vector<bool>> matrix(edges_for_matrix.size(), std::vector<bool>(edges_for_matrix.size(), false));
    for (int i = 0; i < edges_for_matrix.size() - 1; ++i) {
        for (int j = i + 1; j < edges_for_matrix.size(); ++j) {
            std::pair<int, int> edge1 = edges_for_matrix[i],edge2 = edges_for_matrix[j];
            if (edge1.first == edge2.first ||
                edge1.first == edge2.second ||
                edge1.second == edge2.first ||
                edge1.second == edge2.second) {
                matrix[i][j] = true;
                matrix[j][i] = true;
            }
        }
    }
    return matrix;
}



void GraphWidget::paintEdges() {
    QList<Node*> nodes;
    QList<Edge*> edges;
    QVector<Qt::GlobalColor> colors ={Qt::red, Qt::yellow, Qt::blue, Qt::green, Qt::cyan,
                                      Qt::magenta, Qt::darkRed, Qt::darkYellow, Qt::darkBlue, Qt::darkGreen, Qt::darkMagenta};
    QMap<std::pair<int,int>, Edge*> edge_to_ptr;
    foreach (QGraphicsItem* item, scene->items()) {
        if (qgraphicsitem_cast<Node*>(item) != nullptr) {
            nodes << qgraphicsitem_cast<Node*>(item);

        }
        if (qgraphicsitem_cast<Edge*>(item) != nullptr) {
            Edge* edge = qgraphicsitem_cast<Edge*>(item);
            std::pair<int,int> p1 = {edge->getSource()->getNumber() - 1, edge->getDest()->getNumber() - 1},
                           p2 = {edge->getDest()->getNumber() - 1, edge->getSource()->getNumber() - 1};
            edge_to_ptr[p1] = edge;
            edge_to_ptr[p2] = edge;
            edges << qgraphicsitem_cast<Edge*>(item);
        }
    }
    if (edges.empty()) {
        return;
    }
    debug_label->clear();
    debug_label->setText(debug_label->text() + "Матрица смежности исходного графа\n");
    std::vector<std::vector<bool>> graphMatrix = getGraphMatrix(nodes.size(), edges);
    for (int i = 0; i < graphMatrix.size(); ++i) {
        for (int j = 0; j < graphMatrix.size(); ++j) {
            debug_label->setText(debug_label->text() + QString::number(graphMatrix[i][j]) + " ");
        }
        debug_label->setText(debug_label->text() + "\n");
    }
    debug_label->setText(debug_label->text() + "\nМатрица смежности преобразованного графа\n");
    std::vector<std::pair<int, int>> edgesgraph = getMatrixPairEdges(getGraphMatrix(nodes.size(), edges));
//    for (int i = 0; i < edgesgraph.size(); ++i) {
//        debug_label->setText(debug_label->text() + "{" +  QString::number(edgesgraph[i].first + 1) + " " + QString::number(edgesgraph[i].second + 1) + "}");
//    }
//    debug_label->setText(debug_label->text() + "\n\n");
    std::vector<std::vector<bool>> graphMatrix2 = translateGraphMatrix(getGraphMatrix(nodes.size(), edges));
    for (int i = 0; i < graphMatrix2.size(); ++i) {
        for (int j = 0; j < graphMatrix2.size(); ++j) {
            debug_label->setText(debug_label->text() + QString::number(graphMatrix2[i][j]) + " ");
        }
        debug_label->setText(debug_label->text() + "\n");
    }
    debug_label->setText(debug_label->text() + "\n");
    std::vector<int> paint = getNodePaint(translateGraphMatrix(getGraphMatrix(nodes.size(), edges)));
    for (int i = 0; i < edgesgraph.size(); ++i) {
        if (edge_to_ptr[edgesgraph[i]] != 0) {
            edge_to_ptr[edgesgraph[i]]->setColor(colors[paint[i]]);
        }
    }
}

QVector<QVector<int>> GraphWidget::getConnectivityMatrix() {
    QList<Node*> nodes;
    QList<Edge*> edges;
    foreach (QGraphicsItem* item, scene->items()) {
        if (qgraphicsitem_cast<Node*>(item) != nullptr) {
            nodes << qgraphicsitem_cast<Node*>(item);
        }
        if (qgraphicsitem_cast<Edge*>(item) != nullptr) {
            edges << qgraphicsitem_cast<Edge*>(item);
        }
    }
    QVector<QVector<int>> matrix(nodes.size(),QVector<int>(nodes.size(),0));
    foreach (Edge* edge, edges) {
        matrix[edge->getSource()->getNumber()][edge->getDest()->getNumber()] = 1;
        matrix[edge->getDest()->getNumber()][edge->getSource()->getNumber()] = 1;
    }
    return matrix;
}

void GraphWidget::mousePressEvent(QMouseEvent* event) {
    if (qgraphicsitem_cast<Node*>(itemAt(event->pos())) != 0) {
        QGraphicsView::mousePressEvent(event);
        return;
    } else  {
        Node* new_node = new Node(this, new_node_number);
        new_node_number++;
        scene->addItem(new_node);
        new_node->setPos(mapToScene(event->pos()));
    }
}

void GraphWidget::mouseMoveEvent(QMouseEvent* event) {
    QGraphicsView::mouseMoveEvent(event);
}

void GraphWidget::mouseReleaseEvent(QMouseEvent* event) {
    QGraphicsView::mouseReleaseEvent(event);
}

