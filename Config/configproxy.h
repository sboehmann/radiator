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
#ifndef CONFIG_PROXY_H
#define CONFIG_PROXY_H

#include "configuser.h"

#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtCore/QJsonObject>


class ConfigProxy
{
    Q_GADGET
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled)
    Q_PROPERTY(QString host READ host WRITE setHost)
    Q_PROPERTY(int port READ port WRITE setPort)
    Q_PROPERTY(QString username READ username WRITE setUsername)
    Q_PROPERTY(QString password READ password WRITE setPassword)

public:
    ConfigProxy();
    explicit ConfigProxy(const QJsonValue &value);

    QJsonObject toJson() const;

    Q_INVOKABLE bool isEmpty() const;

    bool enabled() const;
    void setEnabled(bool enabled);

    QString host() const;
    void setHost(QString host);

    int port() const;
    void setPort(int port);

    ConfigUser user() const;
    void setUser(const ConfigUser &user);

    QString username() const;
    void setUsername(QString username);

    QString password() const;
    void setPassword(QString password);

private:
    bool m_enabled = false;
    QString m_host;
    int m_port = -1;
    ConfigUser m_user;
};
Q_DECLARE_METATYPE(ConfigProxy)

inline bool operator==(const ConfigProxy &lhs, const ConfigProxy &rhs)
{
    return lhs.enabled() == rhs.enabled()
        && lhs.host() == rhs.host()
        && lhs.port() == rhs.port()
        && lhs.user() == rhs.user();
}

inline bool operator!=(const ConfigProxy& lhs, const ConfigProxy& rhs)
{
    return !(lhs == rhs);
}

#endif
