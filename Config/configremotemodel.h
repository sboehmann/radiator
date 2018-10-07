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
#ifndef CONFIG_REMOTE_MODEL_H
#define CONFIG_REMOTE_MODEL_H

#include "configremote.h"
#include <QAbstractListModel>

class Config;

class ConfigRemoteModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Config* config READ config WRITE setConfig NOTIFY configChanged)

    enum Roles {
        Enabled = Qt::UserRole + 1,
        Type,
        Name,
        Url,
        User
    };

public:
    explicit ConfigRemoteModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool removeRows(int row, int count, const QModelIndex &parent) override;

    Config* config() const;
    void setConfig(Config* config);

signals:
    void configChanged();

private:
    ConfigRemote m_emptyRemote;
    Config* m_config = nullptr;
};

#endif
