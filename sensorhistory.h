#ifndef SENSORHISTORY_H
#define SENSORHISTORY_H

#import <QVector>

template <typename SensorDataStruct>
class SensorHistory : public QVector<SensorDataStruct>
{
public:
    SensorHistory();
    float getRate();
    quint32 getLastReceptionTime();
};

/*********************
 *  Implementation   *
 *********************/

template <typename SensorDataStruct>
SensorHistory<SensorDataStruct>::SensorHistory()
{
}

template <typename SensorDataStruct>
float SensorHistory<SensorDataStruct>::getRate()
{
    if(this->size() == 0)
        return 0;
    if(this->size() < 10)
    {
        return ((float) (this->last().time - this->first().time)) / ((float) this->size());
    }
    else
    {
        return ((float) (this->last().time - this->at(this->size() - 11).time)) / 10.f;
    }
}

template <typename SensorDataStruct>
quint32 SensorHistory<SensorDataStruct>::getLastReceptionTime()
{
    if(this->size() > 0) {
        return this->last().time;
    }
    return 0;
}
#endif // SENSORHISTORY_H
