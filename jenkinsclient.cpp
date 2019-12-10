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
#include "jenkinsclient.h"
#include "jenkinsclient_p.h"

#include <QTimer>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QSslPreSharedKeyAuthenticator>
#include <QRegularExpression>

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <iostream>

Q_LOGGING_CATEGORY(logJenkinsClient, "radiator.jenkins.client")

JenkinsClientPrivate::JenkinsClientPrivate(JenkinsClient *q)
  : QObject(q),
    q_ptr(q)
{
    Q_CHECK_PTR(q_func());

    connect(&jobRefreshTimer, &QTimer::timeout, this, &JenkinsClientPrivate::reload);
    connect(&buildRefreshTimer, &QTimer::timeout, this, &JenkinsClientPrivate::reloadBuilds);

    connect(&manager, &QNetworkAccessManager::authenticationRequired, this, &JenkinsClientPrivate::authenticationRequired);
    connect(&manager, &QNetworkAccessManager::sslErrors, this, &JenkinsClientPrivate::sslErrors);
    connect(&manager, &QNetworkAccessManager::networkAccessibleChanged, this, &JenkinsClientPrivate::networkAccessibleChanged);
    connect(&manager, &QNetworkAccessManager::preSharedKeyAuthenticationRequired, this, &JenkinsClientPrivate::preSharedKeyAuthenticationRequired);
    connect(&manager, &QNetworkAccessManager::proxyAuthenticationRequired, this, &JenkinsClientPrivate::proxyAuthenticationRequired);
}

JenkinsClientPrivate::~JenkinsClientPrivate()
{
}

void JenkinsClientPrivate::reload()
{
    if(Q_UNLIKELY(!url.isValid())) {
        qCWarning(logJenkinsClient) << "reload failed: url is not valid.";
        return;
    }

    QNetworkReply *reply = makeRequest(url, QUrlQuery(QStringLiteral("tree=jobs[name,url]")));
    connect(reply, &QNetworkReply::finished, [&,reply]() {
        jobListRequestFinished(reply);
    });
}

QNetworkReply *JenkinsClientPrivate::makeRequest(QUrl requestUrl, const QUrlQuery &query)
{
    requestUrl.setPath(requestUrl.path() + QStringLiteral("/api/json"));
    if(!query.isEmpty()) {
        requestUrl.setQuery(query);
    }

    QNetworkRequest request(requestUrl);
    request.setMaximumRedirectsAllowed(6);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    if(Q_LIKELY(!userName.isEmpty() || !password.isEmpty())) {
        QString concatenated = QString(QStringLiteral("%1:%2")).arg(userName).arg(password);
        QByteArray data = concatenated.toLocal8Bit().toBase64();
        QString headerData = QStringLiteral("Basic ") + data;
        request.setRawHeader(QByteArrayLiteral("Authorization"), headerData.toLocal8Bit());
    }

    return manager.get(request);
}

void JenkinsClientPrivate::jobListRequestFinished(QNetworkReply *reply)
{
    qCDebug(logJenkinsClient) << "reloading job list";

    bool activeTimer = buildRefreshTimer.isActive();
    if(activeTimer) {
        buildRefreshTimer.stop();
    }

    QJsonParseError error;
    QByteArray res  = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(res, &error);
    if(error.error != QJsonParseError::NoError) {
        qCDebug(logJenkinsClient)
                << "Jenkins client request"
                << reply->url() << "failed to parse:"
                << error.errorString() << res;
        return;
    }

    QHash<QUrl, Job> prev = jobs;
    jobs.clear();

    QJsonArray data = doc.object().value(QStringLiteral("jobs")).toArray();
    for(auto it = data.constBegin(); it != data.constEnd(); ++it) {
        QJsonObject obj = it->toObject();
        if(obj.isEmpty() || !obj.contains(QStringLiteral("url"))) {
            continue;
        }

        QUrl url = QUrl::fromUserInput(obj.value(QStringLiteral("url")).toString());
        Job j = prev.value(url);
        j.setId(url.toString().toLatin1());
        j.setUrl(url);
        j.setName(obj.value(QStringLiteral("name")).toString());

        QList<BranchFilter> filters;
        for(auto it = branchFilters.cbegin(); it != branchFilters.cend(); ++it) {
            if(it->remotes().isEmpty() || it->remotes().contains(name)) {
                filters.append(*it);
            }
        }

        bool addJob = filters.isEmpty();
        for(auto it = filters.cbegin(); it != filters.cend(); ++it) {
            if(it->visibility() == BranchFilter::Visibility::Never) {
                continue;
            }

            foreach(const QString &f, it->filters()) {
                QRegularExpression re(f);
                if(Q_UNLIKELY(!re.isValid())) {
                    qCWarning(logJenkinsClient) << "Failed to compile regular expression"
                                                << f << ":" << re.errorString();
                    continue;
                }

                if(re.match(j.name()).hasMatch()) {
                    addJob = true;
                    j.setPriority(it->priority());
                    j.setVisibility(it->visibility());
                    break;
                }
            }

            if(addJob) {
                break;
            }
        }

        if(addJob) {
            jobs.insert(j.url(), j);

            if(prev.contains(j.url())) {
                auto oj = prev.take(j.url());
                if(oj.id() != j.id() || oj.name() != j.name()) {
                    emit jobChanged(j);
                }
            }
            else {
                qCInfo(logJenkinsClient).nospace().noquote()
                    << "Start watching changes for job '" << j.name() << "'.";

                emit jobAdded(j);
            }
        }
        else {
            qCInfo(logJenkinsClient).nospace().noquote()
                << "Job '" << j.name() << "' skipped.";
        }
    }

    reply->deleteLater();

    foreach(const Job &j, prev) {
        qCInfo(logJenkinsClient).nospace().noquote()
            << "Stop watching changes for job '" << j.name() << "'.";
        emit jobRemoved(j);
    }

    foreach(const Job &j, jobs) {
        auto nreply = makeRequest(j.url(), QUrlQuery());
        connect(nreply, &QNetworkReply::finished, this, [&, nreply, j]() { jobRequestFinished(j, nreply); }, Qt::QueuedConnection);
    }

    if(activeTimer) {
        buildRefreshTimer.start();
    }
    reloadBuilds();
}

