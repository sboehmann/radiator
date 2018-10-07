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
#include "build.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QMetaEnum>
#include <QUuid>

struct BuildData : public QSharedData
{
    QByteArray id;
    QString displayName;
    QString fullDisplayName;
    QString description;
    QUrl url;
    Build::Status status = Build::Status::Unknown;
    int buildNumber = -1;
    int estimatedDuration = 0;
    int duration = 0;
    BuildCause buildCause;
    QList<CulPrit> culPrits;
    QDateTime startedAt;
    QDateTime updatedAt = QDateTime::currentDateTime();
    bool building = false;
};

Build::Build()
  : d(new BuildData)
{
}

Build::Build(const Build &other)
  : d(other.d)
{
}

Build::~Build()
{
}

Build &Build::operator=(const Build &rhs)
{
    if(Q_LIKELY(this != &rhs)) {
        d.operator=(rhs.d);
    }

    return *this;
}

QJsonObject Build::toJson() const
{
    QJsonObject obj;

    obj.insert("id", QString::fromLatin1(d->id));
    obj.insert("displayName", d->displayName);
    obj.insert("fullDisplayName", d->fullDisplayName);
    obj.insert("description", d->description);
    obj.insert("url", d->url.toString());
    QMetaEnum e = QMetaEnum::fromType<Build::Status>();
    obj.insert("status", e.valueToKey(static_cast<int>(d->status)));
    obj.insert("building", d->building);
    obj.insert("buildNumber", d->buildNumber);
    obj.insert("estimatedDuration", d->estimatedDuration);
    obj.insert("duration", d->duration);

    if(!d->buildCause.isEmpty()) {
        obj.insert("buildCause", d->buildCause.toJson());
    }

    if(!d->culPrits.isEmpty()) {
        QJsonArray arr;
        foreach(const CulPrit &p, d->culPrits) {
            arr.append(p.toJson());
        }

        obj.insert("culPrits", arr);
    }

    obj.insert("startedAt", d->startedAt.toString(Qt::ISODate));
    obj.insert("updatedAt", d->updatedAt.toString(Qt::ISODate));

    return obj;
}

QByteArray Build::id() const
{
    return d->id;
}

void Build::setId(QByteArray id)
{
    d->id = id;
}

QString Build::displayName() const
{
    return d->displayName;
}

void Build::setDisplayName(QString displayName)
{
    d->displayName = displayName;
}

QString Build::fullDisplayName() const
{
    return d->fullDisplayName;
}

void Build::setFullDisplayName(QString fullDisplayName)
{
    d->fullDisplayName = fullDisplayName;
}

QString Build::description() const
{
    return d->description;
}

void Build::setDescription(QString description)
{
    d->description = description;
}

QUrl Build::url() const
{
    return d->url;
}

void Build::setUrl(QUrl url)
{
    d->url = url;
}

Build::Status Build::status() const
{
    return d->status;
}

void Build::setStatus(Build::Status status)
{
    d->status = status;
}

bool Build::building() const
{
    return d->building;
}

void Build::setBuilding(bool building)
{
    d->building = building;
}

int Build::buildNumber() const
{
    return d->buildNumber;
}

void Build::setBuildNumber(int buildNumber)
{
    d->buildNumber = buildNumber;
}

int Build::estimatedDuration() const
{
    return d->estimatedDuration;
}

void Build::setEstimatedDuration(int estimatedDuration)
{
    d->estimatedDuration = estimatedDuration;
}

int Build::duration() const
{
    return d->duration;
}

void Build::setDuration(int duration)
{
    d->duration = duration;
}

BuildCause Build::buildCause() const
{
    return d->buildCause;
}

void Build::setBuildCause(const BuildCause &buildCause)
{
    d->buildCause = buildCause;
}

QList<CulPrit> Build::culPrits() const
{
    return d->culPrits;
}

void Build::setCulPrits(const QList<CulPrit> &culPrits)
{
    d->culPrits = culPrits;
}

QDateTime Build::startedAt() const
{
    return d->startedAt;
}

void Build::setStartedAt(const QDateTime &startedAt)
{
    d->startedAt = startedAt;
}

QDateTime Build::updatedAt() const
{
    return d->updatedAt;
}

void Build::setUpdatedAt(const QDateTime &updatedAt)
{
    d->updatedAt = updatedAt;
}

bool operator==(const Build &lhs, const Build &rhs)
{
    return lhs.id() == rhs.id()
        && lhs.url() == rhs.url()
        && lhs.displayName() == rhs.displayName()
        && lhs.fullDisplayName() == rhs.fullDisplayName()
        && lhs.description() == rhs.description()
        && lhs.status() == rhs.status()
        && lhs.building() == rhs.building()
        && lhs.buildNumber() == rhs.buildNumber()
        && lhs.estimatedDuration() == rhs.estimatedDuration()
        && lhs.duration() == rhs.duration()
        && lhs.buildCause() == rhs.buildCause()
        && lhs.culPrits() == rhs.culPrits()
        && lhs.startedAt() == rhs.startedAt()
        && lhs.updatedAt() == rhs.updatedAt();
}

uint qHash(const Build &key, uint seed)
{
    return qHash(key.id(), seed)
         ^ qHash(key.url(), seed)
         ^ qHash(key.displayName(), seed)
         ^ qHash(key.fullDisplayName(), seed)
         ^ qHash(key.description(), seed)
         ^ qHash(static_cast<int>(key.status()), seed)
         ^ qHash(key.building(), seed)
         ^ qHash(key.buildNumber(), seed)
         ^ qHash(key.estimatedDuration(), seed)
         ^ qHash(key.duration(), seed)
         ^ qHash(key.buildCause(), seed)
         ^ qHash(key.culPrits(), seed)
         ^ qHash(key.startedAt(), seed)
         ^ qHash(key.updatedAt(), seed);
}
