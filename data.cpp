#include "data.h"

Data::Data() : m_acc_t(), m_acc_x(), m_acc_y(), m_acc_z(),
    m_gyro_t(), m_gyro_x(), m_gyro_y(), m_gyro_z(),
    m_gps_t(), m_gps_lat(), m_gps_lng(), m_gps_alt(), m_gps_speed(), m_gps_prec(),
    m_optic_t(), m_optic_x(), m_optic_y(),
    m_orientation_t(), m_orientation_x(), m_orientation_y(), m_orientation_z()
{
}

void Data::addAcc(QDataStream &in)
{
    qint32 time, x, y, z;
    quint64 time_o;
    in >> time_o;
    time = (float) ((double)time_o / 1000000.f);
    in >> x;
    in >> y;
    in >> z;
    float *xx, *yy, *zz;
    xx = (float*) &x;
    yy = (float*) &y;
    zz = (float*) &z;
    m_acc_t.push_back(time);
    m_acc_x.push_back(*xx);
    m_acc_y.push_back(*yy);
    m_acc_z.push_back(*zz);
}

void Data::addOrientation(QDataStream &in)
{
    qint32 time, x, y, z;
    quint64 time_o;
    in >> time_o;
    time = (float) ((double)time_o / 1000000.f);
    in >> x;
    in >> y;
    in >> z;
    float *xx, *yy, *zz;
    xx = (float*) &x;
    yy = (float*) &y;
    zz = (float*) &z;
    m_orientation_t.push_back(time);
    m_orientation_x.push_back(*xx);
    m_orientation_y.push_back(*yy);
    m_orientation_z.push_back(*zz);
}

void Data::addGyro(QDataStream &in)
{ 
    quint32 x, y, z;
    float time;
    quint64 time_o;
    in >> time_o;
    time = (float) ((double)time_o / 1000000.f);
    in >> x;
    in >> y;
    in >> z;
    float *xx, *yy, *zz;
    xx = (float*) &x;
    yy = (float*) &y;
    zz = (float*) &z;
    m_gyro_t.push_back(time);
    m_gyro_x.push_back(*xx);
    m_gyro_y.push_back(*yy);
    m_gyro_z.push_back(*zz);
}

void Data::addGPS(QDataStream &in)
{
    float *lat, *lng, *alt, *speed, *prec;
    quint64 time_o;
    qint32 la, lo, al, sp, pr;
    in >> time_o;
    in >> la;
    in >> lo;
    in >> al;
    in >> sp;
    in >> pr;
    lat = (float*) &la;
    lng = (float*) &lo;
    alt = (float*) &al;
    speed = (float*) &sp;
    prec = (float*) &pr;
    m_gps_t.push_back((float) time_o / 1000000.f);
    m_gps_lat.push_back(*lat);
    m_gps_lng.push_back(*lng);
    m_gps_alt.push_back(*alt);
    m_gps_speed.push_back(*speed);
    m_gps_prec.push_back(*prec);
}

void Data::addOpticFlow(QDataStream &in)
{
    /*float t, x, y;
    quint64 t_original;
    in >> t_original;
    in >> x;
    in >> y;
    t = (float) (t_original / 1000000.f);
    m_optic_t.push_back(t);
    m_optic_x.push_back(x);
    m_optic_y.push_back(y);*/
}

QString Data::lastToString(Sensor s) {
    switch(s)
    {
    case ACCELEROMETER:
        return QString("acc ") + QString::number(m_acc_t.last()) + QString(" ") + QString::number(m_acc_x.last()) + QString(" ") + QString::number(m_acc_y.last()) + QString(" ") + QString::number(m_acc_z.last()) + QString("\n");
        break;
    case GYROSCOPE:
        return QString("gyro ") + QString::number(m_gyro_t.last()) + QString(" ") + QString::number(m_gyro_x.last()) + QString(" ") + QString::number(m_gyro_y.last()) + QString(" ") + QString::number(m_gyro_z.last()) + QString("\n");
        break;
    case GPS:
        return QString("gps ") + QString::number(m_gps_t.last()) + QString(" ") + QString::number(m_gps_lat.last()) + QString(" ") + QString::number(m_gps_lng.last()) + QString(" ") + QString::number(m_gps_alt.last()) + QString(" ") + QString::number(m_gps_speed.last()) + QString(" ") + QString::number(m_gps_prec.last()) + QString("\n");
        break;
    case OPTICFLOW:
        return QString("optic ") + QString::number(m_optic_t.last()) + QString(" ") + QString::number(m_optic_x.last()) + QString(" ") + QString::number(m_optic_y.last()) + QString("\n");
        break;
    case ORIENTATION:
        return QString("orientation ") + QString::number(m_orientation_t.last()) + QString(" ") + QString::number(m_orientation_x.last()) + QString(" ") + QString::number(m_orientation_y.last()) + QString(" ") + QString::number(m_orientation_z.last()) + QString("\n");
        break;
    case MAGNETICFIELD:
        return QString();
        break;
    }
}

double Data::getRate(Sensor s)
{
    QVector<double> *times;
    switch(s) {
    case ACCELEROMETER:
        times = &m_acc_t;
        break;
    case GYROSCOPE:
        times = &m_gyro_t;
        break;
    case GPS:
        times = &m_gps_t;
        break;
    case OPTICFLOW:
        times = &m_optic_t;
        break;
    case ORIENTATION:
        times = &m_orientation_t;
        break;
    case MAGNETICFIELD:
        return 0;
    }

    if(times->size() == 0)
        return 0.f;
    if(times->size() <= 10)
    {
        return ((times->last() - times->first())) / (times->size());
    }
    else
    {
        return ((times->last() - times->at(times->size() - 11))) / 10.f;
    }
}

double Data::getLastReceptionTime(Sensor s)
{
    switch(s) {
    case ACCELEROMETER:
        return m_acc_t.last();
    case GYROSCOPE:
        return m_gyro_t.last();
    case GPS:
        return m_gps_t.last();
    case OPTICFLOW:
        return m_optic_t.last();
    case ORIENTATION:
        return m_orientation_t.last();
    case MAGNETICFIELD:
        return 0;
    }
}

