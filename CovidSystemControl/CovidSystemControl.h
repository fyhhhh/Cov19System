#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CovidSystemControl.h"

class CovidSystemControl : public QMainWindow
{
    Q_OBJECT

public:
    CovidSystemControl(QWidget *parent = Q_NULLPTR);

private:
    Ui::CovidSystemControlClass ui;
};
