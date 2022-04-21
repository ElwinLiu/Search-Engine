#include "Search_Engine.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Search_Engine w;
    w.show();
    return a.exec();
}
