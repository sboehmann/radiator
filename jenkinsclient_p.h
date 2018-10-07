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
#ifndef JENKINS_CLIENT_PRIVATE_H
#define JENKINS_CLIENT_PRIVATE_H

#include "jenkinsclient.h"
#include "Entity/job.h"

#include <QtCore/QUrl>
#include <QtCore/QTimer>
#include <QtCore/QUrlQuery>
#include <QtCore/QRegularExpression>
#include <QtCore/QRegularExpressionMatch>
#include <QtCore/QLoggingCategory>

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonValue>
#include <QtCore/QJsonParseError>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>


Q_DECLARE_LOGGING_CATEGORY(logJenkinsClient)

class JenkinsClientPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(JenkinsClient)

public:
    explicit JenkinsClientPrivate(JenkinsClient* q);
    virtual ~JenkinsClientPrivate();

    void reload();
    void reloadBuilds();

    QNetworkReply* makeRequest(QUrl requestUrl, const QUrlQuery &query = QUrlQuery());

public:
    QTimer jobRefreshTimer;
    int jobRefreshInterval = 0;
    QTimer buildRefreshTimer;
    int buildRefreshInterval = 0;
    QString name;
    QUrl url;
    QString userName;
    QString password;
    QHash<QUrl, Job> jobs;
    QNetworkAccessManager manager;
    QList<BranchFilter> branchFilters;

protected slots:
    void jobListRequestFinished(QNetworkReply *reply);
    void jobRequestFinished(Job job, QNetworkReply *reply);
    void buildRequestFinished(QUrl jobUrl, Build build, QNetworkReply *reply);

    void authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible);
    void preSharedKeyAuthenticationRequired(QNetworkReply *reply, QSslPreSharedKeyAuthenticator *authenticator);
    void proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator);

private:
    void parseStatus(Job &job, const QJsonValue &value);

signals:
    void jobChanged(Job job);
    void jobAdded(Job job);
    void jobRemoved(Job job);

    void buildChanged(Job job, Build build);
//    void buildAdded(Job job, Build build);
//    void buildRemoved(Job job, Build build);

protected:
    JenkinsClient* const q_ptr;
};

#endif
