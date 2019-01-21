#include "sensordata.h"
#include "util.h"
#include <QDebug>

/*!
 * \class SensorData
 * \inmodule core
 * \brief The SensorData class represents single data measurment
 */

/*!
 * \brief The default SensorData constructor sets all sensor data to zero. Optional \a parent object can be sent
 */
SensorData::SensorData()
    : temperature(0), humidity(0), pressure(0) {}

/*!
 * \brief The Sensor data constructor initializes all values using \a temperature, \a humidity and \a pressure arguments. Also \a parent object can be sent
 */
SensorData::SensorData(double temperature, double humidity, double pressure)
    : temperature(temperature), humidity(humidity), pressure(pressure) {}


/*!
 * \brief Get current temperature
 */
double SensorData::getTemperature() const
{
    return temperature;
}

/*!
 * \brief Set current temperature using \a value parameter
 */
void SensorData::setTemperature(double value)
{
    temperature = value;
}

/*!
 * \brief Get current humidity
 */
double SensorData::getHumidity() const
{
    return humidity;
}

/*!
 * \brief Set current humidity using \a value parameter
 */
void SensorData::setHumidity(double value)
{
    humidity = value;
}

/*!
 * \brief Get current pressure
 */
double SensorData::getPressure() const
{
    return pressure;
}

/*!
 * Set current pressure using \a value parameter
 */
void SensorData::setPressure(double value)
{
    pressure = value;
}

/*!
 * \brief Calculate cloudiness and returns it
 * TODO implement method
 */
QString SensorData::calcCloudiness() const
{
    double oktan = 4 + (1013.25-(pressure/100)) / 8 - (75 - humidity) / 70;
    if (oktan < 0) {
        oktan = 0;
    }
    if (oktan > 8) {
        oktan = 8;
    }
    if (oktan<1.5) {
        return "Bezchmurnie";
    }
    if (oktan>=1.5 && oktan<3) {
        return "Zachmurzenie małe ";
    }
    if (oktan>=3 && oktan<6.5) {
        return "Zachmurzenie umiarkowane ";
    }
    if (oktan>=6.5 && oktan<7.2) {
        return "Zachmurzenie duże ";
    }
    if (oktan>=7.2) {
        return "Zachmurzenie całkowite ";
    }
    return "Brak danych";
}

/*!
 * \brief Calculate perseived temperature and returns it
 * TODO implement method
 */
double SensorData::calcPerseivedTemperature() const
{
    double sensedT = 0, idealPressure = 1013.25, idealHumidity = 75;
    double increaseTemperature = ((pressure/100) - idealPressure) / 8;
    if (Util::seasonDate()) {
        sensedT = temperature + increaseTemperature;
    } else {
        sensedT = temperature - increaseTemperature;
    }
    return sensedT + (idealHumidity - humidity) / 18;
}

/*!
 * \brief Calculate biometeorological conditions and returns it
 * TODO implement method
 */
QString SensorData::calcBioConds() const
{
    int b = 1;

    double pressure = this->pressure/100;
    if ((pressure > 1021 && pressure < 1029) || (pressure > 997 && pressure < 1005)){
        b = 0;
    }
    if ((humidity > 70 && humidity < 80) || (humidity > 50 && humidity < 60)){
        b = 0;
    }
    if (humidity >= 80 || humidity <= 50){
        b = -1;
    }
    if (pressure >= 1029 || pressure <= 997){
        b = -1;
    }
    if (temperature >= 28 || temperature <= 20){
        b = -1;
    }
    if (b == 1){
        return "korzystny";
    }
    else if (b == 0){
        return "obojętny";
    }
    else {
        return "niekorzystny";
    }
}

/*!
 * \brief Calculate lunar phase and increasing/decreasing rate and returns them
 * TODO implement method
 */
QString SensorData::calcLunarProperties() const
{
    double phase = Util::phaseMoon();
    double z = phase;
    if (z>50){
       z = 100 - z;
    }
    QString d = phase <=50 ? "rośnie":"maleje";
    return QString("%1% (%2)").arg(QString::number(z*2, 'f', 2)).arg(d);
}

/*!
 * \brief Return string containing all sensor data. This is for testing purposes
 */
QString SensorData::toString() const
{
    QString res("Temperature: %1 °C, Humidity: %2 %, Pressure %3 Pa");
    return res.arg(temperature).arg(humidity).arg(pressure);
}
