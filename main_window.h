#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QWidget>
#include "graphwidget.h"
#include "ui_form.h"

namespace Ui {
class Form;
}

class Main_window : public QWidget {
    Q_OBJECT
public:
    Main_window(QWidget* parent = 0);
private:
    GraphWidget* graph;
    Ui::Form* ui;
};

#endif // MAIN_WINDOW_H
