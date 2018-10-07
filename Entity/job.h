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
#ifndef ENTITY_JOB_H
#define ENTITY_JOB_H

#include "Entity/build.h"
#include "Entity/branchfilter.h"

#include <QtCore/QUrl>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtCore/QDateTime>
#include <QtCore/QSharedDataPointer>

struct JobData;

class Job
{
    Q_GADGET

    // TODO remove id and use url only for internal stuff, not for user visible.
    // Then url can truly act as an id (a ciclient could map urls internally or at a custom fragment if needed).
    //
    // TODO add urls for IntegrationServerLink, ConsoleLogLink and CodeBrowserLink...
    //      likely to the Build class and not here.

    Q_PROPERTY(QByteArray id READ id WRITE setId)
    Q_PROPERTY(QUrl url READ url WRITE setUrl)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QMap<int, Build> builds READ builds WRITE setBuilds)
    Q_PROPERTY(bool buildable READ buildable WRITE setBuildable)
    Q_PROPERTY(bool building READ building WRITE setBuilding)
    Q_PROPERTY(bool concurrentBuild READ concurrentBuild WRITE setConcurrentBuild)
    Q_PROPERTY(QString displayName READ displayName WRITE setDisplayName)
    Q_PROPERTY(QString fullDisplayName READ fullDisplayName WRITE setFullDisplayName)
    Q_PROPERTY(QString fullName READ fullName WRITE setFullName)
    Q_PROPERTY(Build::Status status READ status WRITE setStatus)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(bool inQueue READ inQueue WRITE setInQueue)
    Q_PROPERTY(QList<int> buildNumbers READ buildNumbers WRITE setBuildNumbers)
    Q_PROPERTY(int nextBuildNumber READ nextBuildNumber WRITE setNextBuildNumber)
    Q_PROPERTY(int lastBuildNumber READ lastBuildNumber WRITE setLastBuildNumber)
    Q_PROPERTY(int firstBuildNumber READ firstBuildNumber WRITE setFirstBuildNumber)
    Q_PROPERTY(int lastCompletedBuildNumber READ lastCompletedBuildNumber WRITE setLastCompletedBuildNumber)
    Q_PROPERTY(int lastStableBuildNumber READ lastStableBuildNumber WRITE setLastStableBuildNumber)
    Q_PROPERTY(int lastFailedBuildNumber READ lastFailedBuildNumber WRITE setLastFailedBuildNumber)
    Q_PROPERTY(int lastSuccessfulBuildNumber READ lastSuccessfulBuildNumber WRITE setLastSuccessfulBuildNumber)
    Q_PROPERTY(int lastUnstableBuildNumber READ lastUnstableBuildNumber WRITE setLastUnstableBuildNumber)
    Q_PROPERTY(int lastUnsuccessfulBuildNumber READ lastUnsuccessfulBuildNumber WRITE setLastUnsuccessfulBuildNumber)
//    Q_PROPERTY(QList<HealthReport> healthReports READ healthReports WRITE setHealthReports)
    Q_PROPERTY(int priority READ priority WRITE setPriority)
    Q_PROPERTY(BranchFilter::Visibility visibility READ visibility WRITE setVisibility)
    Q_PROPERTY(QDateTime updatedAt READ updatedAt WRITE setUpdatedAt)

public:
    Job();
    Job(const Job &rhs);
    Job& operator=(const Job &rhs);
    ~Job();

    QJsonObject toJson() const;

    QByteArray id() const;
    void setId(QByteArray id);

    QUrl url() const;
    void setUrl(QUrl url);

    QString name() const;
    void setName(QString name);

    QMap<int, Build> builds() const;
    void setBuilds(QMap<int, Build> builds);

    Build build(int number);
    void addBuild(Build build);

    bool buildable() const;
    void setBuildable(bool buildable);

    QString displayName() const;
    void setDisplayName(QString displayName);

    QString fullDisplayName() const;
    void setFullDisplayName(QString fullDisplayName);

    QString fullName() const;
    void setFullName(QString fullName);

    Build::Status status() const;
    void setStatus(Build::Status status);

    bool building() const;
    void setBuilding(bool building);

    QString description() const;
    void setDescription(QString description);

    bool inQueue() const;
    void setInQueue(bool inQueue);

    int nextBuildNumber() const;
    void setNextBuildNumber(int nextBuildNumber);

    bool concurrentBuild() const;
    void setConcurrentBuild(bool concurrentBuild);

    Build lastBuild() const;
    int lastBuildNumber() const;
    void setLastBuildNumber(int lastBuildNumber);

    Build firstBuild() const;
    int firstBuildNumber() const;
    void setFirstBuildNumber(int firstBuildNumber);

    Build lastCompletedBuild() const;
    int lastCompletedBuildNumber() const;
    void setLastCompletedBuildNumber(int lastCompletedBuildNumber);

    Build lastStableBuild() const;
    int lastStableBuildNumber() const;
    void setLastStableBuildNumber(int lastStableBuildNumber);

    Build lastFailedBuild() const;
    int lastFailedBuildNumber() const;
    void setLastFailedBuildNumber(int lastFailedBuildNumber);

    Build lastSuccessfulBuild() const;
    int lastSuccessfulBuildNumber() const;
    void setLastSuccessfulBuildNumber(int lastSuccessfulBuildNumber);

    Build lastUnstableBuild() const;
    int lastUnstableBuildNumber() const;
    void setLastUnstableBuildNumber(int lastUnstableBuildNumber);

    Build lastUnsuccessfulBuild() const;
    int lastUnsuccessfulBuildNumber() const;
    void setLastUnsuccessfulBuildNumber(int lastUnsuccessfulBuildNumber);

    QList<int> buildNumbers() const;
    void setBuildNumbers(QList<int> buildNumbers);

    int priority() const;
    void setPriority(int priority);

    BranchFilter::Visibility visibility() const;
    void setVisibility(BranchFilter::Visibility);

    QDateTime updatedAt() const;
    void setUpdatedAt(QDateTime updatedAt);

private:
    QSharedDataPointer<JobData> d;
};

bool operator==(const Job& lhs, const Job& rhs);

inline bool operator!=(const Job& lhs, const Job& rhs)
{
    return !(lhs == rhs);
}


#endif
