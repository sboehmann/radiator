/*
 * Radiator
 * Copyright (C) 2018  Stefan Böhmann
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "utils.h"

QString Utils::time_elapsed_string(const QDateTime &from, const QDateTime &to)
{
    static qint64 min_in_secs = 60;
    static qint64 hour_in_secs = min_in_secs * 60;
    static qint64 day_in_secs = hour_in_secs * 24;

    if(!from.isValid() || !to.isValid()) {
        return QString();
    }

    // bug: use days since or so and not years diff (consider 31.12.17 to 01.01.18 -> years = 1)
    int years = to.date().year() - from.date().year();
    if(years == 0) {
        if(from.secsTo(to) < min_in_secs) {
            return QObject::tr("just now");
        }

        if(from.secsTo(to) < hour_in_secs) {
            return QObject::tr("%n minute(s) ago", "", static_cast<int>(from.secsTo(to) / min_in_secs));
        }

        if(from.secsTo(to) < day_in_secs) {
            return QObject::tr("%n hour(s) ago", "", static_cast<int>(from.secsTo(to) / hour_in_secs));
        }

        if(from.daysTo(to) < 14) {
            return QObject::tr("%n day(s) ago", "", static_cast<int>(from.daysTo(to)));
        }

        if(from.daysTo(to) < from.date().daysInMonth()) {
            return QObject::tr("%n week(s) ago", "", static_cast<int>(from.daysTo(to) / 7));
        }

        return QObject::tr("%n month(s) ago", "",  to.date().month() - from.date().month());
    }

    if(years > 3) {
        return QObject::tr("antediluvian");
    }

    if(years > 1) {
        return QObject::tr("prehistoric");
    }

    return QObject::tr("a long time ago");
}


QString Utils::human_time_diff(qint64 seconds)
{
    qint64 days = seconds / 60 / 60 / 24;
    seconds -= days * 60 * 60 * 24;

    qint64 hours = seconds / 60 / 60;
    seconds -= hours * 60 * 60;

    qint64 minutes = seconds / 60;
    seconds -= minutes * 60;

    QString str;
    if(days >= 1) {
        str += QString("%1d ").arg(days);
    }
    else if(!str.isEmpty() || hours >= 1) {
        str += QString(" %1h").arg(hours);
    }
    else if(!str.isEmpty() || minutes >= 1) {
        str += QString(" %1m").arg(minutes);
    }
    str += QString(" %1s").arg(seconds, str.isEmpty() ? 1 : 2, 10, QChar('0'));

    return str.trimmed();
}
