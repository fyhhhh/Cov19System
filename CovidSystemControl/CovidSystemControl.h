#ifndef QUICREATOR_H
#define QUICREATOR_H

#pragma execution_character_set("utf-8")

#include <QMainWindow>
#include <QString>
#include <QVector>

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
    void initForm();
    void initTableWidget();
    void initTreeWidget();
    void initListWidget();
    void initOther();
    void initStyle();
    void initTranslator();
    void treeClicked(QTreeWidgetItem* clickedItem, int index);

private:
    CovidInfoController* controller;
};

#endif // QUICREATOR_H