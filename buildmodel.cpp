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
#include "buildmodel.h"
#include "utils.h"

#include "Config/config.h"
#include "jenkinsclient.h"

#include <QTimer>
#include <QTemporaryFile>
#include <QJsonDocument>
#include <QJsonArray>

#include <iostream>
#include <iterator>

Q_LOGGING_CATEGORY(logBuildModel, "radiator.buildmodel")


BuildModel::BuildModel(QObject *parent)
    : QAbstractListModel(parent),
      m_buildingTimer(new QTimer(this)),
      m_statusTextTimer(new QTimer(this))
{
    auto cfg = Config::instance();
    connect(cfg, &Config::remotesChanged, this, &BuildModel::reset);
    connect(cfg, &Config::branchesChanged, this, &BuildModel::reset);
    connect(cfg, &Config::radiatorChanged, this, &BuildModel::reset);

    connect(m_buildingTimer, &QTimer::timeout, [&]() {
        for(int i = 0; i < m_jobs.size(); ++i) {
            if(m_jobs[i].building()) {
                emit dataChanged(index(i), index(i), { Duration, Progress, StatusText });
            }
        }
    });
    m_buildingTimer->start(1000);

    connect(m_statusTextTimer, &QTimer::timeout, [&]() {
        for(int i = 0; i < m_jobs.size(); ++i) {
            emit dataChanged(index(i), index(i), { StatusText });
        }
    });
    m_statusTextTimer->start(30000);

    QTimer::singleShot(100, this, &BuildModel::reset);
}

BuildModel::~BuildModel()
{
}

QHash<int, QByteArray> BuildModel::roleNames() const
{
    static QHash<int, QByteArray> roles({
        { Url,                   QByteArrayLiteral("url")                   },
        { Title,                 QByteArrayLiteral("title")                 },
        { Description,           QByteArrayLiteral("description")           },
        { Culprit,               QByteArrayLiteral("culprit")               },
        { IntegrationServerLink, QByteArrayLiteral("integrationServerLink") },
        { ConsoleLogLink,        QByteArrayLiteral("consoleLogLink")        },
        { CodeBrowserLink,       QByteArrayLiteral("codeBrowserLink")       },
        { Status,                QByteArrayLiteral("status")                },
        { Number,                QByteArrayLiteral("number")                },
        { StatusText,            QByteArrayLiteral("statusText")            },
        { Building,              QByteArrayLiteral("building")              },
        { Duration,              QByteArrayLiteral("duration")              },
        { EstimatedDuration,     QByteArrayLiteral("estimatedDuration")     },
        { StartedAt,             QByteArrayLiteral("startedAt")             },
        { Progress,              QByteArrayLiteral("progress")              },
        { HistoricalBuilds,      QByteArrayLiteral("historicalBuilds")      }
    });

    return roles;
}

int BuildModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_jobs.size();
}

QVariant BuildModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.parent().isValid()) {
        return QVariant();
    }

    if(index.row() < 0 || index.row() >= m_jobs.size()) {
        return QVariant();
    }

    const Job &job = m_jobs[index.row()];

    switch(role) {
    case Url:
        return job.url();
    case Title:
        return job.displayName();
    case Description:
        if(Config::instance()->radiator().blameEnabled() && !job.building()) {
            if(job.status() == Build::Status::Failure || job.status() == Build::Status::Unstable) {
                auto c = job.lastBuild().culPrits();
                if(!c.isEmpty() && !c.last().name().isEmpty()) {
                    return tr("%1 broke the build").arg(c.last().name());
                }
            }
        }

        return job.lastBuild().description();
    case Culprit:
        if(job.building() || (job.status() != Build::Status::Failure && job.status() != Build::Status::Unstable)) {
            auto c = job.lastBuild().culPrits();
            return c.isEmpty() ? QString() : c.first().name();
        }
        return QString();
    case IntegrationServerLink:
        return job.url();
    case ConsoleLogLink: {
        QUrl url = job.url();
        if(url.isValid()) {
            url.setPath(url.path() + QStringLiteral("/consoleFull"));
        }
        return url;
    }
    case CodeBrowserLink:
        return QUrl();
    case Status:
        return QVariant::fromValue<Build::Status>(job.status());
    case Number:
        return job.lastBuildNumber();
    case StatusText:
    {
        const Build last = job.lastBuild();

        if(last.building()) {
            qint64 started = last.startedAt().toSecsSinceEpoch();
            qint64 current = QDateTime::currentDateTime().toSecsSinceEpoch() - started;
            qint64 target = last.startedAt().addMSecs(last.estimatedDuration()).toSecsSinceEpoch() - started;
            if(target <= 0 || current <= 0) {
                return QString("...");
            }

            return QString("%1|%2").arg(Utils::human_time_diff(current)).arg(Utils::human_time_diff(target));
        }

        return Utils::time_elapsed_string(last.startedAt().addMSecs(last.duration()));
    }
    case Building:
        return job.building();
    case Duration:
        return job.lastBuild().duration();
    case EstimatedDuration:
        return job.lastBuild().estimatedDuration();
    case StartedAt:
        return job.lastBuild().startedAt();
    case Progress:
        {
            const Build last = job.lastBuild();
            if(last.building()) {
                qint64 started = last.startedAt().toSecsSinceEpoch();
                qint64 current = QDateTime::currentDateTime().toSecsSinceEpoch() - started;
                qint64 target = last.startedAt().addMSecs(last.estimatedDuration()).toSecsSinceEpoch() - started;

                if(target > 0) {
                    if(current > target) {
                        return 100.0;
                    }

                    qCDebug(logBuildModel)
                            << job.displayName()
                            << "at" << qBound(0.0, 100.0 / target * current, 100.0)
                            << "%";

                    return qBound(0.0, 100.0 / target * current, 100.0);
                }

                return 0.0;
            }

            return 100.0;
        }
    case HistoricalBuilds:
        if(!m_history.contains(job.id())) {
            const int max = Config::instance()->radiator().historicalBuildCount();
            QVariantList list;
            QList<Build> builds = job.builds().values();
            std::sort(builds.begin(), builds.end(), [](const Build &a, const Build &b) {
                return a.startedAt() < b.startedAt();
            });

            for(auto it = builds.crbegin(); it != builds.crend(); ++it) {
                if(*it == job.lastBuild()) {
                    continue;
                }

                QVariantMap map;
                map.insert("status", static_cast<int>(it->status()));
                map.insert("buildNumber", it->buildNumber());
                list.prepend(map);

                if(list.size() > max) {
                    break;
                }
            }

            m_history.insert(job.id(), list);
        }

        return m_history.value(job.id());
    }

    return QVariant();
}

