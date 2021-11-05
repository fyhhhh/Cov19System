#include "CovidSystemControl.h"


CovidSystemControl::CovidSystemControl(CovidInfoController* covidInfoController, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::CovidSystemControl)
{
    this->controller = covidInfoController;
    ui->setupUi(this);
    this->initStyle();
    this->initTranslator();

    ui->label->setFont(QFont(RuiZi, 15));
    ui->label->setText("常态化疫情分析平台  设备管理系统");
    dynamic_cast<QGridLayout*>(ui->centralwidget->layout())->setColumnStretch(1, 1);

    ui->tabWidget->setTabText(0, "数据总览");
    ui->tabWidget->setTabText(1, "发布/撤销");
    ui->tabWidget->setTabText(2, "警报解除");
    ui->tabWidget->setFont(QFont(RuiZi, 18));


    connect(controller, SIGNAL(updateAbnormalFinished()), this, SLOT(initTableWidget()));
    connect(controller, SIGNAL(initialAreaFinished()), this, SLOT(initTreeWidget()));
    ui->tabWidget->setCurrentIndex(0);
    this->initListWidget();
    QObject::connect(ui->listWidget, &QListWidget::currentRowChanged, this, &CovidSystemControl::listChanged);
    QObject::connect(ui->tabWidget, &QTabWidget::currentChanged, this, &CovidSystemControl::tabChanged);

    ui->txtMain->setFont(QFont(RuiZi, 22));
    ui->txtMain->setAlignment(Qt::AlignHCenter);

    QPalette pal;
    pal.setBrush(QPalette::Background, QBrush(QPixmap("./other/image/txtMain.jpg")));
    this->setPalette(pal);

    listShow();

    closeButton = new QPushButton(this);
    closeButton->setGeometry(qApp->desktop()->availableGeometry().width() * 0.93, qApp->desktop()->availableGeometry().height() * 0.006, qApp->desktop()->availableGeometry().width() * 0.06, qApp->desktop()->availableGeometry().height() * 0.03);
    closeButton->setText("关闭");
    closeButton->setFont(QFont(RuiZi, 12));
    closeButton->show();

    ui->pushButton_cancel->setFont(QFont(RuiZi, 18));
    ui->pushButton_release->setFont(QFont(RuiZi, 18));
    ui->pushButton_relieve->setFont(QFont(RuiZi, 18));

    QObject::connect(closeButton, &QPushButton::released, this, &CovidSystemControl::closeAll);
    QObject::connect(ui->pushButton_release, &QPushButton::released, this, &CovidSystemControl::releaseButtonPush);
    QObject::connect(ui->pushButton_cancel, &QPushButton::released, this, &CovidSystemControl::cancelButtonPush);
    QObject::connect(ui->pushButton_relieve, &QPushButton::released, this, &CovidSystemControl::relieveButtonPush);


    connect(this, SIGNAL(initTreeFinished()), this, SLOT(initCancelPage()));

    QObject::connect(ui->comboBox_Province, &QComboBox::currentTextChanged, this, &CovidSystemControl::provinceComboxChange);
    QObject::connect(ui->comboBox_City, &QComboBox::currentTextChanged, this, &CovidSystemControl::cityComboxChange);
    QObject::connect(ui->comboBox_County, &QComboBox::currentTextChanged, this, &CovidSystemControl::countyComboxChange);
    QObject::connect(controller, &CovidInfoController::updateReportFinished, this, &CovidSystemControl::updateListWidget2);
    QObject::connect(ui->listWidget_2, &QListWidget::currentRowChanged, this, &CovidSystemControl::listChanged2);
}

CovidSystemControl::~CovidSystemControl()
{
    delete ui;
}

void CovidSystemControl::initStyle()
{
    //加载样式表
    QFile file("./other/qss/psblack.css");
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }
    RuiZi = QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont("./other/ttf/RuiZi.ttf")).at(0);

}

void CovidSystemControl::initTranslator()
{
    //加载鼠标右键菜单翻译文件
    QTranslator* translator1 = new QTranslator(qApp);
    translator1->load("./other/image/qt_zh_CN.qm");
    qApp->installTranslator(translator1);

    //加载富文本框鼠标右键菜单翻译文件
    QTranslator* translator2 = new QTranslator(qApp);
    translator2->load("./other/image/widgets.qm");
    qApp->installTranslator(translator2);
}

void CovidSystemControl::initCancelPage()
{
    ui->comboBox_Province->addItem("选择省");
    ui->comboBox_City->addItem("选择市");
    ui->comboBox_County->addItem("选择县");
    ui->comboBox_Province->setFont(QFont(RuiZi, 20));
    ui->comboBox_City->setFont(QFont(RuiZi, 20));
    ui->comboBox_County->setFont(QFont(RuiZi, 20));
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
    {
        ui->comboBox_Province->addItem(ui->treeWidget->topLevelItem(i)->text(0));
    }
}

