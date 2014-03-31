#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_data(),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));


    ui->accPlot->addGraph();
    ui->accPlot->graph(0)->setPen(QPen(Qt::red));
    ui->accPlot->addGraph();
    ui->accPlot->graph(1)->setPen(QPen(Qt::green));
    ui->accPlot->addGraph();
    ui->accPlot->graph(2)->setPen(QPen(Qt::blue));


    ui->gyroPlot->addGraph();
    ui->gyroPlot->graph(0)->setPen(QPen(Qt::red));
    ui->gyroPlot->addGraph();
    ui->gyroPlot->graph(1)->setPen(QPen(Qt::green));
    ui->gyroPlot->addGraph();
    ui->gyroPlot->graph(2)->setPen(QPen(Qt::blue));

    ui->GPSPlot->addGraph();
    ui->GPSPlot->graph(0)->setPen(QPen(Qt::red));
    ui->GPSPlot->addGraph();
    ui->GPSPlot->graph(1)->setPen(QPen(Qt::green));
    ui->GPSPlot->addGraph();
    ui->GPSPlot->graph(2)->setPen(QPen(Qt::blue));

    ui->GPSErrorPlot->addGraph();
    ui->GPSErrorPlot->graph(0)->setPen(QPen(Qt::red));
    ui->GPSErrorPlot->addGraph();
    ui->GPSErrorPlot->graph(1)->setPen(QPen(Qt::green));

    ui->orientationPlot->addGraph();
    ui->orientationPlot->graph(0)->setPen(QPen(Qt::red));
    ui->orientationPlot->addGraph();
    ui->orientationPlot->graph(1)->setPen(QPen(Qt::green));
    ui->orientationPlot->addGraph();
    ui->orientationPlot->graph(2)->setPen(QPen(Qt::blue));

    server = new QTcpServer(this);
    if (!server->listen(QHostAddress::Any, 6157))
    {
        ui->statusBar->showMessage(tr("Server failed to start. Reason : ") + server->errorString());
    }
    else
    {
        ui->statusBar->showMessage(tr("Server started on port ") + QString::number(server->serverPort()) + tr(". Awaiting connections..."));
        connect(server, SIGNAL(newConnection()), this, SLOT(incomingConnexion()));
    }
    client = NULL;
    packetSize = 0;
    last_update = QDateTime::currentDateTime().toTime_t();
}

MainWindow::~MainWindow()
{
    if(client != NULL) {
        client->disconnectFromHost();
        client->deleteLater();
        client = NULL;
    }
    server->close();
    server->deleteLater();
    delete ui;
}


void MainWindow::incomingConnexion()
{
    if(!client)
    {
        ui->statusBar->showMessage(tr("Client connected."));

        client = server->nextPendingConnection();

        connect(client, SIGNAL(readyRead()), this, SLOT(dataReceived()));
        connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnection()));
    }
    else
    {
        ui->statusBar->showMessage(tr("Additional connexion refused."));
    }
}

void MainWindow::dataReceived()
{
    QDataStream in(client);
    in.setByteOrder(QDataStream::BigEndian);
    if(packetSize == 0)
    {
        if(client->bytesAvailable() < (qint64) sizeof(quint16))
        {
            return;
        }
        in >> packetSize;
    }
    if(client->bytesAvailable() < packetSize)
    {
        return;
    }
    quint8 sensor;
    in >> sensor;
    switch(sensor) {
        case ACCELEROMETER:
            m_data.addAcc(in);
            ui->statusBar->showMessage("Acc received");
            m_log.log(m_data.lastToString(ACCELEROMETER));
            break;
        case GYROSCOPE:
            m_data.addGyro(in);
            ui->statusBar->showMessage("Gyro received");
            m_log.log(m_data.lastToString(GYROSCOPE));
            break;
        case GPS:
            m_data.addGPS(in);
            ui->statusBar->showMessage("GPS received");
            m_log.log(m_data.lastToString(GPS));
            break;
        case OPTICFLOW:
            m_data.addOpticFlow(in);
            ui->statusBar->showMessage("OF received");
            m_log.log(m_data.lastToString(OPTICFLOW));
            break;
        case ORIENTATION:
            m_data.addOrientation(in);
            ui->statusBar->showMessage("Orientation received");
            m_log.log(m_data.lastToString(ORIENTATION));
            break;
    }
    if( QDateTime::currentDateTime().toTime_t() - last_update > 1000) {
        update(sensor);
    }
    packetSize = 0;
    if(client->bytesAvailable() > sizeof(quint16)) {
        dataReceived();
    }
}

void MainWindow::clientDisconnection()
{
    ui->statusBar->showMessage("Client disconnected.");
    client->deleteLater();
    client = NULL;
}

void MainWindow::update(quint8 dataAdded)
{
    int i = ui->tabWidget->currentIndex();
    if(i == 0)
        updateRaw(dataAdded);
    else if(i == 1)
        update3D(dataAdded);
    else if(i == 2)
        updateGraphs(dataAdded);
    else if(i == 3)
        updateHistory(dataAdded);
}

