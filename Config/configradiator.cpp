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
#include "configradiator.h"

#include <QMetaEnum>
#include <QJsonObject>

const ConfigRadiator empty;


ConfigRadiator::ConfigRadiator()
{
}

ConfigRadiator::ConfigRadiator(const QJsonValue &value)
{
    QJsonObject obj = value.toObject();
    m_backgroundImage = QUrl::fromUserInput(obj.value(QLatin1Literal("background")).toString());
    m_fullScreenMode = obj.value(QLatin1Literal("fullScreen")).toBool();

    if(obj.contains(QLatin1Literal("polling"))) {
        QJsonValue p = obj.value(QLatin1Literal("polling"));
        if(p.isDouble()) {
            setJobPollingInterval(p.toInt(empty.jobPollingInterval()));
            setBuildPollingInterval(p.toInt(empty.buildPollingInterval()));
        }
        else {
            QJsonObject po = p.toObject();
            setJobPollingInterval(po.value(QLatin1Literal("job")).toInt(empty.jobPollingInterval()));
            setBuildPollingInterval(po.value(QLatin1Literal("build")).toInt(empty.buildPollingInterval()));
        }
    }

    setMarquee(obj.value(QLatin1Literal("marguee")).toInt(empty.marquee()));

    QMetaEnum me = QMetaEnum::fromType<Qt::TextElideMode>();
    bool ok;
    int tem = me.keyToValue(obj.value(QLatin1Literal("elide")).toString().toLatin1().data(), &ok);
    setElide(ok ? static_cast<Qt::TextElideMode>(tem) : empty.elide());

    setHistoricalBuildCount(obj.value(QLatin1Literal("history")).toInt(empty.historicalBuildCount()));
    setTrayIconEnabled(obj.value(QLatin1Literal("trayIconEnabled")).toBool(empty.trayIconEnabled()));
    setBlameEnabled(obj.value(QLatin1Literal("blameEnabled")).toBool(empty.blameEnabled()));

    m_header = ConfigRadiatorHeader(obj.value(QLatin1Literal("header")));
}

QJsonObject ConfigRadiator::toJson() const
{
    QJsonObject obj;
    if(!m_backgroundImage.isEmpty()) {
        obj.insert(QStringLiteral("background"), m_backgroundImage.toString());
    }

    if(m_fullScreenMode) {
        obj.insert(QStringLiteral("fullScreen"), true);
    }

    if(m_jobPollingInterval == m_buildPollingInterval) {
        obj.insert(QStringLiteral("polling"), m_jobPollingInterval);
    }
    else {
        QJsonObject polling;
        if(m_jobPollingInterval != empty.jobPollingInterval()) {
            polling.insert(QStringLiteral("job"), m_jobPollingInterval);
        }

        if(m_buildPollingInterval != empty.buildPollingInterval()) {
            polling.insert(QStringLiteral("build"), m_buildPollingInterval);
        }

        if(!polling.isEmpty()) {
            obj.insert(QStringLiteral("polling"), polling);
        }
    }

    if(m_marguee != empty.marquee()) {
        obj.insert(QStringLiteral("marguee"), m_marguee);
    }

    if(m_elide != empty.elide()) {
        QMetaEnum me = QMetaEnum::fromType<Qt::TextElideMode>();
        obj.insert(QStringLiteral("elide"), me.valueToKey(m_elide));
    }

    if(m_historicalBuildCount != empty.historicalBuildCount()) {
        obj.insert(QStringLiteral("history"), m_historicalBuildCount);
    }

    if(m_trayIconEnabled != empty.trayIconEnabled()) {
        obj.insert(QStringLiteral("trayIconEnabled"), m_trayIconEnabled);
    }

    if(m_blameEnabled != empty.blameEnabled()) {
        obj.insert(QStringLiteral("blameEnabled"), m_blameEnabled);
    }

    if(m_header != empty.header()) {
        obj.insert(QStringLiteral("header"), m_header.toJson());
    }

    return obj;
}

bool ConfigRadiator::isEmpty() const
{
    return *this == empty;
}

