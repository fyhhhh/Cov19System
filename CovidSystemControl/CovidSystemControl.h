#ifndef QUICREATOR_H
#define QUICREATOR_H

#pragma execution_character_set("utf-8")

#include <QMainWindow>
#include <QString>
#include <QVector>
#include <QFontDatabase>
#include <QFile>
#include <QTranslator>
#include <QDesktopWidget>

#include "ui_CovidSystemControl.h"
#include "Information.hpp"
#include "CovidInfoController.h"


class CovidSystemControl : public QMainWindow
{
    Q_OBJECT

public:
    CovidSystemControl(CovidInfoController* covidInfoController, QWidget* parent = 0);
    ~CovidSystemControl();
    Ui::CovidSystemControl *ui;

private slots:
    void initTableWidget();
    void initTreeWidget();
    void initListWidget();
    void initStyle();
    void initTranslator();
    void treeClicked(QTreeWidgetItem* clickedItem, int index);
    void listChanged(int row);
    void tabChanged(int index);
    inline void listShow();
    inline void treeShow();
    inline void closeAll() { this->close(); }

private:
    CovidInfoController* controller;
    QVector<Abnormal*>* select;
    QString RuiZi;
    Abnormal* now;
    QPushButton* closeButton = NULL;
};

#endif // QUICREATOR_H