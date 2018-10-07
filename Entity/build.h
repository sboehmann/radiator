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
#ifndef ENTITY_BUILD_H
#define ENTITY_BUILD_H

#include "buildcause.h"
#include "culprit.h"

#include <QtCore/QUrl>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QSharedDataPointer>

struct BuildData;

class Build
{
    Q_GADGET

    Q_PROPERTY(QUrl url READ url WRITE setUrl)
    Q_PROPERTY(QString displayName READ displayName WRITE setDisplayName)
    Q_PROPERTY(QString fullDisplayName READ fullDisplayName WRITE setFullDisplayName)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(Status status READ status WRITE setStatus)
    Q_PROPERTY(bool building READ building WRITE setBuilding)
    Q_PROPERTY(int buildNumber READ buildNumber WRITE setBuildNumber)
    Q_PROPERTY(int estimatedDuration READ estimatedDuration WRITE setEstimatedDuration)
    Q_PROPERTY(int duration READ duration WRITE setDuration)
    Q_PROPERTY(BuildCause buildCause READ buildCause WRITE setBuildCause)
    Q_PROPERTY(QList<CulPrit> culPrits READ culPrits WRITE setCulPrits)
    Q_PROPERTY(QDateTime startedAt READ startedAt WRITE setStartedAt)
    Q_PROPERTY(QDateTime updatedAt READ updatedAt WRITE setUpdatedAt)

public:
    enum class Status {
        Unknown,   //!< The status could not be determined
        Disabled,  //!< The job has been disabled
        Success,   //!< Job successfully finished
        Unstable,  //!< The job has a non-stable result
        Failure,   //!< The job has failed
        NoBuild,   //!< The Job has never been build
        Aborted,   //!< Job was already running and has been aborted
        Cancelled  //!< Job was in the queue, but was aborted during the time in the queue
    };
    Q_ENUM(Status)

    Build();
    Build(const Build &rhs);
    Build& operator=(const Build &rhs);
    ~Build();

    QJsonObject toJson() const;

    QByteArray id() const;
    void setId(QByteArray id);

    QString displayName() const;
    void setDisplayName(QString displayName);

    QString fullDisplayName() const;
    void setFullDisplayName(QString fullDisplayName);

    QString description() const;
    void setDescription(QString description);

    QUrl url() const;
    void setUrl(QUrl url);

    Status status() const;
    void setStatus(Status status);

    bool building() const;
    void setBuilding(bool building);

    int buildNumber() const;
    void setBuildNumber(int buildNumber);

    int estimatedDuration() const;
    void setEstimatedDuration(int estimatedDuration);

    int duration() const;
    void setDuration(int duration);

    QDateTime startedAt() const;
    void setStartedAt(const QDateTime &startedAt);

    QDateTime updatedAt() const;
    void setUpdatedAt(const QDateTime &updatedAt);

    BuildCause buildCause() const;
    void setBuildCause(const BuildCause &buildCause);

    QList<CulPrit> culPrits() const;
    void setCulPrits(const QList<CulPrit> &culPrit);

private:
    QSharedDataPointer<BuildData> d;
};


bool operator==(const Build& lhs, const Build& rhs);

inline bool operator!=(const Build& lhs, const Build& rhs)
{
    return !(lhs == rhs);
}

uint qHash(const Build& key, uint seed = 6917);

#endif
