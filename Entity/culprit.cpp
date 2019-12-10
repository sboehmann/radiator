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
#include "culprit.h"
#include <QJsonObject>


static CulPrit empty;

struct CulPritData : public QSharedData
{
    QString id;
    QString name;
    QString description;
    QUrl url;
};


CulPrit::CulPrit()
  : d(new CulPritData)
{
}

CulPrit::CulPrit(const QJsonValue &value)
  : d(new CulPritData)
{
    QJsonObject obj = value.toObject();
    d->id = obj.value(QLatin1String("id")).toString();
    d->name = obj.value(QLatin1String("fullName")).toString();
    d->description = obj.value(QLatin1String("description")).toString();
    d->url = QUrl::fromUserInput(obj.value(QLatin1String("absoluteUrl")).toString());
}

CulPrit::CulPrit(const CulPrit &rhs)
  : d(rhs.d)
{
}

CulPrit &CulPrit::operator=(const CulPrit &rhs)
{
    if (Q_LIKELY(this != &rhs)) {
        d.operator=(rhs.d);
    }
    return *this;
}

CulPrit::~CulPrit()
{
}

bool CulPrit::isEmpty() const
{
    return *this == empty;
}

QJsonObject CulPrit::toJson() const
{
    QJsonObject obj;

    if(d->id != empty.id()) {
        obj.insert(QStringLiteral("id"), d->id);
    }

    if(d->name != empty.name()) {
        obj.insert(QStringLiteral("name"), d->name);
    }

    if(d->description != empty.description()) {
        obj.insert(QStringLiteral("description"), d->description);
    }

    if(d->url != empty.url()) {
        obj.insert(QStringLiteral("url"), d->url.toString());
    }

    return obj;
}

QString CulPrit::id() const
{
    return d->id;
}

void CulPrit::setId(QString id)
{
    d->id = id;
}

QString CulPrit::name() const
{
    return d->name;
}

void CulPrit::setName(QString name)
{
    d->name = name;
}

QUrl CulPrit::url() const
{
    return d->url;
}

void CulPrit::setUrl(QUrl url)
{
    d->url = url;
}

QString CulPrit::description() const
{
    return d->description;
}

void CulPrit::setDescription(QString description)
{
    d->description = description;
}

bool operator==(const CulPrit &lhs, const CulPrit &rhs)
{
    return lhs.id() == rhs.id()
        && lhs.name() == rhs.name()
        && lhs.description() == rhs.description()
        && lhs.url() == rhs.url();
}

uint qHash(const CulPrit &key, uint seed)
{
    return qHash(key.id(), seed)
         ^ qHash(key.name(), seed)
         ^ qHash(key.description(), seed)
         ^ qHash(key.url(), seed);
}

