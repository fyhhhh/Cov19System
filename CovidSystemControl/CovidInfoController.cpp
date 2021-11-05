#include "CovidInfoController.h"
CovidInfoController::CovidInfoController()
{
    FILE* fp = NULL;
    char tmp[50];
    fopen_s(&fp, "./IP.txt", "r");
    if (fp != NULL)
    {
        fgets(tmp, 50, fp);
        fclose(fp);
    }
    else
    {
        qDebug() << "IP.txt读取失败";
    }
    IP_PORT = QString::fromStdString(tmp);
    qDebug() << "URL:" << IP_PORT;

    areaInfo = new QVector<Area*>;
    updateAbnormalInfo();
    initialAreaInfo();
}


//发送GET /abnormal请求更新Controller
void CovidInfoController::updateAbnormalInfo()
{
    QNetworkRequest request;
    QNetworkAccessManager* naManager = new QNetworkAccessManager(this);
    QMetaObject::Connection connRet = QObject::connect(naManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updateAbnormalInfoFinished(QNetworkReply*)));
    Q_ASSERT(connRet);
    request.setUrl(QUrl(IP_PORT + "/abnormal"));
    QNetworkReply* reply = naManager->get(request);
    qDebug() << "GET " << request.url();
}

void CovidInfoController::updateReportInfo(const QString province, const QString city, const QString county)
{
    if (province != "选择省" && city != "选择市" && county != "选择县")
    {
        QNetworkRequest request;
        QNetworkAccessManager* naManager = new QNetworkAccessManager(this);
        QMetaObject::Connection connRet = QObject::connect(naManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updateReportInfoFinished(QNetworkReply*)));
        Q_ASSERT(connRet);
        request.setUrl(QUrl(IP_PORT + "/reports?province=" + province + "&city=" + city + "&county=" + county));
        QNetworkReply* reply = naManager->get(request);
        qDebug() << "GET " << request.url();
    }
}

//GET /abnormal请求完成
void CovidInfoController::updateAbnormalInfoFinished(QNetworkReply* reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (statusCode.isValid())
        qDebug() << "status code=" << statusCode.toInt();

    QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    if (reason.isValid())
        qDebug() << "reason=" << reason.toString();

    QNetworkReply::NetworkError err = reply->error();
    QString returnJson;
    if (err != QNetworkReply::NoError) {
        qDebug() << "Failed: " << reply->errorString();
    }
    else {
        returnJson = QString::fromStdString(reply->readAll().toStdString());
        parseJson(QByteArray::fromStdString(returnJson.toStdString()), &abnormalInfo);
        emit updateAbnormalFinished();
    }
}

void CovidInfoController::updateReportInfoFinished(QNetworkReply* reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (statusCode.isValid())
        qDebug() << "status code=" << statusCode.toInt();

    QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    if (reason.isValid())
        qDebug() << "reason=" << reason.toString();

    QNetworkReply::NetworkError err = reply->error();
    QString returnJson;
    if (err != QNetworkReply::NoError) {
        qDebug() << "Failed: " << reply->errorString();
    }
    else {
        returnJson = QString::fromStdString(reply->readAll().toStdString());
        parseJson(QByteArray::fromStdString(returnJson.toStdString()), &reportInfo);
        emit updateReportFinished();
    }
}

//发送GET /area请求更新Controller
void CovidInfoController::initialAreaInfo()
{
    QNetworkRequest request;
    QNetworkAccessManager* naManager = new QNetworkAccessManager(this);
    QMetaObject::Connection connRet = QObject::connect(naManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(initialAreaInfoFinished(QNetworkReply*)));
    Q_ASSERT(connRet);
    request.setUrl(QUrl(IP_PORT + "/area"));
    QNetworkReply* reply = naManager->get(request);
}

QVector<Abnormal*>* CovidInfoController::findAbnormalInfo(QString& province, QString& city, QString& county)
{
    QVector<Abnormal*>* res = new QVector<Abnormal*>;
    res->clear();
    for (int i = 0; i < abnormalInfo.size(); i++)
    {
        if (abnormalInfo.at(i)->Province == province && abnormalInfo.at(i)->City == city && abnormalInfo.at(i)->County == county)
        {
            res->push_back(abnormalInfo[i]);
        }
    }
    return res->empty() ? NULL : res;
}

