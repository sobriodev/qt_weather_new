#ifndef PARSER_H
#define PARSER_H

#include "sensordata.h"

#include <QObject>

class Parser : public QObject
{
    Q_OBJECT
public:
    static const int DataCount = 3;
    explicit Parser(QObject *parent = nullptr);
    void parseSensorData(const QString &data);

signals:
    void temperatureChanged(double temperature);
    void humidityChanged(double humidity);
    void pressureChanged(double pressure);
    void newReadouts(SensorData sensorData);
};

#endif // PARSER_H
