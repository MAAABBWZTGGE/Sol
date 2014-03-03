#ifndef SENSORS_STRUCTS_H
#define SENSORS_STRUCTS_H

#include <QtGlobal>
#include <QString>

class BasicData {
public:
    QString toString()
    {
        return QString::number(x) + " " + QString::number(y) + " " + QString::number(z);
    }
    quint32 time;
    float x;
    float y;
    float z;
};

typedef BasicData AccData;
typedef BasicData GyroData;

class GPSData {
public:
    QString toStringPos()
    {
        return QString::number(lat) + "N " + QString::number(lng) + "W@" + QString::number(alt) + "m";
    }
    QString toStringPrec()
    {
        return QString::number(HDOP) + "H " + QString::number(VDOP) + "V";
    }
    quint32 time;
    double lat;
    double lng;
    float alt;
    float HDOP;
    float VDOP;
};

class OpticFlowData {
public:
    QString toString()
    {
        return QString::number(x) + " " + QString::number(y);
    }

    quint32 time;
    float x;
    float y;
};

#endif // SENSORS_STRUCTS_H
