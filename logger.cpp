#include "logger.h"

Logger::Logger()
{
    QString filename = "/Users/aloys/Documents/Log";
    quint32 time = QDateTime::currentDateTime().toTime_t();
    filename += QString::number(time);
    out = new QFile(filename);
    if(out != NULL) {
        if(!out->open(QIODevice::WriteOnly)) {
            out->deleteLater();
            out = NULL;
            std::cout << "Can't open log file" << std::endl;
            return;
        }
        datastream = new QDataStream(out);
    }
}

Logger::~Logger()
{
    out->flush();
    out->close();
}

void Logger::log(QString s)
{
    //std::cout << s.toStdString() << std::endl;
    if(out != NULL && datastream != NULL) {
        *datastream << s;
    }
}