void BuildModel::dump()
{
    QTemporaryFile file;
    file.setAutoRemove(false);
    if(file.open()) {
        QJsonArray arr;
        foreach(const Job &j, m_jobs) {
            arr.append(j.toJson());
        }

        QJsonDocument doc(arr);

        file.write(doc.toJson());
        file.close();

        std::cout << "dump << " << file.size() << " bytes to: "
                  << file.fileName().toStdString() << std::endl;
    }
}

void BuildModel::jobAdded(const Job &job)
{
    int index = 0;
    if(!m_jobs.isEmpty()) {
        auto it = std::lower_bound(m_jobs.cbegin(), m_jobs.cend(), job,
                                   [](const Job &a, const Job &b) {
            if(a.priority() == b.priority()) {
                return a.name() > b.name();
            }
            return a.priority() > b.priority();
        });

        index = static_cast<int>(std::distance(m_jobs.cbegin(), it));
    }

    beginInsertRows(QModelIndex(), index, index);
    m_jobs.insert(index, job);
    endInsertRows();
}

void BuildModel::jobRemoved(const Job &job)
{
    m_history.remove(job.id());
    for(int i = 0; i < m_jobs.size(); ++i) {
        if(m_jobs[i].id() == job.id()) {
            beginRemoveRows(QModelIndex(), i + 1, i + 1);
            m_jobs.removeAt(i);
            endRemoveRows();
            break;
        }
    }
}

void BuildModel::jobChanged(const Job &job)
{
    m_history.remove(job.id());
    for(int i = 0; i < m_jobs.size(); ++i) {
        if(m_jobs[i].id() == job.id()) {
            m_jobs[i] = job;
            emit dataChanged(index(i), index(i));
            break;
        }
    }
}

void BuildModel::buildChanged(const Job &job, const Build &build)
{
    Q_UNUSED(build);
    jobChanged(job); // TODO
}

void BuildModel::reset()
{
    beginResetModel();
    for(auto c : m_clients) {
        c->disconnect(this);
        c->deleteLater();
    }
    m_clients.clear();
    m_jobs.clear();
    m_history.clear();
    endResetModel();

    auto cfg = Config::instance();
    for(auto remote : cfg->remotes()) {
        if(!remote.enabled()) {
            continue;
        }

        JenkinsClient *jc = new JenkinsClient(this);
        m_clients.append(jc);

        QObject::connect(jc, &JenkinsClient::jobAdded, this, &BuildModel::jobAdded);
        QObject::connect(jc, &JenkinsClient::jobRemoved, this, &BuildModel::jobRemoved);
        QObject::connect(jc, &JenkinsClient::jobChanged, this, &BuildModel::jobChanged);
        QObject::connect(jc, &JenkinsClient::buildChanged, this, &BuildModel::buildChanged);

        jc->setName(remote.name());
        jc->setUrl(remote.url());
        jc->setBranchFilters(cfg->branches());

        if(!remote.user().isEmpty()) {
            jc->setUserName(remote.user().username());
            jc->setPassword(remote.user().password());
        }

        jc->setJobRefreshInterval(cfg->radiator().jobPollingInterval());
        jc->setBuildRefreshInterval(cfg->radiator().buildPollingInterval());

        jc->reload();
    }
}

