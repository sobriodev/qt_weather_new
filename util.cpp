#include "util.h"
#include <QDateTime>
#include "math.h"
#include <QDebug>

Util::Util() {}

bool Util::seasonDate()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    return dateTime.date().month() > 3 && dateTime.date().month() < 10;
}

double Util::phaseMoon()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    int month = dateTime.date().month();
    int year = dateTime.date().year();
    int day = dateTime.date().day();
    double ms = 29.5305902778;
    double x = (month + 9) / 12;
    double a = 4716 + year + floor(x);
    double y = 275 * month / 9;
    double v = 7 * a / 4;
    double b = 1729279.5 + 367 * year + floor(y) - floor(v) + day;
    double q = (a + 83) / 100;
    double c = floor(q);
    double w = 3 * (c + 1) / 4;
    double e = floor(w);
    double jd = b + 38 - e;
    jd = jd / ms - 0.3033;
    jd = 100 * (jd - floor(jd));
    return jd;
}