void CovidSystemControl::provinceComboxChange(const QString& provinceName)
{
    if (provinceName != "选择省")
    {
        ui->comboBox_City->clear();
        ui->comboBox_City->addItem("选择市");
        for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
        {
            if (ui->treeWidget->topLevelItem(i)->text(0) == provinceName)
            {
                QTreeWidgetItem* ChildItem = ui->treeWidget->topLevelItem(i);
                for (int j = 0; j < ChildItem->childCount(); j++)
                {
                    ui->comboBox_City->addItem(ChildItem->child(j)->text(0));
                }
                break;
            }
        }
    }
    else
    {
        ui->comboBox_City->clear();
        ui->comboBox_City->addItem("选择市");
    }
    ui->comboBox_County->clear();
    ui->comboBox_County->addItem("选择县");
    ui->listWidget_2->clear();
    listShow();
}

void CovidSystemControl::cityComboxChange(const QString& cityName)
{
    if (cityName != "选择市")
    {
        ui->comboBox_County->clear();
        ui->comboBox_County->addItem("选择县");
        for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
        {
            if (ui->treeWidget->topLevelItem(i)->text(0) == ui->comboBox_Province->currentText())
            {
                QTreeWidgetItem* ChildItem = ui->treeWidget->topLevelItem(i);
                for (int j = 0; j < ChildItem->childCount(); j++)
                {
                    if (ChildItem->child(j)->text(0) == cityName)
                    {
                        QTreeWidgetItem* GrandChildItem = ChildItem->child(j);
                        for (int k = 0; k < GrandChildItem->childCount(); k++)
                        {
                            ui->comboBox_County->addItem(GrandChildItem->child(k)->text(0));
                        }
                    }
                }
            }
        }
    }
    else
    {
        ui->comboBox_County->clear();
        ui->comboBox_County->addItem("选择县");
    }
    ui->listWidget_2->clear();
    listShow();
}

void CovidSystemControl::countyComboxChange(const QString& countyName)
{
    if (countyName != "选择县" && countyName != "")
        controller->updateReportInfo(ui->comboBox_Province->currentText(), ui->comboBox_City->currentText(), ui->comboBox_County->currentText());

    ui->listWidget_2->clear();
    listShow();
}

void CovidSystemControl::updateListWidget2()
{
    select2 = &(controller->getReportInfo());
    ui->listWidget_2->clear();
    if (select2 != NULL)
    {
        for (int i = 0; i < select2->size(); i++)
        {
            ui->listWidget_2->addItem(select2->at(i)->Time + "\t\t" + select2->at(i)->Province + select2->at(i)->City + select2->at(i)->County + select2->at(i)->District + select2->at(i)->Info);
            ui->listWidget_2->item(i)->setSizeHint(QSize(ui->listWidget_2->width(), qApp->desktop()->availableGeometry().height() * 0.05));
            ui->listWidget_2->item(i)->setFont(QFont(RuiZi, 18));
        }
    }
}

void CovidSystemControl::listChanged2(int row)
{
    select2 = &(controller->getReportInfo());
    if (row >= 0)
    {
        now2 = select2->at(row);
        QString messageTitle = "已发布消息：检测设备" + now2->Device + "报告：\n" + now2->Time;
        QString message = "\n" + now2->Province + "\t" + now2->City + "\t" + now2->County + "\t" + now2->District + "\t\t" + now2->Info;
        QString messageTile = "\n请选择是否解除";
        ui->txtMain->setText(messageTitle + message + messageTile);
        cancelPageShow();
    }
    else
    {
        ui->txtMain->setText("");
    }
}

void CovidSystemControl::initTableWidget()
{
    //设置列数和列宽
    int width = qApp->desktop()->availableGeometry().width() - 120;
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setColumnWidth(0, width * 0.10);
    ui->tableWidget->setColumnWidth(1, width * 0.10);
    ui->tableWidget->setColumnWidth(2, width * 0.10);
    ui->tableWidget->setColumnWidth(3, width * 0.2);
    ui->tableWidget->setColumnWidth(4, width * 0.3);
    ui->tableWidget->setColumnWidth(5, width * 0.2);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(25);

    //设置表头信息
    QStringList headText;
    headText << "省" << "市" << "区县" << "地区" << "内容" << "时间";
    ui->tableWidget->setHorizontalHeaderLabels(headText);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setRowCount(300);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(50);

    
    for (int i = 0; i < 6; i++)
        ui->tableWidget->horizontalHeaderItem(i)->setFont(QFont(RuiZi, 12, 75));


    QVector<Abnormal*>* abnormalInfo = controller->getAbnormalInfo();
    ui->tableWidget->clearContents();
    qDebug() << abnormalInfo->size();
    //视图显示
    for (int i = 0; i < abnormalInfo->size(); i++)
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(abnormalInfo->at(i)->Province));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(abnormalInfo->at(i)->City));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(abnormalInfo->at(i)->County));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(abnormalInfo->at(i)->District));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(abnormalInfo->at(i)->Info));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(abnormalInfo->at(i)->Time));
        for (int j = 0; j < 6; j++)
        {
            ui->tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->tableWidget->item(i, j)->setFont(QFont(RuiZi, 12));
        }
    }
}

