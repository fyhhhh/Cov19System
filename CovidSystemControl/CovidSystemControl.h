#ifndef QUICREATOR_H
#define QUICREATOR_H

#pragma execution_character_set("utf-8")

#include <QMainWindow>
#include "ui_CovidSystemControl.h"

class CovidSystemControl : public QMainWindow
{
    Q_OBJECT

public:
    CovidSystemControl(QWidget* parent = 0);
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
};

#endif // QUICREATOR_H