void JenkinsClientPrivate::reloadBuilds()
{
    qCDebug(logJenkinsClient) << "Reloading build status for" << jobs.size() << "jobs.";

    foreach(const Job &j, jobs) {
        auto nreply = makeRequest(j.url(), QUrlQuery());
        connect(nreply, &QNetworkReply::finished, this, [&, nreply, j]() {
            jobRequestFinished(j, nreply); }, Qt::QueuedConnection);
    }
}

void JenkinsClientPrivate::jobRequestFinished(Job job, QNetworkReply *reply)
{
    QJsonParseError error;
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);

    if(error.error == QJsonParseError::NoError) {
        QJsonObject root = doc.object();
        auto getBuildFn = [&](const QJsonValue &v) {
            QJsonObject obj = v.toObject();
            QUrl url = QUrl::fromUserInput(obj.value("url").toString());
            int number = obj.value("number").toInt(-1);
            if(number >= 0 && url.isValid()) {
                Build b = job.build(number);
                b.setId(url.toString().toLatin1());
                b.setUrl(url);
                b.setBuildNumber(number);
                job.addBuild(b);

                return b.buildNumber();
            }

            return -1;
        };

        parseStatus(job, root.value(QLatin1String("color")));
        job.setBuildable(root.value(QLatin1String("buildable")).toBool());
        job.setConcurrentBuild(root.value(QLatin1String("concurrentBuild")).toBool());
        job.setDisplayName(root.value(QLatin1String("displayName")).toString());
        job.setFullDisplayName(root.value(QLatin1String("fullDisplayName")).toString());
        job.setFullName(root.value(QLatin1String("fullName")).toString());
        job.setDescription(root.value(QLatin1String("description")).toString());
        job.setInQueue(root.value(QLatin1String("inQueue")).toBool());
        job.setNextBuildNumber(root.value(QLatin1String("nextBuildNumber")).toInt());

        job.setLastBuildNumber(getBuildFn(root.value(QLatin1String("lastBuild"))));
        job.setFirstBuildNumber(getBuildFn(root.value(QLatin1String("firstBuild"))));
        job.setLastCompletedBuildNumber(getBuildFn(root.value(QLatin1String("lastCompletedBuild"))));
        job.setLastStableBuildNumber(getBuildFn(root.value(QLatin1String("lastStableBuild"))));
        job.setLastFailedBuildNumber(getBuildFn(root.value(QLatin1String("lastFailedBuild"))));
        job.setLastSuccessfulBuildNumber(getBuildFn(root.value(QLatin1String("lastSuccessfulBuild"))));
        job.setLastUnstableBuildNumber(getBuildFn(root.value(QLatin1String("lastUnstableBuild"))));
        job.setLastUnsuccessfulBuildNumber(getBuildFn(root.value(QStringLiteral("lastUnsuccessfulBuild"))));

        {
            QJsonArray buildArray = root.value(QLatin1String("builds")).toArray();
            QList<int> buildNumbers;
            for(auto it = buildArray.constBegin(); it != buildArray.constEnd(); ++it) {
                buildNumbers.append(getBuildFn(*it));
            }
            job.setBuildNumbers(buildNumbers);
        }

        if(jobs.value(job.url()) != job) {
            jobs.insert(job.url(), job);
            emit jobChanged(job);
        }

        foreach(const Build &b, job.builds()) {
            auto nreply = makeRequest(b.url(), QUrlQuery());
            connect(nreply, &QNetworkReply::finished, this, [&, nreply, job, b]() {
                buildRequestFinished(job.url(), b, nreply); }, Qt::QueuedConnection);
        }
    }
    else {
        qCDebug(logJenkinsClient) << "Reply parser error" << error.errorString()
                                  << "for job" << reply->url();
    }

    reply->deleteLater();
}

