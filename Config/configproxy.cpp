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
#include "configproxy.h"

ConfigProxy::ConfigProxy()
{
}

ConfigProxy::ConfigProxy(const QJsonValue &value)
{
    QJsonObject obj = value.toObject();

    m_enabled = obj.contains("enabled") && obj.value("enabled").toBool();
    m_host = obj.value("host").toString();
    m_port = obj.value("port").toInt(-1);
    m_user = ConfigUser(obj.value("user"));
}

QJsonObject ConfigProxy::toJson() const
{
    QJsonObject obj;

    if(!m_host.isEmpty()) {
        obj.insert("host", m_host);
    }
    if(m_port >= 0) {
        obj.insert("port", m_port);
    }

    if(!m_user.isEmpty()) {
        obj.insert("user", m_user.toJson());
    }

    if(!obj.isEmpty()) {
        obj.insert("enabled", m_enabled);
    }

    return obj;
}

bool ConfigProxy::isEmpty() const
{
    return m_port < 0 && m_host.trimmed().isEmpty() && m_user.isEmpty();
}

bool ConfigProxy::enabled() const
{
    return m_enabled;
}

void ConfigProxy::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

QString ConfigProxy::host() const
{
    return m_host;
}

void ConfigProxy::setHost(QString host)
{
    m_host = host;
}

int ConfigProxy::port() const
{
    return m_port;
}

void ConfigProxy::setPort(int port)
{
    m_port = port;
}

ConfigUser ConfigProxy::user() const
{
    return m_user;
}

void ConfigProxy::setUser(const ConfigUser &user)
{
    m_user = user;
}

QString ConfigProxy::username() const
{
    return m_user.username();
}

void ConfigProxy::setUsername(QString username)
{
    m_user.setUsername(username);
}

QString ConfigProxy::password() const
{
    return m_user.password();
}

void ConfigProxy::setPassword(QString password)
{
    m_user.setPassword(password);
}




