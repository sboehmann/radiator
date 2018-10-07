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
#ifndef BUILDMODEL_H
#define BUILDMODEL_H

#include <QAbstractListModel>
#include <QNetworkAccessManager>
#include <QLoggingCategory>
#include <QPointer>

#include "Entity/job.h"

class QTimer;
class JenkinsClient;

Q_DECLARE_LOGGING_CATEGORY(logBuildModel)

class BuildModel : public QAbstractListModel
{
    Q_OBJECT

    enum Roles {
        Url = Qt::UserRole + 1,
        Title,
        Description,
        Culprit,
        IntegrationServerLink,
        ConsoleLogLink,
        CodeBrowserLink,
        Status,
        Number,
        StatusText,
        Building,
        Duration,
        EstimatedDuration,
        StartedAt,
        Progress,
        HistoricalBuilds
    };

public:
    explicit BuildModel(QObject *parent = nullptr);
    ~BuildModel();

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void dump();

private slots:
    void jobAdded(const Job &job);
    void jobRemoved(const Job &job);
    void jobChanged(const Job &job);
    void buildChanged(const Job &job, const Build &build);
    void reset();

private:
    QTimer *m_buildingTimer = nullptr;
    QTimer *m_statusTextTimer = nullptr;
    QList<JenkinsClient*> m_clients;
    QList<Job> m_jobs;
    mutable QHash<QByteArray, QVariantList> m_history;
};

#endif