void JenkinsClientPrivate::buildRequestFinished(QUrl jobUrl, Build build, QNetworkReply *reply)
{
    QJsonParseError error;
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);

    reply->deleteLater();

    if(error.error != QJsonParseError::NoError) {
        qCDebug(logJenkinsClient) << "Build request parser error" << error.errorString() << "for build" << build.buildNumber() << ", job" << jobUrl;

        return;
    }

    QJsonObject root = doc.object();
    build.setBuilding(root.value(QLatin1String("building")).toBool());
    build.setDisplayName(root.value(QLatin1String("displayName")).toString());
    build.setFullDisplayName(root.value(QLatin1String("fullDisplayName")).toString());
    build.setDescription(root.value(QLatin1String("description")).toString());
    build.setDuration(root.value(QLatin1String("duration")).toInt());
    build.setEstimatedDuration(root.value(QLatin1String("estimatedDuration")).toInt());
    build.setStartedAt(QDateTime::fromMSecsSinceEpoch(root.value(QLatin1String("timestamp")).toVariant().toLongLong()));

    if(root.contains("culprits")) {
        QJsonArray culprits = root.value("culprits").toArray();
        QList<CulPrit> list;
        for(int i = 0; i < culprits.size(); ++i) {
            CulPrit cp(culprits[i]);
            if(!cp.isEmpty()) {
                list.append(cp);
            }
        }
        build.setCulPrits(list);
    }

    QString result = root.value(QLatin1String("result")).toString().toLower().trimmed();
    if(result == QStringLiteral("success")) {
        build.setStatus(Build::Status::Success);
    }
    else if(result == QStringLiteral("unstable")) {
        build.setStatus(Build::Status::Unstable);
    }
    else if(result == QStringLiteral("failure")) {
        build.setStatus(Build::Status::Failure);
    }
    else if(result == QStringLiteral("aborted")) {
        build.setStatus(Build::Status::Aborted);
    }
    else if(result == QStringLiteral("not_build")) {
        build.setStatus(Build::Status::NoBuild);
    }
    else {
        qCDebug(logJenkinsClient) << "Unknown build status" << result;
        build.setStatus(Build::Status::Unknown);
    }

    if(Q_UNLIKELY(!jobs.contains(jobUrl))) {
        qCDebug(logJenkinsClient) << "Job not found.";
    }

    Job job = jobs.value(jobUrl);
    job.addBuild(build);
    jobs.insert(job.url(), job);

    emit buildChanged(job, build);
}

void JenkinsClientPrivate::parseStatus(Job &job, const QJsonValue &value)
{
    static QHash<QString, Build::Status> statusTable({
        { QStringLiteral("red"),            Build::Status::Failure  },
        { QStringLiteral("red_anime"),      Build::Status::Failure  },
        { QStringLiteral("yellow"),         Build::Status::Unstable },
        { QStringLiteral("yellow_anime"),   Build::Status::Unstable },
        { QStringLiteral("blue"),           Build::Status::Success  },
        { QStringLiteral("blue_anime"),     Build::Status::Success  },
        { QStringLiteral("grey"),           Build::Status::Unknown  },
        { QStringLiteral("grey_anime"),     Build::Status::Unknown  },
        { QStringLiteral("disabled"),       Build::Status::Disabled },
        { QStringLiteral("disabled_anime"), Build::Status::Disabled },
        { QStringLiteral("aborted"),        Build::Status::Aborted  },
        { QStringLiteral("aborted_anime"),  Build::Status::Aborted  },
        { QStringLiteral("notbuilt"),       Build::Status::NoBuild  },
        { QStringLiteral("notbuilt_anime"), Build::Status::NoBuild  }
    });

    const QString stat = value.toString().toLower();
    if(Q_UNLIKELY(!statusTable.contains(stat))) {
        qCDebug(logJenkinsClient) << "Unknown status parsed:" << stat;
    }
    job.setStatus(statusTable.value(stat, Build::Status::Unknown));
    job.setBuilding(stat.endsWith(QStringLiteral("_anime")));
}

void JenkinsClientPrivate::authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    Q_UNUSED(reply);
    Q_UNUSED(authenticator);

    qCDebug(logJenkinsClient) << "authenticationRequired";
}

