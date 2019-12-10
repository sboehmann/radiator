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
#include "configradiatorheader.h"

#include <QJsonObject>

static const ConfigRadiatorHeader empty;


ConfigRadiatorHeader::ConfigRadiatorHeader()
{
}

ConfigRadiatorHeader::ConfigRadiatorHeader(const QJsonValue &value)
{
    QJsonObject obj = value.toObject();

    if(obj.contains(QLatin1String("visible"))) {
        m_visible = obj.value(QLatin1String("visible")).toBool();
    }

    if(obj.contains(QLatin1String("height"))) {
        m_height = obj.value(QLatin1String("height")).toInt();
    }

    if(obj.contains(QLatin1String("date"))) {
        m_showDate = obj.value(QLatin1String("date")).toBool();
    }

    if(obj.contains(QLatin1String("time"))) {
        m_showTime = obj.value(QLatin1String("time")).toBool();
    }

    if(obj.contains(QLatin1String("title"))) {
        m_title = obj.value(QLatin1String("title")).toString();
    }
}

QJsonObject ConfigRadiatorHeader::toJson() const
{
    QJsonObject obj;

    if(m_visible != empty.visible()) {
        obj.insert(QStringLiteral("visible"), m_visible);
    }

    if(m_height != empty.height()) {
        obj.insert(QStringLiteral("height"), m_height);
    }

    if(m_showDate != empty.showDate()) {
        obj.insert(QStringLiteral("date"), m_showDate);
    }

    if(m_showTime != empty.showTime()) {
        obj.insert(QStringLiteral("time"), m_showTime);
    }

    if(m_title != empty.title()) {
        obj.insert(QStringLiteral("title"), m_title);
    }

    return obj;
}

bool ConfigRadiatorHeader::isEmpty() const
{
    return *this == empty;
}

bool ConfigRadiatorHeader::visible() const
{
    return m_visible;
}

void ConfigRadiatorHeader::setVisible(bool visible)
{
    m_visible = visible;
}

int ConfigRadiatorHeader::height() const
{
    return m_height;
}

void ConfigRadiatorHeader::setHeight(int height)
{
    m_height = height;
}

bool ConfigRadiatorHeader::showDate() const
{
    return m_showDate;
}

void ConfigRadiatorHeader::setShowDate(bool showDate)
{
    m_showDate = showDate;
}

bool ConfigRadiatorHeader::showTime() const
{
    return m_showTime;
}

void ConfigRadiatorHeader::setShowTime(bool showTime)
{
    m_showTime = showTime;
}

QString ConfigRadiatorHeader::title() const
{
    return m_title;
}

void ConfigRadiatorHeader::setTitle(QString title)
{
    m_title = title;
}

bool operator==(const ConfigRadiatorHeader &lhs, const ConfigRadiatorHeader &rhs)
{
    return lhs.visible() == rhs.visible()
        && lhs.title() == rhs.title()
        && lhs.height() == rhs.height()
        && lhs.showDate() == rhs.showDate()
        && lhs.showTime() == rhs.showTime();
}
