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
#ifndef CONFIG_REMOTE_H
#define CONFIG_REMOTE_H

#include "configuser.h"

#include <QtCore/QUrl>
#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtCore/QJsonObject>

class ConfigRemote
{
    Q_GADGET
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled)
    Q_PROPERTY(Type type READ type WRITE setType)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QUrl url READ url WRITE setUrl)
    Q_PROPERTY(QString username READ username WRITE setUsername)
    Q_PROPERTY(QString password READ password WRITE setPassword)

public:
    enum Type {
        Jenkins
    };
    Q_ENUM(Type)

    ConfigRemote();
    explicit ConfigRemote(const QJsonValue &value);

    Q_INVOKABLE bool isEmpty() const;

    QJsonObject toJson() const;

    bool enabled() const;
    void setEnabled(bool enabled);

    Type type() const;
    void setType(Type type);

    QString name() const;
    void setName(QString name);

    QUrl url() const;
    void setUrl(QUrl url);

    ConfigUser user() const;
    void setUser(const ConfigUser &user);

    QString username() const;
    void setUsername(QString username);

    QString password() const;
    void setPassword(QString password);

private:
    bool m_enabled = true;
    Type m_type = Jenkins;
    QString m_name;
    QUrl m_url;
    ConfigUser m_user;
};
Q_DECLARE_METATYPE(ConfigRemote)

bool operator==(const ConfigRemote &lhs, const ConfigRemote &rhs);

inline bool operator!=(const ConfigRemote& lhs, const ConfigRemote& rhs)
{
    return !(lhs == rhs);
}

#endif
