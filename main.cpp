#include <QApplication>
#include "graphwidget.h"

#include "main_window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Main_window window;
    window.show();
    return a.exec();
}
