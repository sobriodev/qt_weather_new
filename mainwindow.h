#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>
#include "thread.h"
#include "parser.h"
#include "sensordata.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static const QString SendAllCommand;
    static const int DefaultRefresh = 1000;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setTimerTimeout(int timeout);

private:
    void configureSerialPort();
    void connectHandlers();
    void createUi();
    void loadCss();

private slots:
    void onThreadTick();
    void onTimerTimeout();

    /* Data readout handlers */
    void onTemperatureChanged(double temperature);
    void onHumidityChanged(double humidity);
    void onPressureChanged(double pressure);
    void onNewReadouts(SensorData sensorData);

private:
    Ui::MainWindow *ui;
    Thread thread;
    QSerialPort serial;
    QByteArray dataRead;
    QByteArray dataWrite;
    SensorData sensorData;
    Parser parser;
    QTimer timer;

    /* ui */
    QGridLayout *mainLayout;
    QVBoxLayout *dateLayout;
    QVBoxLayout *tempLayout;
    QVBoxLayout *extLayout;

    QLabel *dateLabel;
    QLabel *tempLabel;
    QLabel *cloudLabel;
    QLabel *pressureLabel;
    QLabel *humidityLabel;
    QLabel *percTempLabel;
    QLabel *bioCondLabel;
    QLabel *lunarLabel;

};

#endif // MAINWINDOW_H
