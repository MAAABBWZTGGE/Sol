#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>

#include <iostream>

#include "data.h"
#include "logger.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void update(quint8 dataAdded);
    void updateRaw(quint8 dataAdded);
    void update3D(quint8 dataAdded);
    void updateGraphs(quint8 dataAdded);
    void updateHistory(quint8 dataAdded);

protected:
    QTcpServer *server;
    QTcpSocket *client;
    Data m_data;
    Logger m_log;
    quint16 packetSize;

private slots:
    void incomingConnexion();
    void dataReceived();
    void clientDisconnection();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