void CovidSystemControl::initTreeWidget()
{
    ui->treeWidget->clear();
    ui->treeWidget->setHeaderLabel("异常信息筛选");

    areaInfo = controller->getAreaInfo();
    QTreeWidgetItem* provinceItem = NULL;
    QTreeWidgetItem* cityItem = NULL;
    QTreeWidgetItem* countyItem = NULL;
    for (int i = 0; i < areaInfo->size(); i++)
    {
        provinceItem = NULL;
        cityItem = NULL;
        countyItem = NULL;
        for (int j = 0; j < ui->treeWidget->topLevelItemCount(); j++)
        {
            if (ui->treeWidget->topLevelItem(j)->text(0)==areaInfo->at(i)->Province)
            {
                provinceItem = ui->treeWidget->topLevelItem(j);
            }
        }
        if (provinceItem == NULL)
        {
            QTreeWidgetItem* newProvinceItem = new QTreeWidgetItem(ui->treeWidget);
            newProvinceItem->setText(0, areaInfo->at(i)->Province);
            provinceItem = newProvinceItem;
        }
        for (int j = 0; j < provinceItem->childCount(); j++)
        {
            if (provinceItem->child(j)->text(0) == areaInfo->at(i)->City)
            {
                cityItem = provinceItem->child(j);
            }
        }
        if (cityItem == NULL)
        {
            QTreeWidgetItem* newCityItem = new QTreeWidgetItem(provinceItem);
            newCityItem->setText(0, areaInfo->at(i)->City);
            cityItem = newCityItem;
        }
        for (int j = 0; j < cityItem->childCount(); j++)
        {
            if (cityItem->child(j)->text(0) == areaInfo->at(i)->County)
            {
                countyItem = cityItem->child(j);
            }
        }
        if (countyItem == NULL)
        {
            QTreeWidgetItem* newCountyItem = new QTreeWidgetItem(cityItem);
            newCountyItem->setText(0, areaInfo->at(i)->County);
            countyItem = newCountyItem;
        }
    }
    ui->treeWidget->collapseAll();
    connect(ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(treeClicked(QTreeWidgetItem*, int)));
    emit initTreeFinished();
}

void CovidSystemControl::treeClicked(QTreeWidgetItem* clickedItem, int index)
{
    if (clickedItem->parent() && clickedItem->parent()->parent())
    {
        select = controller->findAbnormalInfo(clickedItem->parent()->parent()->text(index), clickedItem->parent()->text(index), clickedItem->text(index));
        ui->listWidget->clear();
        if (select != NULL)
        {
            for (int i = 0; i < select->size(); i++)
            {
                ui->listWidget->addItem(select->at(i)->Time + "\t\t" + select->at(i)->Province + select->at(i)->City + select->at(i)->County + select->at(i)->District + select->at(i)->Info);
                ui->listWidget->item(i)->setSizeHint(QSize(ui->listWidget->width(), qApp->desktop()->availableGeometry().height() * 0.05));
                ui->listWidget->item(i)->setFont(QFont(RuiZi, 18));
            }
            if (select->size() > 0)
            {
                listChanged(0);
            }
        }
    }
}

void CovidSystemControl::initListWidget()
{
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);
}

void CovidSystemControl::listChanged(int row)
{
    if (row >= 0)
    {
        ui->txtMain->clear();
        now = select->at(row);
        QString messageTitle = "检测设备" + now->Device + "报告：\n" + now->Time;
        QString message = "\n" + now->Province + "\t" + now->City + "\t" + now->County + "\t" + now->District + "\t\t" + now->Info;
        QString messageTile = "\n请选择是否发布";
        ui->txtMain->setText(messageTitle + message + messageTile);
        treeShow();
    }
    else
    {
        ui->txtMain->setText("");
    }
}

void CovidSystemControl::tabChanged(int index)
{
    switch (index)
    {
    case 0:
        listShow();
        break;
    case 1:
        listShow();
        ui->treeWidget->collapseAll();
        ui->listWidget->clear();
        break;
    case 2:
        listShow();
        break;
    default:
        break;
    }
}

void CovidSystemControl::releaseButtonPush()
{
    controller->releaseReport(now);
    ui->treeWidget->collapseAll();
    ui->listWidget->clear();
    ui->txtMain->setText("发布成功！");
}

void CovidSystemControl::cancelButtonPush()
{
    controller->cancelAbnormal(now);
    ui->treeWidget->collapseAll();
    ui->listWidget->clear();
    ui->txtMain->setText("撤销成功！");
}

void CovidSystemControl::relieveButtonPush()
{
    controller->relieveReport(now2);
    ui->listWidget_2->clear();
    ui->txtMain->setText("解除成功！");
    ui->comboBox_Province->setCurrentIndex(0);
    cancelPageShow();
}


