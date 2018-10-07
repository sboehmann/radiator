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
#ifndef CONFIG_USER_H
#define CONFIG_USER_H

#include <QtCore/QString>
#include <QtCore/QMetaType>

class QJsonObject;

class ConfigUser
{
    Q_GADGET
    Q_PROPERTY(QString username READ username WRITE setUsername)
    Q_PROPERTY(QString password READ password WRITE setPassword)

public:
    explicit ConfigUser();
    explicit ConfigUser(const QJsonValue &value);

    Q_INVOKABLE bool isEmpty() const;

    QJsonObject toJson() const;

    QString username() const;
    void setUsername(QString username);

    QString password() const;
    void setPassword(QString password);

private:
    QString m_username;
    QString m_password;
};
Q_DECLARE_METATYPE(ConfigUser)

inline bool operator==(const ConfigUser &lhs, const ConfigUser &rhs)
{
    return lhs.username() == rhs.username()
        && lhs.password() == rhs.password();
}

inline bool operator!=(const ConfigUser& lhs, const ConfigUser& rhs)
{
    return !(lhs == rhs);
}

#endif
