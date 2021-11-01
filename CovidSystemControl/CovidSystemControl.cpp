#include "CovidSystemControl.h"


CovidSystemControl::CovidSystemControl(CovidInfoController* covidInfoController, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::CovidSystemControl)
{
    this->controller = covidInfoController;
    ui->setupUi(this);
    this->initStyle();
    this->initTranslator();

    ui->txtMain->hide();
    ui->pushButton_release->hide();
    ui->pushButton_cancel->hide();
    ui->label->setFont(QFont(RuiZi, 12));
    ui->label->setText("常态化疫情分析平台设备管理系统");
    dynamic_cast<QGridLayout*>(ui->centralwidget->layout())->setColumnStretch(1, 1);
    

    connect(controller, SIGNAL(updateAbnormalFinished()), this, SLOT(initTableWidget()));
    connect(controller, SIGNAL(initialAreaFinished()), this, SLOT(initTreeWidget()));
    ui->tabWidget->setCurrentIndex(0);
    this->initListWidget();
    QObject::connect(ui->listWidget, &QListWidget::currentRowChanged, this, &CovidSystemControl::listChanged);
    QObject::connect(ui->tabWidget, &QTabWidget::currentChanged, this, &CovidSystemControl::tabChanged);

    ui->txtMain->setFont(QFont(RuiZi, 18));
    ui->txtMain->setAlignment(Qt::AlignHCenter);
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

void CovidSystemControl::initTableWidget()
{
    //设置列数和列宽
    int width = qApp->desktop()->availableGeometry().width() - 120;
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setColumnWidth(0, width * 0.15);
    ui->tableWidget->setColumnWidth(1, width * 0.15);
    ui->tableWidget->setColumnWidth(2, width * 0.15);
    ui->tableWidget->setColumnWidth(3, width * 0.3);
    ui->tableWidget->setColumnWidth(4, width * 0.3);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(25);

    //设置表头信息
    QStringList headText;
    headText << "省" << "市" << "区县" << "地区" << "内容";
    ui->tableWidget->setHorizontalHeaderLabels(headText);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setRowCount(300);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(50);

    
    for (int i = 0; i < 5; i++)
        ui->tableWidget->horizontalHeaderItem(i)->setFont(QFont(RuiZi, 12, 75));


    QVector<Abnormal*>* abnormalInfo = controller->getAbnormalInfo();
    ui->tableWidget->clearContents();
    //视图显示
    for (int i = 0; i < abnormalInfo->size(); i++)
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(abnormalInfo->at(i)->Province));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(abnormalInfo->at(i)->City));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(abnormalInfo->at(i)->County));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(abnormalInfo->at(i)->District));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(abnormalInfo->at(i)->Info));
        for (int j = 0; j < 5; j++)
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
        select = controller->findAbnormalInfo(clickedItem->text(index));
        ui->listWidget->clear();
        if (select != NULL)
        {
            for (int i = 0; i < select->size(); i++)
            {
                ui->listWidget->addItem("\t\t" + select->at(i)->Province + "\t" + select->at(i)->City + "\t" + select->at(i)->County + "\t" + select->at(i)->District + "\t\t" + select->at(i)->Info);
            }
        }
    }
}

void CovidSystemControl::initListWidget()
{
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

void CovidSystemControl::listChanged(int row)
{
    qDebug() << row;
    if (row >= 0)
    {
        ui->txtMain->clear();
        now = select->at(row);
        ui->txtMain->setText(now->Province + "\t" + now->City + "\t" + now->County + "\t" + now->District + "\t\t" + now->Info);
    }
    else
    {
        ui->txtMain->clear();
    }
}

void CovidSystemControl::tabChanged(int index)
{
    switch (index)
    {
    case 0:
        ui->txtMain->hide();
        ui->pushButton_release->hide();
        ui->pushButton_cancel->hide();
        break;
    case 1:
        ui->txtMain->show();
        ui->pushButton_release->show();
        ui->pushButton_cancel->show();
        break;
    default:
        break;
    }
}

void CovidSystemControl::titleShow()
{
}
