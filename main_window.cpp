#include "main_window.h"

Main_window::Main_window(QWidget* parent) : ui(new Ui::Form) {
    Q_UNUSED(parent);
    ui->setupUi(this);
    graph = ui->graphWidget;
    graph->debug_label = ui->label;
    connect(ui->paintBtn, SIGNAL(clicked()), graph, SLOT(paintEdges()));
    this->setWindowTitle(tr("Раскраска ребер графа"));
}
