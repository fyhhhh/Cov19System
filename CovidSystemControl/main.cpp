#include "CovidSystemControl.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CovidSystemControl w;
    w.show();
    return a.exec();
}
