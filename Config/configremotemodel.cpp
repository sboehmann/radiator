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
#include "configremotemodel.h"

#include "config.h"

#include <QSet>
#include <QDebug>

ConfigRemoteModel::ConfigRemoteModel(QObject *parent)
  : QAbstractListModel(parent)
{
}

int ConfigRemoteModel::rowCount(const QModelIndex &parent) const
{
    if (Q_UNLIKELY(!m_config || parent.isValid())) {
        return 0;
    }

    return m_config->remotes().size() + 1;
}

QHash<int, QByteArray> ConfigRemoteModel::roleNames() const
{
    static QHash<int, QByteArray> roles({
        { Enabled, QByteArrayLiteral("enabled") },
        { Type,    QByteArrayLiteral("type")    },
        { Name,    QByteArrayLiteral("name")    },
        { Url,     QByteArrayLiteral("url")     },
        { User,    QByteArrayLiteral("user")    }
    });

    return roles;
}

QVariant ConfigRemoteModel::data(const QModelIndex &index, int role) const
{
    if (Q_UNLIKELY(!index.isValid())) {
        return QVariant();
    }

    if(Q_UNLIKELY(index.row() < 0 || index.row() > m_config->remotes().size())) {
        return QVariant();
    }

    const ConfigRemote remote = index.row() < m_config->remotes().size() ? m_config->remotes().at(index.row()) : m_emptyRemote;

    switch(role) {
    case Enabled:
        return remote.enabled();
    case Type:
        return remote.type();
    case Name:
        return remote.name();
    case Url:
        return remote.url();
    case User:
        return QVariant::fromValue<ConfigUser>(remote.user());
    }

    return QVariant();
}

bool ConfigRemoteModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (Q_UNLIKELY(!m_config || !index.isValid())) {
        return false;
    }

    auto list = m_config->remotes();

    if(Q_UNLIKELY(index.row() < 0 || index.row() > list.size())) {
        return false;
    }

    if (Q_UNLIKELY(data(index, role) == value)) {
        return false;
    }

    ConfigRemote remote = index.row() == list.size() ? m_emptyRemote : list[index.row()];
    switch(role) {
    case Enabled:
        remote.setEnabled(value.toBool());
        break;
    case Type:
        remote.setType(static_cast<ConfigRemote::Type>(value.toInt()));
        break;
    case Name:
        remote.setName(value.toString());
        break;
    case Url:
        remote.setUrl(value.toUrl());
        break;
    case User:
        remote.setUser(value.value<ConfigUser>());
        break;
    default:
        return false;
    }

    bool b= m_config->blockSignals(true);
    if(index.row() == list.size()) {
        m_emptyRemote = remote;

        if(!m_emptyRemote.isEmpty()) {
            list.append(m_emptyRemote);
            beginInsertRows(QModelIndex(), list.size(), list.size());
            m_config->setRemotes(list);
            m_emptyRemote = ConfigRemote();
            endInsertRows();
        }
    }
    else {
        list.replace(index.row(), remote);
        m_config->setRemotes(list);
        emit dataChanged(index, index, {role});
    }
    m_config->blockSignals(b);

    return true;
}

bool ConfigRemoteModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(Q_UNLIKELY(!m_config || parent.isValid())) {
        return false;
    }

    auto list = m_config->remotes();

    if(Q_UNLIKELY(row < 0 || count < 1 || (row + count - 1) >= list.size())) {
        return false;
    }

    beginRemoveRows(parent, row, row + count - 1);
    for(int i = row; i < (row + count); ++i) {
        list.removeAt(row);
    }
    m_config->setRemotes(list);
    endRemoveRows();

    return true;
}

Config *ConfigRemoteModel::config() const
{
    return m_config;
}

void ConfigRemoteModel::setConfig(Config *config)
{
    if (m_config != config) {
        beginResetModel();
        m_config = config;
        endResetModel();

        emit configChanged();
    }
}

