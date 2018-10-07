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
#include "job.h"

#include <QUuid>
#include <QJsonObject>
#include <QJsonArray>
#include <QMetaEnum>

struct JobData : public QSharedData
{
    QByteArray id;
    QUrl url;
    QString name;
    QMap<int, Build> builds;
    bool buildable = false;
    QString displayName;
    QString fullDisplayName;
    QString fullName;
    Build::Status status = Build::Status::Unknown;
    bool building = false;
    QString description;
    bool inQueue = false;
    bool concurrentBuild = false;
    int nextBuildNumber = -1;
    int lastBuildNumber = -1;
    int firstBuildNumber = -1;
    int lastCompletedBuildNumber = -1;
    int lastStableBuildNumber = -1;
    int lastFailedBuildNumber = -1;
    int lastSuccessfulBuildNumber = -1;
    int lastUnstableBuildNumber = -1;
    int lastUnsuccessfulBuildNumber = -1;
    QList<int> buildNumbers;
    int priority = 0;
    BranchFilter::Visibility visibility = BranchFilter::Visibility::Always;
    QDateTime updatedAt = QDateTime::currentDateTime();
};

Job::Job()
    : d(new JobData)
{
}

Job::Job(const Job &other)
    : d(other.d)
{
}

Job::~Job()
{
}

QJsonObject Job::toJson() const
{
    QJsonObject obj;

    obj.insert("id", QString::fromLatin1(d->id));
    obj.insert("url", d->url.toString());
    obj.insert("name", d->name);
    obj.insert("buildable", d->buildable);
    obj.insert("displayName", d->displayName);
    obj.insert("fullDisplayName", d->fullDisplayName);
    obj.insert("fullName", d->fullName);
    QMetaEnum e = QMetaEnum::fromType<Build::Status>();
    obj.insert("status", e.valueToKey(static_cast<int>(d->status)));
    obj.insert("building", d->building);
    obj.insert("description", d->description);
    obj.insert("inQueue", d->inQueue);
    obj.insert("concurrentBuild", d->concurrentBuild);
    obj.insert("nextBuildNumber", d->nextBuildNumber);
    obj.insert("lastBuildNumber", d->lastBuildNumber);
    obj.insert("firstBuildNumber", d->firstBuildNumber);
    obj.insert("lastCompletedBuildNumber", d->lastCompletedBuildNumber);
    obj.insert("lastStableBuildNumber", d->lastStableBuildNumber);
    obj.insert("lastFailedBuildNumber", d->lastFailedBuildNumber);
    obj.insert("lastSuccessfulBuildNumber", d->lastSuccessfulBuildNumber);
    obj.insert("lastUnstableBuildNumber", d->lastUnstableBuildNumber);
    obj.insert("lastUnsuccessfulBuildNumber", d->lastUnsuccessfulBuildNumber);
    obj.insert("updatedAt", d->updatedAt.toString(Qt::ISODate));

    QJsonArray arr;
    for(auto it = d->builds.constBegin(); it != d->builds.constEnd(); ++it) {
        arr.append(it.value().toJson());
    }
    obj.insert("builds", arr);

    return obj;
}

Job &Job::operator=(const Job &rhs)
{
    if(Q_LIKELY(this != &rhs)) {
        d.operator=(rhs.d);
    }

    return *this;
}

QByteArray Job::id() const
{
    return d->id;
}

void Job::setId(QByteArray id)
{
    d->id = id;
}

QUrl Job::url() const
{
    return d->url;
}

void Job::setUrl(QUrl url)
{
    d->url = url;
}

QString Job::name() const
{
    return d->name;
}

void Job::setName(QString name)
{
    d->name = name;
}

QMap<int, Build> Job::builds() const
{
    return d->builds;
}

void Job::setBuilds(QMap<int, Build> builds)
{
    d->builds = builds;
}

Build Job::build(int number)
{
    return d->builds.value(number);
}

void Job::addBuild(Build build)
{
    d->builds.insert(build.buildNumber(), build);
}

bool Job::buildable() const
{
    return d->buildable;
}

void Job::setBuildable(bool buildable)
{
    d->buildable = buildable;
}

QString Job::displayName() const
{
    if(d->displayName.isEmpty()) {
        return d->name;
    }

    return d->displayName;
}

void Job::setDisplayName(QString displayName)
{
    d->displayName = displayName;
}

void Job::setFullDisplayName(QString fullDisplayName)
{
    d->fullDisplayName = fullDisplayName;
}

QString Job::fullDisplayName() const
{
    return d->fullDisplayName;
}

QString Job::fullName() const
{
    return d->fullName;
}

void Job::setFullName(QString fullName)
{
    d->fullName = fullName;
}

Build::Status Job::status() const
{
    return d->status;
}

void Job::setStatus(Build::Status status)
{
    d->status = status;
}

bool Job::building() const
{
    return d->building;
}

void Job::setBuilding(bool building)
{
    d->building = building;
}

QString Job::description() const
{
    return d->description;
}

void Job::setDescription(QString description)
{
    d->description = description;
}

bool Job::inQueue() const
{
    return d->inQueue;
}

void Job::setInQueue(bool inQueue)
{
    d->inQueue = inQueue;
}

int Job::nextBuildNumber() const
{
    return d->nextBuildNumber;
}

void Job::setNextBuildNumber(int nextBuildNumber)
{
    d->nextBuildNumber = nextBuildNumber;
}

bool Job::concurrentBuild() const
{
    return d->concurrentBuild;
}

