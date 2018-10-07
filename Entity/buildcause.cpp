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
#include "buildcause.h"

#include <QJsonObject>
#include <QMetaEnum>

static BuildCause empty;

struct BuildCauseData : public QSharedData
{
    BuildCause::Trigger trigger = BuildCause::Trigger::Unknown;
    QDateTime startedAt;
    QString startedBy;
    QString message;
};


BuildCause::BuildCause()
  : d(new BuildCauseData)
{
}

BuildCause::BuildCause(const BuildCause &rhs)
  : d(rhs.d)
{
}

BuildCause &BuildCause::operator=(const BuildCause &rhs)
{
    if (Q_LIKELY(this != &rhs)) {
        d.operator=(rhs.d);
    }
    return *this;
}

BuildCause::~BuildCause()
{
}

bool BuildCause::isEmpty() const
{
    return *this == empty;
}

QJsonObject BuildCause::toJson() const
{
    QJsonObject obj;
    if(d->trigger != empty.trigger()) {
        QMetaEnum e = QMetaEnum::fromType<BuildCause::Trigger>();
        obj.insert("trigger", e.valueToKey(static_cast<int>(d->trigger)));
    }

    if(d->message != empty.message()) {
        obj.insert("message", d->message);
    }

    if(d->startedAt != empty.startedAt()) {
        obj.insert("startedAt", d->startedAt.toString(Qt::ISODate));
    }

    if(d->startedBy != empty.startedBy()) {
        obj.insert("startedBy", d->startedBy);
    }

    return obj;
}

BuildCause::Trigger BuildCause::trigger() const
{
    return d->trigger;
}

void BuildCause::setTrigger(BuildCause::Trigger trigger)
{
    d->trigger = trigger;
}

QDateTime BuildCause::startedAt() const
{
    return d->startedAt;
}

void BuildCause::setStartedAt(const QDateTime &startedAt)
{
    d->startedAt = startedAt;
}

QString BuildCause::message() const
{
    return d->message;
}

void BuildCause::setMessage(const QString &message)
{
    d->message = message;
}

QString BuildCause::startedBy() const
{
    return d->startedBy;
}

void BuildCause::setStartedBy(const QString &startedBy)
{
    d->startedBy = startedBy;
}

bool operator==(const BuildCause &lhs, const BuildCause &rhs)
{
    return lhs.trigger() == rhs.trigger()
            && lhs.startedAt() == rhs.startedAt()
            && lhs.startedBy() == rhs.startedBy()
            && lhs.message() == rhs.message();
}

uint qHash(const BuildCause &key, uint seed)
{
    return static_cast<int>(key.trigger())
         ^ qHash(key.startedAt(), seed)
         ^ qHash(key.startedBy(), seed)
         ^ qHash(key.message(), seed);
}
