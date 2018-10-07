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
#include "config.h"

#include <QStandardPaths>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QMetaEnum>
#include <QFileInfo>
#include <QDebug>
#include <QDir>

#include <iostream>

Q_GLOBAL_STATIC(Config, config)

Config::Config(QObject *parent)
    : QObject(parent)
{
    connect(&m_watcher, &QFileSystemWatcher::fileChanged, [this](const QString &fileName) {
        if(Q_LIKELY(m_fileName == fileName)) {
            readFile();
        }
    });

    if(config.exists() && config != this) {
        readFile();
    }
}

Config* Config::instance()
{
    return config;
}

QString Config::fileName() const
{
    if(!m_fileName.isEmpty()) {
        return m_fileName;
    }

    if(config != this) {
        return config->fileName();
    }

    if(QFile::exists(QDir::currentPath() + QStringLiteral("radiator.conf"))) {
        return QDir::currentPath() + QStringLiteral("radiator.conf");
    }

    QString fn = QStandardPaths::locate(QStandardPaths::AppConfigLocation, QStringLiteral("radiator.conf"));
    if(fn.isEmpty()) {
        fn = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + QStringLiteral("/radiator.conf");
        if(QFile::copy(":/radiator.conf", fn)) {
            QFile::setPermissions(fn, QFileDevice::ReadUser | QFileDevice::WriteUser);
        }
    }

    return fn;
}

void Config::setFileName(const QString &fileName)
{
    if(m_fileName != fileName) {
        if(m_watcher.files().contains(m_fileName)) {
            m_watcher.removePath(m_fileName);
        }

        m_fileName = fileName;
        m_watcher.addPath(m_fileName);

        emit fileNameChanged();
    }
}

bool Config::readFile(const QString &fn)
{
    QString fileName = fn.isEmpty() ? Config::fileName() : fn;
    if(fileName.isEmpty()) {
        qDebug() << "config file name is empty.";
        return false;
    }

    QFile file(fileName);
    if(!file.exists()) {
        qDebug().nospace().noquote() << "config file '" << fileName << "' does not exists.";
        return false;
    }

    if(!file.open(QIODevice::ReadOnly)) {
        qDebug().nospace().noquote() << "can not open config file '" << fileName << "' for reading.";
        return false;
    }

    qDebug().nospace().noquote() << "reading config file '" << fileName << "'.";
    QByteArray data;
    do {
        QByteArray line = file.readLine();

        if(line.isEmpty()) {
            break;
        }

        if(line.trimmed().startsWith('#') || line.trimmed().startsWith("//")) {
            continue;
        }

        data.append(line);
    } while(true);

    file.close();

    if(data.trimmed().isEmpty()) {
        data.append(QByteArrayLiteral("{}"));
    }

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if(err.error != QJsonParseError::NoError) {
        qWarning() << err.errorString() << err.offset;
        return false;
    }

    if(!doc.isObject()) {
        qWarning() << "Expect json object in the root of the config file.";
        return false;
    }

    QJsonObject root = doc.object();
    readRadiator(root.value(QLatin1String("radiator")));
    readProxy(root.value(QLatin1String("proxy")));
    readRemotes(root.value(QLatin1String("remotes")));
    readBranches(root.value(QLatin1String("branches")));

    setFileName(fileName);

    return true;
}

bool Config::writeFile(const QString &fn)
{
    QString fileName = fn.isEmpty() ? Config::fileName() : fn;
    if(fileName.isEmpty()) {
        qDebug() << "Empty config file name";
        return false;
    }

    QFileInfo info(fileName);
    if(!info.absoluteDir().exists()) {
        if(!info.absoluteDir().mkpath(info.absolutePath())) {
            qDebug() << "Failed to create config directory" << info.absolutePath();
        }
    }

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Error writing config file " << fileName << ":" << file.errorString();
        return false;
    }

    auto res = file.write(QJsonDocument(toJson()).toJson());
    file.close();

    if(res > 0) {
        setFileName(fileName);

        if(config != this && this->fileName() == config->fileName()) {
            config->readFile();
        }

        return true;
    }

    return false;
}

ConfigProxy Config::proxy() const
{
    return m_proxy;
}

void Config::setProxy(const ConfigProxy &proxy)
{
    if(m_proxy != proxy) {
        m_proxy = proxy;
        emit proxyChanged();
    }
}

QList<ConfigRemote> Config::remotes() const
{
    return m_remotes;
}

void Config::setRemotes(const QList<ConfigRemote> &remotes)
{
    if(m_remotes != remotes) {
        m_remotes = remotes;
        emit remotesChanged();
    }
}

QList<BranchFilter> Config::branches() const
{
    return m_branches;
}

void Config::setBranches(const QList<BranchFilter> &branches)
{
    if(m_branches != branches) {
        m_branches = branches;
        emit branchesChanged();
    }
}

ConfigRadiator Config::radiator() const
{
    return m_radiator;
}

void Config::setRadiator(const ConfigRadiator &radiator)
{
    if (m_radiator != radiator) {
        m_radiator = radiator;
        emit radiatorChanged();
    }
}

void Config::readRadiator(const QJsonValue &value)
{
    m_radiator = ConfigRadiator(value);
    emit radiatorChanged();
}

void Config::readProxy(const QJsonValue &value)
{
    m_proxy = ConfigProxy(value);
    emit proxyChanged();
}

void Config::readRemotes(const QJsonValue &value)
{
    m_remotes.clear();
    const QJsonObject obj = value.toObject();
    for(auto it = obj.constBegin(); it != obj.constEnd(); ++it) {
        ConfigRemote r(it.value());
        r.setName(it.key());

        if(!r.isEmpty()) {
            m_remotes.append(r);
        }
    }

    emit remotesChanged();
}

void Config::readBranches(const QJsonValue &value)
{
    m_branches.clear();

    if(value.isString()) {
        BranchFilter b(value);
        if(!b.isEmpty()) {
            m_branches.append(b);
        }
    }
    else if(value.isArray()) {
        const QJsonArray arr = value.toArray();
        for(auto it = arr.constBegin(); it != arr.constEnd(); ++it) {
            BranchFilter b(*it);
            if(!b.isEmpty()) {
                m_branches.append(b);
            }
        }
    }
    else if(!(value.isNull() || value.isUndefined())) {
        qDebug() << "Illegal type found for 'filter' while reading branch configuration.";
    }

    emit branchesChanged();
}

QJsonObject Config::toJson() const
{
    QJsonObject obj;

    if(!m_radiator.isEmpty()) {
        auto o = m_radiator.toJson();
        if(!o.isEmpty()) {
            obj.insert(QStringLiteral("radiator"), o);
        }
    }

    if(!m_proxy.isEmpty()) {
        auto o = m_proxy.toJson();
        if(!o.isEmpty()) {
            obj.insert(QStringLiteral("proxy"), o);
        }
    }

    QJsonObject remotes;
    for(auto it : m_remotes) {
        if(!it.isEmpty()) {
            auto o = it.toJson();
            if(!o.isEmpty()) {
                remotes.insert(it.name(), o);
            }
        }
    }
    if(!remotes.isEmpty()) {
        obj.insert(QStringLiteral("remotes"), remotes);
    }

    QJsonArray branches;
    for(auto it : m_branches) {
        if(!it.isEmpty()) {
            auto o = it.toJson();
            if(!o.isEmpty()) {
                branches.append(o);
            }
        }
    }
    if(!branches.isEmpty()) {
        obj.insert(QStringLiteral("branches"), branches);
    }

    return obj;
}

void Config::save()
{
    writeFile();
}

void Config::reload()
{
    readFile();
}

