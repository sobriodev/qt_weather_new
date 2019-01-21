#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <QObject>

class SensorData
{

public:
    explicit SensorData();
    SensorData(double temperature, double humidity, double pressure);

    double getTemperature() const;
    void setTemperature(double value);
    double getHumidity() const;
    void setHumidity(double value);
    double getPressure() const;
    void setPressure(double value);

    /* Extendend measurements */
    QString calcCloudiness() const;
    double calcPerseivedTemperature() const;
    QString calcBioConds() const;
    QString calcLunarProperties() const;

    QString toString() const;

private:
    double temperature;
    double humidity;
    double pressure;
};

#endif // SENSORDATA_H