void MainWindow::updateRaw(quint8 dataAdded)
{
    switch(dataAdded)
    {
        case ACCELEROMETER:
            ui->accLastDataLabel->setText(m_data.lastToString(ACCELEROMETER));
            ui->accLastTimeLabel->setText(QString::number(m_data.getLastReceptionTime(ACCELEROMETER)));
            ui->accRateLabel->setText(QString::number(m_data.getRate(ACCELEROMETER)));
            break;
        case GYROSCOPE:
            ui->gyroLastDataLabel->setText(m_data.lastToString(GYROSCOPE));
            ui->gyroLastTimeLabel->setText(QString::number(m_data.getLastReceptionTime(GYROSCOPE)));
            ui->gyroRateLabel->setText(QString::number(m_data.getRate(GYROSCOPE)));
            break;
        case GPS:
            ui->GPSPosLastDataLabel->setText(m_data.lastToString(GPS));
            ui->GPSPosLastTimeLabel->setText(QString::number(m_data.getLastReceptionTime(GPS)));
            ui->GPSPosRateLabel->setText(QString::number(m_data.getRate(GPS)));
            /*ui->GPSDOPLastDataLabel->setText(m_data.m_GPS.last().toStringPrec());
            ui->GPSDOPLastTimeLabel->setText(QString::number(m_data.m_GPS.getLastReceptionTime()));
            ui->GPSDOPRateLabel->setText(QString::number(m_data.m_GPS.getRate()));*/
            break;
        case OPTICFLOW:
            break;
        default:
            break;
    }
}

void MainWindow::update3D(quint8 dataAdded)
{
    switch(dataAdded)
    {
        case ACCELEROMETER:
            break;
        case GYROSCOPE:
            break;
        case GPS:
            break;
        case OPTICFLOW:
            break;
        default:
            break;
    }
}
void MainWindow::updateGraphs(quint8 dataAdded)
{
    //switch(dataAdded)
    //{
    //    case ACCELEROMETER:
            ui->accPlot->graph(0)->setData(m_data.m_acc_t, m_data.m_acc_x);
            ui->accPlot->graph(1)->setData(m_data.m_acc_t, m_data.m_acc_y);
            ui->accPlot->graph(2)->setData(m_data.m_acc_t, m_data.m_acc_z);
            ui->accPlot->rescaleAxes();
            ui->accPlot->replot();
            //break;
        //case GYROSCOPE:
            ui->gyroPlot->graph(0)->setData(m_data.m_gyro_t, m_data.m_gyro_x);
            ui->gyroPlot->graph(1)->setData(m_data.m_gyro_t, m_data.m_gyro_y);
            ui->gyroPlot->graph(2)->setData(m_data.m_gyro_t, m_data.m_gyro_z);
            ui->gyroPlot->rescaleAxes();
            ui->gyroPlot->replot();
            //break;
        //case GPS:
            ui->GPSPlot->graph(0)->setData(m_data.m_gps_t, m_data.m_gps_lat);
            ui->GPSPlot->graph(1)->setData(m_data.m_gps_t, m_data.m_gps_lng);
            ui->GPSPlot->graph(2)->setData(m_data.m_gps_t, m_data.m_gps_alt);
            ui->GPSErrorPlot->graph(0)->setData(m_data.m_gps_t, m_data.m_gps_speed);
            ui->GPSErrorPlot->graph(1)->setData(m_data.m_gps_t, m_data.m_gps_prec);
            ui->GPSPlot->rescaleAxes();
            ui->GPSErrorPlot->rescaleAxes();
            ui->GPSPlot->replot();
            ui->GPSErrorPlot->replot();
            //break;
        //case OPTICFLOW:
            ui->opticFlowPlot->graph(0)->setData(m_data.m_optic_t, m_data.m_optic_x);
            ui->opticFlowPlot->graph(1)->setData(m_data.m_optic_t, m_data.m_optic_y);
            ui->opticFlowPlot->rescaleAxes();
            ui->opticFlowPlot->replot();
            //break;
        //case ORIENTATION:
            ui->orientationPlot->graph(0)->setData(m_data.m_orientation_t, m_data.m_orientation_x);
            ui->orientationPlot->graph(1)->setData(m_data.m_orientation_t, m_data.m_orientation_y);
            ui->orientationPlot->graph(2)->setData(m_data.m_orientation_t, m_data.m_orientation_z);
            ui->orientationPlot->rescaleAxes();
            ui->orientationPlot->replot();
            //break;
        //default:
            //break;
    //}
}
void MainWindow::updateHistory(quint8 dataAdded)
{
    switch(dataAdded)
    {
        case ACCELEROMETER:
            break;
        case GYROSCOPE:
            break;
        case GPS:
            break;
        case OPTICFLOW:
            break;
        default:
            break;
    }
}