void Job::setConcurrentBuild(bool concurrentBuild)
{
    d->concurrentBuild = concurrentBuild;
}

Build Job::lastBuild() const
{
    return d->builds.value(lastBuildNumber());
}

int Job::lastBuildNumber() const
{
    return d->lastBuildNumber;
}

void Job::setLastBuildNumber(int lastBuildNumber)
{
    d->lastBuildNumber = lastBuildNumber;
}

Build Job::firstBuild() const
{
    return d->builds.value(firstBuildNumber());
}

int Job::firstBuildNumber() const
{
    return d->firstBuildNumber;
}

void Job::setFirstBuildNumber(int firstBuildNumber)
{
    d->firstBuildNumber = firstBuildNumber;
}

Build Job::lastCompletedBuild() const
{
    return d->builds.value(lastCompletedBuildNumber());
}

int Job::lastCompletedBuildNumber() const
{
    return d->lastCompletedBuildNumber;
}

void Job::setLastCompletedBuildNumber(int lastCompletedBuildNumber)
{
    d->lastCompletedBuildNumber = lastCompletedBuildNumber;
}

Build Job::lastStableBuild() const
{
    return d->builds.value(lastStableBuildNumber());
}

int Job::lastStableBuildNumber() const
{
    return d->lastStableBuildNumber;
}

void Job::setLastStableBuildNumber(int lastStableBuildNumber)
{
    d->lastStableBuildNumber = lastStableBuildNumber;
}

Build Job::lastFailedBuild() const
{
    return d->builds.value(lastFailedBuildNumber());
}

int Job::lastFailedBuildNumber() const
{
    return d->lastFailedBuildNumber;
}

void Job::setLastFailedBuildNumber(int lastFailedBuildNumber)
{
    d->lastFailedBuildNumber = lastFailedBuildNumber;
}

Build Job::lastSuccessfulBuild() const
{
    return d->builds.value(lastSuccessfulBuildNumber());
}

int Job::lastSuccessfulBuildNumber() const
{
    return d->lastSuccessfulBuildNumber;
}

void Job::setLastSuccessfulBuildNumber(int lastSuccessfulBuildNumber)
{
    d->lastSuccessfulBuildNumber = lastSuccessfulBuildNumber;
}

Build Job::lastUnstableBuild() const
{
    return d->builds.value(lastUnstableBuildNumber());
}

int Job::lastUnstableBuildNumber() const
{
    return d->lastUnstableBuildNumber;
}

void Job::setLastUnstableBuildNumber(int lastUnstableBuildNumber)
{
    d->lastUnstableBuildNumber = lastUnstableBuildNumber;
}

Build Job::lastUnsuccessfulBuild() const
{
    return d->builds.value(lastUnsuccessfulBuildNumber());
}

int Job::lastUnsuccessfulBuildNumber() const
{
    return d->lastUnsuccessfulBuildNumber;
}

void Job::setLastUnsuccessfulBuildNumber(int lastUnsuccessfulBuildNumber)
{
    d->lastUnsuccessfulBuildNumber = lastUnsuccessfulBuildNumber;
}

QList<int> Job::buildNumbers() const
{
    return d->buildNumbers;
}

void Job::setBuildNumbers(QList<int> buildNumbers)
{
    d->buildNumbers = buildNumbers;
}

int Job::priority() const
{
    return d->priority;
}

void Job::setPriority(int priority)
{
    d->priority = priority;
}

BranchFilter::Visibility Job::visibility() const
{
    return d->visibility;
}

void Job::setVisibility(BranchFilter::Visibility visibility)
{
    d->visibility = visibility;
}

QDateTime Job::updatedAt() const
{
    return d->updatedAt;
}

void Job::setUpdatedAt(QDateTime updatedAt)
{
    d->updatedAt = updatedAt;
}

bool operator==(const Job &lhs, const Job &rhs)
{
    return lhs.id() == rhs.id()
        && lhs.url() == rhs.url()
        && lhs.name() == rhs.name()
        && lhs.builds() == rhs.builds()
        && lhs.buildable() == rhs.buildable()
        && lhs.building() == rhs.building()
        && lhs.concurrentBuild() == rhs.concurrentBuild()
        && lhs.displayName() == rhs.displayName()
        && lhs.fullDisplayName() == rhs.fullDisplayName()
        && lhs.fullName() == rhs.fullName()
        && lhs.status() == rhs.status()
        && lhs.description() == rhs.description()
        && lhs.inQueue() == rhs.inQueue()
        && lhs.buildNumbers() == rhs.buildNumbers()
        && lhs.nextBuildNumber() == rhs.nextBuildNumber()
        && lhs.lastBuildNumber() == rhs.lastBuildNumber()
        && lhs.firstBuildNumber() == rhs.firstBuildNumber()
        && lhs.lastCompletedBuildNumber() == rhs.lastCompletedBuildNumber()
        && lhs.lastStableBuildNumber() == rhs.lastStableBuildNumber()
        && lhs.lastFailedBuildNumber() == rhs.lastFailedBuildNumber()
        && lhs.lastSuccessfulBuildNumber() == rhs.lastSuccessfulBuildNumber()
        && lhs.lastUnstableBuildNumber() == rhs.lastUnstableBuildNumber()
        && lhs.lastUnsuccessfulBuildNumber() == rhs.lastUnsuccessfulBuildNumber()
        && lhs.priority() == rhs.priority()
        && lhs.visibility() == rhs.visibility()
        && lhs.updatedAt() == rhs.updatedAt();
}
