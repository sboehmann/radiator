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
#ifndef ENTITY_BRANCH_FILTER_H
#define ENTITY_BRANCH_FILTER_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtCore/QJsonObject>
#include <QtCore/QSharedDataPointer>

struct BranchFilterData;

class BranchFilter
{
    Q_GADGET
    Q_PROPERTY(QStringList filters READ filters WRITE setFilters)
    Q_PROPERTY(QStringList remotes READ remotes WRITE setRemotes)
    Q_PROPERTY(Visibility visibility READ visibility WRITE setVisibility)
    Q_PROPERTY(int priority READ priority WRITE setPriority)

public:
    enum class Visibility {
        Always,
        Active,
        Buildable,
        Unstable,
        Failed,
        Never
    };
    Q_ENUM(Visibility)

    BranchFilter();
    explicit BranchFilter(const QJsonValue &value);
    BranchFilter(const BranchFilter &rhs);
    BranchFilter& operator=(const BranchFilter &rhs);
    ~BranchFilter();

    Q_INVOKABLE bool isEmpty() const;

    QJsonObject toJson() const;

    QStringList filters() const;
    void setFilters(QStringList filters);

    QStringList remotes() const;
    void setRemotes(QStringList remotes);

    Visibility visibility() const;
    void setVisibility(Visibility visibility);

    int priority() const;
    void setPriority(int priority);

private:
    QSharedDataPointer<BranchFilterData> d;
};
Q_DECLARE_METATYPE(BranchFilter)


bool operator==(const BranchFilter &lhs, const BranchFilter &rhs);

inline bool operator!=(const BranchFilter& lhs, const BranchFilter& rhs)
{
    return !(lhs == rhs);
}

#endif
