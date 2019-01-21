#include "parser.h"
#include <QChar>

/*!
 * \class Parser
 * \inmodule core
 * \brief The Parser class provides a way to parse crude sensor data
 */

/*!
 * \brief The parser class constructor with optional \a parent object
 */
Parser::Parser(QObject *parent) : QObject(parent) {}

/*!
 * \fn void Parser::temperatureChanged(double temperature)
 * \brief The signal is emmited when temperature has changed. The \a temperature parameter holds the actual value
 */

/*!
 * \fn void Parser::humidityChanged(double humidity)
 * \brief The signal is emmited when humidity has changed. The \a humidity parameter holds the actual value
 */

/*!
 * \fn void Parser::pressureChanged(double pressure)
 * \brief The signal is emmited when pressure has changed. The \a pressure parameter holds the actual value
 */

/*!
 * \brief Parse crude sensor data. The \a data format is \b {T{value}|H{value}|P{value}}, where \b {{value}} is a placeholder for real double value.
 * This function emits
 * \list
 *     \li \l Parser::temperatureChanged()
 *     \li \l Parser::humidityChanged()
 *     \li \l Parser::pressureChanged()
 * \endlist
 */
void Parser::parseSensorData(const QString &data)
{
    QStringList values = data.split("|");
    SensorData sensorData;

    for (QString &value : values) {
        char type = value.at(0).toLatin1();
        double readout = value.remove(0, 1).toDouble();

        switch (type) {
        case 'T':
            emit temperatureChanged(readout);
            sensorData.setTemperature(readout);
            break;
        case 'P':
            emit pressureChanged(readout);
            sensorData.setPressure(readout);
            break;
        case 'H':
            emit humidityChanged(readout);
            sensorData.setHumidity(readout);
            break;
        default:
            break;
        }
    }

    if (values.count() == DataCount) {
        emit newReadouts(sensorData);
    }
}
