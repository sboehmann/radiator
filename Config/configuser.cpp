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
#include "configuser.h"

#include <QJsonObject>


ConfigUser::ConfigUser()
{
}

ConfigUser::ConfigUser(const QJsonValue &value)
{
    QJsonObject obj = value.toObject();
    m_username = obj.value("username").toString();
    m_password = obj.value("password").toString();
}

bool ConfigUser::isEmpty() const
{
    return m_username.isEmpty() && m_password.isEmpty();
}

QString ConfigUser::username() const
{
    return m_username;
}

void ConfigUser::setUsername(QString username)
{
    m_username = username;
}

QString ConfigUser::password() const
{
    return m_password;
}

void ConfigUser::setPassword(QString password)
{
    m_password = password;
}

QJsonObject ConfigUser::toJson() const
{
    QJsonObject obj;
    if(!m_username.isEmpty()) {
        obj.insert("username", m_username);
    }

    if(!m_password.isEmpty()) {
        obj.insert("password", m_password);
    }

    return obj;
}