QUrl ConfigRadiator::backgroundImage() const
{
    return m_backgroundImage;
}

void ConfigRadiator::setBackgroundImage(QUrl backgroundImage)
{
    m_backgroundImage = backgroundImage;
}

int ConfigRadiator::jobPollingInterval() const
{
    return m_jobPollingInterval;
}

void ConfigRadiator::setJobPollingInterval(int seconds)
{
    m_jobPollingInterval = qMax(0, seconds);
}

int ConfigRadiator::buildPollingInterval() const
{
    return m_buildPollingInterval;
}

void ConfigRadiator::setBuildPollingInterval(int seconds)
{
    m_buildPollingInterval = qMax(0, seconds);
}

bool ConfigRadiator::fullScreenMode() const
{
    return m_fullScreenMode;
}

void ConfigRadiator::setFullScreenMode(bool fullScreenMode)
{
    m_fullScreenMode = fullScreenMode;
}

int ConfigRadiator::marquee() const
{
    return m_marguee;
}

void ConfigRadiator::setMarquee(int marguee)
{
    m_marguee = marguee;
}

Qt::TextElideMode ConfigRadiator::elide() const
{
    return m_elide;
}

void ConfigRadiator::setElide(Qt::TextElideMode elide)
{
    m_elide = elide;
}

int ConfigRadiator::historicalBuildCount() const
{
    return m_historicalBuildCount;
}

void ConfigRadiator::setHistoricalBuildCount(int historicalBuildCount)
{
    m_historicalBuildCount = historicalBuildCount;
}

bool ConfigRadiator::trayIconEnabled() const
{
    return m_trayIconEnabled;
}

void ConfigRadiator::setTrayIconEnabled(bool trayIconEnabled)
{
    m_trayIconEnabled = trayIconEnabled;
}

bool ConfigRadiator::blameEnabled() const
{
    return m_blameEnabled;
}

void ConfigRadiator::setBlameEnabled(bool blameEnabled)
{
    m_blameEnabled = blameEnabled;
}

ConfigRadiatorHeader &ConfigRadiator::header()
{
    return m_header;
}

ConfigRadiatorHeader ConfigRadiator::header() const
{
    return m_header;
}

void ConfigRadiator::setHeader(ConfigRadiatorHeader header)
{
    m_header = header;
}

bool ConfigRadiator::headerVisible() const
{
    return m_header.visible();
}

void ConfigRadiator::setHeaderVisible(bool headerVisible)
{
    m_header.setVisible(headerVisible);
}

int ConfigRadiator::headerHeight() const
{
    return m_header.height();
}

void ConfigRadiator::setHeaderHeight(int headerHeight)
{
    m_header.setHeight(headerHeight);
}

bool ConfigRadiator::headerShowDate() const
{
    return m_header.showDate();
}

void ConfigRadiator::setHeaderShowDate(bool headerShowDate)
{
    m_header.setShowDate(headerShowDate);
}

bool ConfigRadiator::headerShowTime() const
{
    return m_header.showTime();
}

void ConfigRadiator::setHeaderShowTime(bool headerShowTime)
{
    m_header.setShowTime(headerShowTime);
}

QString ConfigRadiator::headerTitle() const
{
    return m_header.title();
}

void ConfigRadiator::setHeaderTitle(QString headerTitle)
{
    m_header.setTitle(headerTitle);
}

bool operator==(const ConfigRadiator &lhs, const ConfigRadiator &rhs)
{
    return lhs.jobPollingInterval() == rhs.jobPollingInterval()
        && lhs.buildPollingInterval() == rhs.buildPollingInterval()
        && lhs.backgroundImage() == rhs.backgroundImage()
        && lhs.fullScreenMode() == rhs.fullScreenMode()
        && lhs.marquee() == rhs.marquee()
        && lhs.elide() == rhs.elide()
        && lhs.historicalBuildCount() == rhs.historicalBuildCount()
        && lhs.trayIconEnabled() == rhs.trayIconEnabled()
        && lhs.blameEnabled() == rhs.blameEnabled()
        && lhs.header() == rhs.header();
}
