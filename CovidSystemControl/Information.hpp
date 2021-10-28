/*
    本文件声明了信息接口类
    所有信息类必须继承自Information
*/
#pragma once
#include <QString>
#include <QDebug>
#include <QVector>


class Information
{
public:
    virtual bool isVaild() = 0;
    virtual void show() = 0;
    virtual void set(QString key, QString value) = 0;
};

class Abnormal :public Information
{
public:
    QString Province;
    QString City;
    QString County;
    QString District;
    QString Info;
    const static QVector<QString> names;
    
    virtual bool isVaild()
    {
        if (!Province.isEmpty() && !City.isEmpty() && !County.isEmpty() && !District.isEmpty() && !Info.isEmpty())
        {
            return true;
        }
        return false;
    }
    virtual void set(QString key, QString value)
    {
        if (key == "Province") Province = value;
        if (key == "City") City = value;
        if (key == "County") County = value;
        if (key == "District") District = value;
        if (key == "Info") Info = value;
    }
    virtual void show()
    {
        qDebug() << Province << City << County << District << Info;
    }

    static QVector<QString> getNames()
    {
        return names;
    }
};

class Area :public Information
{
public:
    QString Province;
    QString City;
    QString County;
    const static QVector<QString> names;

    virtual bool isVaild()
    {
        if (!Province.isEmpty() && !City.isEmpty() && !County.isEmpty())
        {
            return true;
        }
        return false;
    }
    virtual void set(QString key, QString value)
    {
        if (key == "Province") Province = value;
        if (key == "City") City = value;
        if (key == "County") County = value;
    }
    virtual void show()
    {
        qDebug() << Province << City << County;
    }

    static QVector<QString> getNames()
    {
        return names;
    }
};

