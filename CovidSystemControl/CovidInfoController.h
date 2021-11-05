#pragma once
#pragma execution_character_set("utf-8")
#include <QVector>
#include <QString>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QObject>
#include <cstdio>
#include "Information.hpp"

class CovidInfoController :public QObject
{
    Q_OBJECT

private:
    QVector<Abnormal*> abnormalInfo;
    QVector<Area*>* areaInfo;
    QVector<Report*> reportInfo;
    QString IP_PORT;

public:
    CovidInfoController();

    QVector<Abnormal*>* getAbnormalInfo() { return &abnormalInfo; }
    QVector<Area*>* getAreaInfo() { return areaInfo; }
    QVector<Report*> getReportInfo() { return this->reportInfo; }

    void updateAbnormalInfo();
    void updateReportInfo(const QString province, const QString city, const QString county);
    void initialAreaInfo();

    QVector<Abnormal*>* findAbnormalInfo(QString& province, QString& city, QString& county);

    void releaseReport(Abnormal* now);
    void cancelAbnormal(Abnormal* now);
    void relieveReport(Report* now2);

signals:
    void updateAbnormalFinished();
    void updateReportFinished();
    void initialAreaFinished();

private slots:
    void updateAbnormalInfoFinished(QNetworkReply* reply);
    void updateReportInfoFinished(QNetworkReply* reply);
    void initialAreaInfoFinished(QNetworkReply* reply);
    void postFinished(QNetworkReply* reply);
};

template <class T>
void parseJson(QByteArray& json, QVector<T*>* res);

