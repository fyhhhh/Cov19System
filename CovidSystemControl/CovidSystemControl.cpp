#include "CovidSystemControl.h"
#include "qfile.h"
#include "qtranslator.h" 
#include "qdesktopwidget.h"


CovidSystemControl::CovidSystemControl(CovidInfoController* covidInfoController, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::CovidSystemControl)
{
    this->controller = covidInfoController;
    ui->setupUi(this);
    this->initStyle();
    this->initTranslator();
    connect(controller, SIGNAL(updateAbnormalFinished()), this, SLOT(initTableWidget()));
    connect(controller, SIGNAL(initialAreaFinished()), this, SLOT(initTreeWidget()));
    ui->tabWidget->setCurrentIndex(0);
}

CovidSystemControl::~CovidSystemControl()
{
    delete ui;
}

void CovidSystemControl::initForm()
{
    /*this->initTableWidget();
    this->initTreeWidget();
    this->initListWidget();
    this->initOther();*/
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

void CovidSystemControl::initTableWidget()
{
    //设置列数和列宽
    int width = qApp->desktop()->availableGeometry().width() - 120;
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setColumnWidth(0, width * 0.06);
    ui->tableWidget->setColumnWidth(1, width * 0.10);
    ui->tableWidget->setColumnWidth(2, width * 0.06);
    ui->tableWidget->setColumnWidth(3, width * 0.10);
    ui->tableWidget->setColumnWidth(4, width * 0.15);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(25);

    //设置表头信息
    QStringList headText;
    headText << "省/市" << "市/区" << "地区" << "内容" << "时间";
    ui->tableWidget->setHorizontalHeaderLabels(headText);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setRowCount(300);

    QVector<Abnormal*>* abnormalInfo = controller->getAbnormalInfo();
    //视图显示
    for (int i = 0; i < abnormalInfo->size(); i++)
    {
        ui->tableWidget->setRowHeight(i, 24);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(abnormalInfo->at(i)->Province));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(abnormalInfo->at(i)->City));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(abnormalInfo->at(i)->County));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(abnormalInfo->at(i)->District));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(abnormalInfo->at(i)->Info));
    }
}

void CovidSystemControl::initTreeWidget()
{
    ui->treeWidget->clear();
    ui->treeWidget->setHeaderLabel("异常信息筛选");

    QVector<Area*>* areaInfo = controller->getAreaInfo();
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
}

void CovidSystemControl::treeClicked(QTreeWidgetItem* clickedItem, int index)
{
    if (clickedItem->parent() && clickedItem->parent()->parent())
    {
        qDebug() << clickedItem->parent()->parent()->text(index) << clickedItem->parent()->text(index) << clickedItem->text(index);
    }
}

void CovidSystemControl::initListWidget()
{
    QStringList items;
    for (int i = 1; i <= 30; i++) {
        items << QString("元素%1").arg(i);
    }

    ui->listWidget->addItems(items);
    ui->cbox1->addItems(items);
    ui->cbox2->addItems(items);
}

void CovidSystemControl::initOther()
{
    ui->rbtn1->setChecked(true);
    ui->ck2->setChecked(true);
    ui->ck3->setCheckState(Qt::PartiallyChecked);
    ui->horizontalSlider->setValue(88);

    ui->tab9->setStyleSheet("QPushButton{font:20pt;}");
    ui->widgetVideo->setStyleSheet("QLabel{font:20pt;}");
  
    ui->widgetVideo->setProperty("video", true);

    QList<QLabel*> labChs = ui->widgetVideo->findChildren<QLabel*>();
    foreach(QLabel * lab, labChs) {
        lab->setFocusPolicy(Qt::StrongFocus);
    }
}


