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
#include "configremote.h"

#include <QMetaEnum>

ConfigRemote::ConfigRemote()
{
}

ConfigRemote::ConfigRemote(const QJsonValue &value)
{
    QJsonObject obj = value.toObject();
    if(obj.contains(QLatin1String("enabled"))) {
        m_enabled = obj.value(QLatin1String("enabled")).toBool();
    }
    QString v = obj.value(QLatin1String("type")).toString().trimmed().toLower();
    QMetaEnum e = QMetaEnum::fromType<Type>();
    for(int i = 0; i < e.keyCount(); ++i) {
        if(QString::fromLatin1(e.key(i)).compare(v, Qt::CaseInsensitive) == 0) {
            m_type = static_cast<Type>(e.value(i));
            break;
        }
    }

    m_url = QUrl::fromUserInput(obj.value(QLatin1String("url")).toString());
    m_user = ConfigUser(obj.value(QLatin1String("user")));
}

bool ConfigRemote::isEmpty() const
{
    return m_name.isEmpty() && m_url.isEmpty();
}

bool ConfigRemote::enabled() const
{
    return m_enabled;
}

void ConfigRemote::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

ConfigRemote::Type ConfigRemote::type() const
{
    return m_type;
}

void ConfigRemote::setType(ConfigRemote::Type type)
{
    m_type = type;
}

QString ConfigRemote::name() const
{
    return m_name;
}

void ConfigRemote::setName(QString name)
{
    m_name = name;
}

QUrl ConfigRemote::url() const
{
    return m_url;
}

void ConfigRemote::setUrl(QUrl url)
{
    m_url = url;
}

ConfigUser ConfigRemote::user() const
{
    return m_user;
}

void ConfigRemote::setUser(const ConfigUser &user)
{
    m_user = user;
}

QString ConfigRemote::username() const
{
    return m_user.username();
}

void ConfigRemote::setUsername(QString username)
{
    m_user.setUsername(username);
}

QString ConfigRemote::password() const
{
    return m_user.password();
}

void ConfigRemote::setPassword(QString password)
{
    m_user.setPassword(password);
}

QJsonObject ConfigRemote::toJson() const
{
    QJsonObject obj;

    if(!m_enabled) {
        obj.insert(QStringLiteral("enabled"), false);
    }

    if(m_type != ConfigRemote::Jenkins) {
        QMetaEnum e = QMetaEnum::fromType<Type>();
        QString v = QString::fromLatin1(e.valueToKey(m_type));
        obj.insert(QStringLiteral("type"), v);
    }

    if(!m_url.isEmpty()) {
        obj.insert(QStringLiteral("url"), m_url.toString());
    }

    if(!m_user.isEmpty()) {
        obj.insert(QStringLiteral("user"), m_user.toJson());
    }

    return obj;
}

bool operator==(const ConfigRemote &lhs, const ConfigRemote &rhs)
{
    return lhs.enabled() == rhs.enabled()
            && lhs.type() == rhs.type()
            && lhs.name() == rhs.name()
            && lhs.url() == rhs.url()
            && lhs.user() == rhs.user();
}
