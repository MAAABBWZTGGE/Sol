#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QDateTime>
#include <QDataStream>
#include <iostream>

class Logger
{
public:
    Logger();
    ~Logger();
    void log(QString s);

protected:
    QFile *out;
    QDataStream *datastream;
};

#endif // LOGGER_H
