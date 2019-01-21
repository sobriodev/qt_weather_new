#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QTimer>
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QDateTime>

/*!
 * \class MainWindow
 * \inmodule core
 * \brief The MainWindow class joins the GUI and the application logic
 */

/*!
 * \variable MainWindow::DefaultRefresh
 * @brief The variable stores default USB data refresh time
 */

/*!
 * \variable MainWindow::SendAllCommand
 * \brief Default command for sending all sensor data
 */
const QString MainWindow::SendAllCommand = "SD";

/*!
 * \brief MainWindow constructor with optional \a parent widget
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connectHandlers();
    configureSerialPort();
    createUi();
    setFixedSize(QSize(650, 350));
    loadCss();
}

/*!
 * \brief MainWindow destructor
 */
MainWindow::~MainWindow()
{
    thread.terminate();
    thread.wait();
    serial.close();
    delete ui;
}

/*!
 * \brief Set sensor data readout frequency. It is used for choosing USB data refresh time. The \a timeout parameter stores new timeout in milliseconds
 */
void MainWindow::setTimerTimeout(int timeout)
{
    timer.stop();
    timer.start(timeout);
}

/*!
 * \internal
 * \brief Find correct NXP device and prepare data broadcast
 */
void MainWindow::configureSerialPort()
{
    ui->statusBar->showMessage(tr("Device not found"));
    QString portname;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        if (info.serialNumber() == "NXP-77") {
            portname=info.portName();
            serial.setPortName(portname);
            if (serial.open(QIODevice::ReadWrite)) {
                serial.setBaudRate(serial.Baud115200,  serial.AllDirections);
                ui->statusBar->showMessage(tr("Device: %1").arg(info.serialNumber()));
                serial.clear();
                thread.start(thread.HighestPriority);
                timer.start(DefaultRefresh);
            } else {
                ui->statusBar->showMessage(tr("Can't open %1, error code %2") .arg(serial.portName()).arg(serial.error()));
                return;
            }
            break;
        }
    }
}

/*!
 * \internal
 * \brief Connect all signals with handlers
 */
void MainWindow::connectHandlers()
{
    /* External thread tick */
    connect(&thread, &Thread::tick, this, &MainWindow::onThreadTick);

    /* Sensor data */
    connect(&parser, &Parser::temperatureChanged, this, &MainWindow::onTemperatureChanged);
    connect(&parser, &Parser::pressureChanged, this, &MainWindow::onPressureChanged);
    connect(&parser, &Parser::humidityChanged, this, &MainWindow::onHumidityChanged);
    connect(&parser, &Parser::newReadouts, this, &MainWindow::onNewReadouts);

    /* Timer */
    connect(&timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
}

void MainWindow::createUi()
{
    mainLayout = new QGridLayout;
    dateLayout = new QVBoxLayout;
    tempLayout = new QVBoxLayout;
    extLayout = new QVBoxLayout;

    QDate date = QDate::currentDate();
    QLocale locale;
    dateLabel = new QLabel(QString("%1 %2 %3, %4").arg(date.day()).arg(locale.monthName(date.month())).arg(date.year()).arg(locale.dayName(date.dayOfWeek())));
    tempLabel = new QLabel("--.--");
    cloudLabel = new QLabel("-----");
    pressureLabel = new QLabel("Ciśnienie: ---");
    humidityLabel = new QLabel("Wilgotność: ---");
    percTempLabel = new QLabel("Temp. odczuwalna: ---");
    bioCondLabel = new QLabel("Biomet: ---");
    lunarLabel = new QLabel("Księżyc: ---");

    QFont font = dateLabel->font();
    font.setPointSize(13);
    dateLabel->setFont(font);
    font.setPointSize(40);
    tempLabel->setFont(font);
    font.setPointSize(14);
    cloudLabel->setFont(font);
    font.setPointSize(12);
    pressureLabel->setFont(font);
    humidityLabel->setFont(font);
    percTempLabel->setFont(font);
    bioCondLabel->setFont(font);
    lunarLabel->setFont(font);

    dateLayout->addWidget(dateLabel, 0, Qt::AlignTop);
    tempLayout->addWidget(tempLabel, 0, Qt::AlignBottom);
    tempLayout->addWidget(cloudLabel, 0, Qt::AlignTop);

    extLayout->addWidget(pressureLabel, Qt::AlignCenter);
    extLayout->addWidget(humidityLabel, Qt::AlignCenter);
    extLayout->addWidget(percTempLabel, Qt::AlignCenter);
    extLayout->addWidget(bioCondLabel, Qt::AlignCenter);
    extLayout->addWidget(lunarLabel, Qt::AlignCenter);


    mainLayout->addLayout(dateLayout, 0, 0);
    mainLayout->addLayout(tempLayout, 1, 0);
    mainLayout->addLayout(extLayout, 0, 1);

    centralWidget()->setLayout(mainLayout);
}

void MainWindow::loadCss()
{

    QFile f(":qdarkstyle/style.qss");
    if (!f.exists()) {
        printf("Unable to set stylesheet, file not found\n");
    }
    else {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
}

/*!
 * \internal
 * \brief External thread tick handler. It is used for checking USB data presence
 */
void MainWindow::onThreadTick()
{
    if (serial.size())
        parser.parseSensorData(serial.readAll());
}

/*!
 * \internal
 * \brief Timer timeout handler. It sends USB commands
 */
void MainWindow::onTimerTimeout()
{
    dataWrite.clear();
    dataWrite.append(SendAllCommand);
    serial.write(dataWrite);

    QDate date = QDate::currentDate();
    QLocale locale;
    dateLabel->setText(QString("%1 %2 %3, %4").arg(date.day()).arg(locale.monthName(date.month())).arg(date.year()).arg(locale.dayName(date.dayOfWeek())));
}

/*!
 * \internal
 * \brief Temperature chaged slot. This is used for presenting and storing actual values. The \a temperature parameter holds new value
 */
void MainWindow::onTemperatureChanged(double temperature)
{
    tempLabel->setText(QString("%1°").arg(QString::number(temperature, 'f', 2)));
}

/*!
 * \internal
 * \brief Humidity chaged slot. This is used for presenting and storing actual values. The \a humidity parameter holds new value
 */
void MainWindow::onHumidityChanged(double humidity)
{
    humidityLabel->setText(QString("Wilgotność: %1%").arg(QString::number(humidity, 'f', 2)));
}

/*!
 * \internal
 * \brief Pressure chaged slot. This is used for presenting and storing actual values. The \a pressure parameter holds new value
 */
void MainWindow::onPressureChanged(double pressure)
{
    pressureLabel->setText(QString("Ciśnienie: %1hPa").arg(QString::number(pressure/100, 'f', 2)));
}

void MainWindow::onNewReadouts(SensorData sensorData)
{
    qDebug() << sensorData.toString();
    percTempLabel->setText(QString("Temp. odczuwalna: %1°").arg(QString::number(sensorData.calcPerseivedTemperature(), 'f', 2)));
    cloudLabel->setText(sensorData.calcCloudiness());
    bioCondLabel->setText(QString("Biomet: %1").arg(sensorData.calcBioConds()));
    lunarLabel->setText(QString("Księżyc: %1").arg(sensorData.calcLunarProperties()));
}
