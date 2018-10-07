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
#ifndef JENKINS_CLIENT_H
#define JENKINS_CLIENT_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

#include "Entity/job.h"
#include "Entity/branchfilter.h"

class QNetworkAccessManager;
class JenkinsClientPrivate;


class JenkinsClient : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(int jobRefreshInterval READ jobRefreshInterval WRITE setJobRefreshInterval NOTIFY jobRefreshIntervalChanged)
    Q_PROPERTY(int buildRefreshInterval READ buildRefreshInterval WRITE setBuildRefreshInterval NOTIFY buildRefreshIntervalChanged)
    Q_PROPERTY(QList<BranchFilter> branchFilters READ branchFilters WRITE setBranchFilters NOTIFY branchFiltersChanged)

public:
    explicit JenkinsClient(QObject *parent = nullptr);
    ~JenkinsClient();

    QUrl url() const;
    int jobRefreshInterval() const;
    int buildRefreshInterval() const;
    QString userName() const;
    QString password() const;
    QList<BranchFilter> branchFilters() const;

    QString name() const;

public slots:
    void reload();

    void setName(QString name);
    void setUrl(QUrl url);
    void setJobRefreshInterval(int refreshIntervalInSeconds);
    void setBuildRefreshInterval(int buildRefreshInterval);
    void setUserName(QString userName);
    void setPassword(QString password);
    void setBranchFilters(QList<BranchFilter> branchFilters);

private:
    void reloadBuilds();

signals:
    void nameChanged();
    void urlChanged();
    void jobRefreshIntervalChanged();
    void buildRefreshIntervalChanged();
    void userNameChanged();
    void passwordChanged();

    void jobChanged(Job job);
    void jobAdded(Job job);
    void jobRemoved(Job job);
    void buildChanged(Job job, Build build);
    void branchFiltersChanged();

private:
    Q_DECLARE_PRIVATE(JenkinsClient)
    JenkinsClientPrivate *d_ptr = nullptr;
};

#endif
