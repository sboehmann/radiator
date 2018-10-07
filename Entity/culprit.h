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
#ifndef ENTITY_CULPRIT_H
#define ENTITY_CULPRIT_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QJsonObject>
#include <QtCore/QSharedDataPointer>

struct CulPritData;

class CulPrit
{
    Q_GADGET
    Q_PROPERTY(QString id READ id WRITE setId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(QUrl url READ url WRITE setUrl)
public:

    CulPrit();
    explicit CulPrit(const QJsonValue &value);
    CulPrit(const CulPrit &rhs);
    CulPrit& operator=(const CulPrit &rhs);
    ~CulPrit();

    Q_INVOKABLE bool isEmpty() const;

    QJsonObject toJson() const;

    QString id() const;
    void setId(QString id);

    QString name() const;
    void setName(QString name);

    QUrl url() const;
    void setUrl(QUrl url);

    QString description() const;
    void setDescription(QString description);

private:
    QSharedDataPointer<CulPritData> d;
};
Q_DECLARE_METATYPE(CulPrit)


bool operator==(const CulPrit &lhs, const CulPrit &rhs);

inline bool operator!=(const CulPrit& lhs, const CulPrit& rhs)
{
    return !(lhs == rhs);
}

uint qHash(const CulPrit& key, uint seed = 6917);

#endif
