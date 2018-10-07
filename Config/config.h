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
#ifndef CONFIG_H
#define CONFIG_H

#include "Config/configuser.h"
#include "Config/configradiator.h"
#include "Config/configproxy.h"
#include "Config/configremote.h"

#include "Entity/branchfilter.h"

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QHash>
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QJsonObject>

class Config : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ConfigProxy proxy READ proxy WRITE setProxy NOTIFY proxyChanged)
    Q_PROPERTY(ConfigRadiator radiator READ radiator WRITE setRadiator NOTIFY radiatorChanged)
    Q_PROPERTY(QList<ConfigRemote> remotes READ remotes WRITE setRemotes NOTIFY remotesChanged)
    Q_PROPERTY(QList<BranchFilter> branches READ branches WRITE setBranches NOTIFY branchesChanged)

public:
    explicit Config(QObject *parent = nullptr);

    static Config* instance();

    QString fileName() const;
    void setFileName(const QString &fileName);

    bool readFile(const QString &fileName = QString());
    bool writeFile(const QString &fileName = QString());

    ConfigProxy proxy() const;
    void setProxy(const ConfigProxy &proxy);

    QList<ConfigRemote> remotes() const;
    void setRemotes(const QList<ConfigRemote> &remotes);

    QList<BranchFilter> branches() const;
    void setBranches(const QList<BranchFilter> &branches);

    ConfigRadiator radiator() const;
    void setRadiator(const ConfigRadiator &radiator);

    QJsonObject toJson() const;

public slots:
    void save();
    void reload();

signals:
    void fileNameChanged();
    void remotesChanged();
    void branchesChanged();
    void proxyChanged();
    void radiatorChanged();

private:
    void readRadiator(const QJsonValue &value);
    void readProxy(const QJsonValue &value);
    void readRemotes(const QJsonValue &value);
    void readBranches(const QJsonValue &value);

private:
    QFileSystemWatcher m_watcher;
    ConfigRadiator m_radiator;
    ConfigProxy m_proxy;
    QList<ConfigRemote> m_remotes;
    QList<BranchFilter> m_branches;
    QString m_fileName;
};

#endif