void JenkinsClientPrivate::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    for(const QSslError &err : errors) {
        if(err.error() == QSslError::UnableToGetLocalIssuerCertificate) {
            continue;
        }
        if(err.error() == QSslError::UnableToVerifyFirstCertificate) {
            continue;
        }

        qCDebug(logJenkinsClient) << "ssl errors:" << errors;
        return;
    }

    reply->ignoreSslErrors(errors);
}

void JenkinsClientPrivate::networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible)
{
    qCDebug(logJenkinsClient) << "networkAccessibleChanged:" << accessible;
}

void JenkinsClientPrivate::preSharedKeyAuthenticationRequired(QNetworkReply *reply, QSslPreSharedKeyAuthenticator *authenticator)
{
    Q_UNUSED(reply);
    Q_UNUSED(authenticator);

    qCDebug(logJenkinsClient) << "preSharedKeyAuthenticationRequired";
}

void JenkinsClientPrivate::proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator)
{
    Q_UNUSED(proxy);
    Q_UNUSED(authenticator);

    qCDebug(logJenkinsClient) << "proxyAuthenticationRequired";
}

// ---------------------------------------------------------------------------

JenkinsClient::JenkinsClient(QObject *parent)
    : QObject(parent),
      d_ptr(new JenkinsClientPrivate(this))
{
    Q_D(JenkinsClient);

    connect(d, &JenkinsClientPrivate::jobAdded, this, &JenkinsClient::jobAdded);
    connect(d, &JenkinsClientPrivate::jobRemoved, this, &JenkinsClient::jobRemoved);
    connect(d, &JenkinsClientPrivate::jobChanged, this, &JenkinsClient::jobChanged);
    connect(d, &JenkinsClientPrivate::buildChanged, this, &JenkinsClient::buildChanged);
}

JenkinsClient::~JenkinsClient()
{
    delete d_ptr;
}

QString JenkinsClient::name() const
{
    return d_func()->name;
}

void JenkinsClient::setName(QString name)
{
    Q_D(JenkinsClient);

    if (d->name == name) {
        d->name = name;
        emit nameChanged();
    }
}

QUrl JenkinsClient::url() const
{
    return d_func()->url;
}

void JenkinsClient::setUrl(QUrl url)
{
    Q_D(JenkinsClient);

    if (d->url != url) {
        if(!url.userName().isEmpty()) {
            setUserName(url.userName());
        }

        if(!url.password().isEmpty()) {
            setPassword(url.password());
        }

        d->url = url.adjusted(QUrl::RemoveUserInfo);
        emit urlChanged();
    }
}

int JenkinsClient::jobRefreshInterval() const
{
    return d_func()->jobRefreshInterval;
}

void JenkinsClient::setJobRefreshInterval(int refreshIntervalInSeconds)
{
    Q_D(JenkinsClient);

    if (d->jobRefreshInterval != refreshIntervalInSeconds) {
        d->jobRefreshTimer.stop();
        d->jobRefreshInterval = qMax(0, refreshIntervalInSeconds);
        if(d->jobRefreshInterval > 0) {
            d->jobRefreshTimer.start(d->jobRefreshInterval * 1000);
        }

        emit jobRefreshIntervalChanged();
    }
}

int JenkinsClient::buildRefreshInterval() const
{
    return d_func()->buildRefreshInterval;
}

void JenkinsClient::setBuildRefreshInterval(int refreshIntervalInSeconds)
{
    Q_D(JenkinsClient);

    if (d->buildRefreshInterval != refreshIntervalInSeconds) {
        d->buildRefreshTimer.stop();
        d->buildRefreshInterval = qMax(0, refreshIntervalInSeconds);
        if(d->buildRefreshInterval > 0) {
            d->buildRefreshTimer.start(d->buildRefreshInterval * 1000);
        }

        emit buildRefreshIntervalChanged();
    }
}

QString JenkinsClient::userName() const
{
    return d_func()->userName;
}

void JenkinsClient::setUserName(QString userName)
{
    Q_D(JenkinsClient);

    if (d->userName != userName) {
        d->userName = userName;
        emit userNameChanged();
    }
}

QString JenkinsClient::password() const
{
    return d_func()->password;
}

void JenkinsClient::setPassword(QString password)
{
    Q_D(JenkinsClient);

    if (d->password != password) {
        d->password = password;
        emit passwordChanged();
    }
}

QList<BranchFilter> JenkinsClient::branchFilters() const
{
    return d_func()->branchFilters;
}

void JenkinsClient::setBranchFilters(QList<BranchFilter> branchFilters)
{
    Q_D(JenkinsClient);

    if (d->branchFilters != branchFilters) {
        d->branchFilters = branchFilters;
        emit branchFiltersChanged();
    }
}

void JenkinsClient::reload()
{
    d_func()->reload();
}

void JenkinsClient::reloadBuilds()
{
    d_func()->reloadBuilds();
}
