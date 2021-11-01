#include "CovidSystemControl.h"
#include <QtWidgets/QApplication>
#include "Information.hpp"
#include "CovidInfoController.h"

const QVector<QString> Abnormal::names = QVector<QString>({ "Province","City","County","District","Info" });
const QVector<QString> Area::names = QVector<QString>({ "Province","City","County" });

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setFont(QFont(QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont("./other/ttf/RuiZi.ttf")).at(0), 9));
    a.setWindowIcon(QIcon("./other/main.ico"));

    CovidInfoController c;

    CovidSystemControl w(&c);

    w.showFullScreen();

    return a.exec();
}
