#pragma once
#include <QVector>
#include <QString>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QObject>
#include "Information.hpp"

const QString IP_PORT = "http://localhost:8080";


class CovidInfoController :public QObject
{
    Q_OBJECT

private:
    QVector<Abnormal*> abnormalInfo;
    QVector<Area*>* areaInfo;
public:
    CovidInfoController();
    QVector<Abnormal*>* getAbnormalInfo()
    {
        return &abnormalInfo;
    }
    QVector<Area*>* getAreaInfo()
    {
        return areaInfo;
    }
    void updateAbnormalInfo();
    void initialAreaInfo();

signals:
    void updateAbnormalFinished();
    void initialAreaFinished();

private slots:
    void updateAbnormalInfoFinished(QNetworkReply* reply);
    void initialAreaInfoFinished(QNetworkReply* reply);
};

template <class T>
void parseJson(QByteArray& json, QVector<T*>* res);

