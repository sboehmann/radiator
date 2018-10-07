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
#include "branchfiltermodel.h"
#include "Config/config.h"

BranchFilterModel::BranchFilterModel(QObject *parent)
  : QAbstractListModel(parent)
{
    m_visibilities = {
        { BranchFilter::Visibility::Always,    tr("Always")    },
        { BranchFilter::Visibility::Active,    tr("Active")    },
        { BranchFilter::Visibility::Buildable, tr("Buildable") },
        { BranchFilter::Visibility::Unstable,  tr("Unstable")  },
        { BranchFilter::Visibility::Failed,    tr("Failed")    },
        { BranchFilter::Visibility::Never,     tr("Never")     }
    };
}

int BranchFilterModel::rowCount(const QModelIndex &parent) const
{
    if (!m_config || parent.isValid()) {
        return 0;
    }

    return m_config->branches().size() + 1;
}

QHash<int, QByteArray> BranchFilterModel::roleNames() const
{
    static QHash<int, QByteArray> roles({
        { Filters,      QByteArrayLiteral("filters")      },
        { Remotes,      QByteArrayLiteral("remotes")      },
        { Visibilities, QByteArrayLiteral("visibilities") },
        { Visibility,   QByteArrayLiteral("visibility")   },
        { Priority,     QByteArrayLiteral("priority")     }
    });

    return roles;
}

QVariant BranchFilterModel::data(const QModelIndex &index, int role) const
{
    if (Q_UNLIKELY(!m_config || !index.isValid())) {
        return QVariant();
    }

    auto list = m_config->branches();

    if(Q_UNLIKELY(index.row() < 0 || index.row() > list.size())) {
        return QVariant();
    }

    const BranchFilter branch = index.row() < list.size() ? list.at(index.row()) : m_emptyBranch;

    switch(role) {
    case Filters:
        return branch.filters();
    case Remotes:
        return branch.remotes();
    case Visibilities:
        return QStringList(m_visibilities.values());
    case Visibility:
        return m_visibilities.value(branch.visibility());
    case Priority:
        return branch.priority();
    }

    return QVariant();
}

bool BranchFilterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (Q_UNLIKELY(!m_config || !index.isValid())) {
        return false;
    }

    auto list = m_config->branches();

    if(Q_UNLIKELY(index.row() < 0 || index.row() > list.size())) {
        return false;
    }

    if (Q_UNLIKELY(data(index, role) == value)) {
        return false;
    }

    BranchFilter branch = index.row() == list.size() ? m_emptyBranch : list[index.row()];
    switch(role) {
    case Filters:
        branch.setFilters(value.toStringList());
        break;
    case Remotes:
        branch.setRemotes(value.toStringList());
        break;
    case Visibility:
        branch.setVisibility(m_visibilities.key(value.toString()));
        break;
    case Priority:
        branch.setPriority(value.toInt());
        break;
    default:
        return false;
    }

    if(index.row() == list.size()) {
        m_emptyBranch = branch;

        if(!m_emptyBranch.isEmpty()) {
            list.append(m_emptyBranch);
            beginInsertRows(QModelIndex(), list.size(), list.size());
            m_config->setBranches(list);
            m_emptyBranch = BranchFilter();
            endInsertRows();
        }
    }
    else {
        list.replace(index.row(), branch);
        m_config->setBranches(list);
        emit dataChanged(index, index, {role});
    }

    return true;
}

bool BranchFilterModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(Q_UNLIKELY(!m_config || parent.isValid())) {
        return false;
    }

    auto list = m_config->branches();

    if(Q_UNLIKELY(row < 0 || count < 1 || (row + count - 1) >= list.size())) {
        return false;
    }

    beginRemoveRows(parent, row, row + count - 1);
    for(int i = row; i < (row + count); ++i) {
        list.removeAt(row);
    }
    m_config->setBranches(list);
    endRemoveRows();

    return true;
}

Config *BranchFilterModel::config() const
{
    return m_config;
}

void BranchFilterModel::setConfig(Config *config)
{
    if (m_config != config) {
        beginResetModel();
        m_config = config;
        endResetModel();

        emit configChanged();
    }
}