void CovidInfoController::releaseReport(Abnormal* now)
{
    QJsonObject postJson
    {
        {"Province", now->Province},
        {"City", now->City},
        {"County" ,now->County},
        {"District" ,now->District},
        {"Info" ,now->Info},
        {"Time" ,now->Time},
        {"Device" , now->Device.toInt()}
    };
    QNetworkRequest request;
    QNetworkAccessManager* naManager = new QNetworkAccessManager(this);
    QMetaObject::Connection connRet = QObject::connect(naManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(postFinished(QNetworkReply*)));
    Q_ASSERT(connRet);

    request.setUrl(QUrl(IP_PORT + "/reports"));

    QJsonDocument document = QJsonDocument(postJson);
    QByteArray postData = document.toJson(QJsonDocument::Compact);
    QNetworkReply* reply = naManager->post(request, postData);
    qDebug() << "POST " << request.url();
}

void CovidInfoController::cancelAbnormal(Abnormal* now)
{
    QJsonObject postJson
    {
        {"Province", now->Province},
        {"City", now->City},
        {"County" ,now->County},
        {"District" ,now->District},
        {"Info" ,now->Info},
        {"Time" ,now->Time},
        {"Device" , now->Device.toInt()}
    };
    QNetworkRequest request;
    QNetworkAccessManager* naManager = new QNetworkAccessManager(this);
    QMetaObject::Connection connRet = QObject::connect(naManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(postFinished(QNetworkReply*)));
    Q_ASSERT(connRet);

    request.setUrl(QUrl(IP_PORT + "/abnormal_delete"));

    QJsonDocument document = QJsonDocument(postJson);
    QByteArray postData = document.toJson(QJsonDocument::Compact);
    QNetworkReply* reply = naManager->post(request, postData);
    qDebug() << "POST " << request.url();
}

void CovidInfoController::relieveReport(Report* now2)
{
    QJsonObject postJson
    {
        {"Province", now2->Province},
        {"City", now2->City},
        {"County" ,now2->County},
        {"District" ,now2->District},
        {"Info" ,now2->Info},
        {"Time" ,now2->Time},
        {"Device" , now2->Device.toInt()}
    };
    QNetworkRequest request;
    QNetworkAccessManager* naManager = new QNetworkAccessManager(this);
    QMetaObject::Connection connRet = QObject::connect(naManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(postFinished(QNetworkReply*)));
    Q_ASSERT(connRet);

    request.setUrl(QUrl(IP_PORT + "/reports_delete"));

    QJsonDocument document = QJsonDocument(postJson);
    QByteArray postData = document.toJson(QJsonDocument::Compact);
    QNetworkReply* reply = naManager->post(request, postData);
    qDebug() << "POST " << request.url();
}

void CovidInfoController::initialAreaInfoFinished(QNetworkReply* reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (statusCode.isValid())
        qDebug() << "status code=" << statusCode.toInt();

    QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    if (reason.isValid())
        qDebug() << "reason=" << reason.toString();

    QNetworkReply::NetworkError err = reply->error();
    QString returnJson;
    if (err != QNetworkReply::NoError) {
        qDebug() << "Failed: " << reply->errorString();
    }
    else {
        returnJson = QString::fromStdString(reply->readAll().toStdString());
    }
    parseJson(QByteArray::fromStdString(returnJson.toStdString()), areaInfo);
    emit initialAreaFinished();
}

void CovidInfoController::postFinished(QNetworkReply* reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (statusCode.isValid())
        qDebug() << "status code=" << statusCode.toInt();

    QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    if (reason.isValid())
        qDebug() << "reason=" << reason.toString();

    QNetworkReply::NetworkError err = reply->error();
    QString returnJson;
    if (err != QNetworkReply::NoError) {
        qDebug() << "Failed: " << reply->errorString();
    }
    else {
        qDebug() << reply->readAll();
    }

    this->updateAbnormalInfo();
}

template <class T>
void parseJson(QByteArray& json, QVector<T*>* res)
{
    res->clear();
    T* newInfo;
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(json, &jsonError);
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if (doucment.isArray())
        {
            QJsonArray array = doucment.array();
            int nSize = array.size();
            for (int i = 0; i < nSize; i++)
            {
                QJsonValue value = array.at(i);
                if (value.isObject())
                {
                    newInfo = new T;
                    QJsonObject object = value.toObject();

                    for (int i = 0; i < T::getNames().size(); i++)
                    {
                        if (object.contains(T::getNames().at(i)))
                        {
                            QJsonValue subValue = object.value(T::getNames().at(i));
                            if (subValue.isString())
                            {
                                QString strName = subValue.toString();
                                newInfo->set(T::getNames().at(i), strName);
                            }
                            else if (subValue.isDouble())
                            {
                                QString strName = QString::number(subValue.toInt());
                                newInfo->set(T::getNames().at(i), strName);
                            }
                        }
                    }
                    if (newInfo->isVaild())
                    {
                        res->push_back(newInfo);
                    }
                }
            }
        }
    }
}

