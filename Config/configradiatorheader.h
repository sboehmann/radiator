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
#ifndef CONFIG_RADIATOR_HEADER_H
#define CONFIG_RADIATOR_HEADER_H

#include <QtCore/QUrl>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtCore/QJsonValue>


class ConfigRadiatorHeader
{
    Q_GADGET
    Q_PROPERTY(bool visible READ visible WRITE setVisible)
    Q_PROPERTY(int height READ height WRITE setHeight)
    Q_PROPERTY(bool showDate READ showDate WRITE setShowDate)
    Q_PROPERTY(bool showTime READ showTime WRITE setShowTime)

public:
    ConfigRadiatorHeader();
    explicit ConfigRadiatorHeader(const QJsonValue &value);

    QJsonObject toJson() const;

    Q_INVOKABLE bool isEmpty() const;

    bool visible() const;
    void setVisible(bool visible);

    int height() const;
    void setHeight(int height);

    bool showDate() const;
    void setShowDate(bool showDate);

    bool showTime() const;
    void setShowTime(bool showTime);

    QString title() const;
    void setTitle(QString title);

private:
    bool m_visible = true;
    int m_height = 100;
    bool m_showDate = true;
    bool m_showTime = true;
    QString m_title;
};
Q_DECLARE_METATYPE(ConfigRadiatorHeader)


bool operator==(const ConfigRadiatorHeader &lhs, const ConfigRadiatorHeader &rhs);

inline bool operator!=(const ConfigRadiatorHeader& lhs, const ConfigRadiatorHeader& rhs)
{
    return !(lhs == rhs);
}


#endif
