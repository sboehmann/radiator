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
#ifndef CONFIG_RADIATOR_H
#define CONFIG_RADIATOR_H

#include "configradiatorheader.h"

#include <QtCore/QUrl>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtCore/QJsonValue>

class ConfigRadiator
{
    Q_GADGET
    Q_PROPERTY(QUrl backgroundImage READ backgroundImage WRITE setBackgroundImage)
    Q_PROPERTY(int jobPollingInterval READ jobPollingInterval WRITE setJobPollingInterval)
    Q_PROPERTY(int buildPollingInterval READ buildPollingInterval WRITE setBuildPollingInterval)
    Q_PROPERTY(bool fullScreenMode READ fullScreenMode WRITE setFullScreenMode)
    Q_PROPERTY(int marquee READ marquee WRITE setMarquee)
    Q_PROPERTY(Qt::TextElideMode elide READ elide WRITE setElide)
    Q_PROPERTY(int historicalBuildCount READ historicalBuildCount WRITE setHistoricalBuildCount)
    Q_PROPERTY(bool trayIconEnabled READ trayIconEnabled WRITE setTrayIconEnabled)
    Q_PROPERTY(bool blameEnabled READ blameEnabled WRITE setBlameEnabled)
    Q_PROPERTY(bool headerVisible READ headerVisible WRITE setHeaderVisible)
    Q_PROPERTY(int headerHeight READ headerHeight WRITE setHeaderHeight)
    Q_PROPERTY(QString headerTitle READ headerTitle WRITE setHeaderTitle)
    Q_PROPERTY(bool headerShowDate READ headerShowDate WRITE setHeaderShowDate)
    Q_PROPERTY(bool headerShowTime READ headerShowTime WRITE setHeaderShowTime)

public:
    ConfigRadiator();
    explicit ConfigRadiator(const QJsonValue &value);

    QJsonObject toJson() const;

    Q_INVOKABLE bool isEmpty() const;

    QUrl backgroundImage() const;
    void setBackgroundImage(QUrl backgroundImage);

    int jobPollingInterval() const;
    void setJobPollingInterval(int seconds);

    int buildPollingInterval() const;
    void setBuildPollingInterval(int seconds);

    bool fullScreenMode() const;
    void setFullScreenMode(bool fullScreenMode);

    int marquee() const;
    void setMarquee(int marquee);

    Qt::TextElideMode elide() const;

    void setElide(Qt::TextElideMode elide);

    int historicalBuildCount() const;
    void setHistoricalBuildCount(int historicalBuildCount);

    bool trayIconEnabled() const;
    void setTrayIconEnabled(bool trayIconEnabled);

    bool blameEnabled() const;
    void setBlameEnabled(bool blameEnabled);

    ConfigRadiatorHeader& header();
    ConfigRadiatorHeader header() const;
    void setHeader(ConfigRadiatorHeader header);

    bool headerVisible() const;
    void setHeaderVisible(bool headerVisible);

    QString headerTitle() const;
    void setHeaderTitle(QString headerTitle);

    int headerHeight() const;
    void setHeaderHeight(int headerHeight);

    bool headerShowDate() const;
    void setHeaderShowDate(bool headerShowDate);

    bool headerShowTime() const;
    void setHeaderShowTime(bool headerShowTime);

private:
    QUrl m_backgroundImage;
    int m_jobPollingInterval = 300;
    int m_buildPollingInterval = 60;
    int m_historicalBuildCount = 5;
    int m_marguee = 0;
    Qt::TextElideMode m_elide = Qt::ElideNone;
    bool m_fullScreenMode = false;
    bool m_trayIconEnabled = false;
    bool m_blameEnabled = true;
    ConfigRadiatorHeader m_header;
};
Q_DECLARE_METATYPE(ConfigRadiator)


bool operator==(const ConfigRadiator &lhs, const ConfigRadiator &rhs);

inline bool operator!=(const ConfigRadiator& lhs, const ConfigRadiator& rhs)
{
    return !(lhs == rhs);
}

#endif
