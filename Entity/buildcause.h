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
#ifndef ENTITY_BUILD_CAUSE_H
#define ENTITY_BUILD_CAUSE_H

#include <QtCore/QMetaType>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QDateTime>
#include <QtCore/QString>

struct BuildCauseData;

class BuildCause
{
    Q_GADGET
    Q_PROPERTY(Trigger trigger READ trigger WRITE setTrigger)
    Q_PROPERTY(QDateTime startedAt READ startedAt WRITE setStartedAt)
    Q_PROPERTY(QString startedBy READ startedBy WRITE setStartedBy)
    Q_PROPERTY(QString message READ message WRITE setMessage)

public:
    enum class Trigger
    {
        Unknown,
        SCM,
        Timer,
        User,
        UpstreamProject,
        RemoteHost
    };
    Q_ENUM(Trigger)

    BuildCause();
    BuildCause(const BuildCause &rhs);
    BuildCause& operator=(const BuildCause &rhs);
    ~BuildCause();

    bool isEmpty() const;

    QJsonObject toJson() const;

    Trigger trigger() const;
    void setTrigger(Trigger trigger);

    QDateTime startedAt() const;
    void setStartedAt(const QDateTime &startedAt);

    QString message() const;
    void setMessage(const QString &message);

    QString startedBy() const;
    void setStartedBy(const QString &startedBy);

private:
    QSharedDataPointer<BuildCauseData> d;
};

bool operator==(const BuildCause& lhs, const BuildCause& rhs);

inline bool operator!=(const BuildCause& lhs, const BuildCause& rhs)
{
    return !(lhs == rhs);
}

uint qHash(const BuildCause& key, uint seed = 6917);

#endif
