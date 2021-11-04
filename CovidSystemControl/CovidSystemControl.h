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


signals:
    void initTreeFinished();

private slots:
    void initTableWidget();
    void initTreeWidget();
    void initListWidget();
    void initStyle();
    void initTranslator();
    void initCancelPage();
    void provinceComboxChange(const QString& provinceName);
    void cityComboxChange(const QString& cityName);
    void countyComboxChange(const QString& countyName);
    void updateListWidget2();
    void listChanged2(int row);
    void treeClicked(QTreeWidgetItem* clickedItem, int index);
    void listChanged(int row);
    void tabChanged(int index);
    inline void listShow();
    inline void treeShow();
    inline void cancelPageShow();
    inline void closeAll() { this->close(); }
    void releaseButtonPush();
    void cancelButtonPush();
    void relieveButtonPush();

private:
    CovidInfoController* controller;
    QVector<Abnormal*>* select;
    QVector<Report*>* select2;
    QString RuiZi;
    Abnormal* now;
    Report* now2;
    QPushButton* closeButton = NULL;
    QVector<Area*>* areaInfo;
};

#endif // QUICREATOR_H