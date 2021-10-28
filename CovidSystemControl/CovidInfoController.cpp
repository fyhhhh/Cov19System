#include "CovidInfoController.h"
CovidInfoController::CovidInfoController()
{
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
    }

    parseJson(QByteArray::fromStdString(returnJson.toStdString()), &abnormalInfo);
    emit updateAbnormalFinished();
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

