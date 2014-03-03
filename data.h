#ifndef DATA_H
#define DATA_H

#include <QDataStream>
#include <QTabWidget>

#include <iostream>

enum Sensor {ACCELEROMETER = 0, GYROSCOPE = 1, GPS = 2, OPTICFLOW = 3, ORIENTATION = 4, MAGNETICFIELD = 5};


class Data
{
public:
    Data();
    void addAcc(QDataStream &in);
    void addGyro(QDataStream &in);
    void addGPS(QDataStream &in);
    void addOpticFlow(QDataStream &in);
    void addOrientation(QDataStream &in);
    double getRate(Sensor s);
    double getLastReceptionTime(Sensor s);
    QString lastToString(Sensor s);

    QVector<double> m_acc_t;
    QVector<double> m_acc_x;
    QVector<double> m_acc_y;
    QVector<double> m_acc_z;
    QVector<double> m_gyro_t;
    QVector<double> m_gyro_x;
    QVector<double> m_gyro_y;
    QVector<double> m_gyro_z;
    QVector<double> m_orientation_t;
    QVector<double> m_orientation_x;
    QVector<double> m_orientation_y;
    QVector<double> m_orientation_z;
    QVector<double> m_gps_t;
    QVector<double> m_gps_lat;
    QVector<double> m_gps_lng;
    QVector<double> m_gps_alt;
    QVector<double> m_gps_hdop;
    QVector<double> m_gps_vdop;
    QVector<double> m_optic_t;
    QVector<double> m_optic_x;
    QVector<double> m_optic_y;


};

#endif // DATA_H
