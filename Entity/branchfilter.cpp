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
#include "branchfilter.h"

#include <QMetaEnum>
#include <QJsonObject>
#include <QJsonArray>

struct BranchFilterData : public QSharedData
{
    QStringList filters;
    QStringList remotes;
    BranchFilter::Visibility visibility = BranchFilter::Visibility::Always;
    int priority = 0;
};

BranchFilter::BranchFilter()
  : d(new BranchFilterData)
{
}

BranchFilter::BranchFilter(const QJsonValue &value)
  : d(new BranchFilterData)
{
    if(value.isString()) {
        d->filters.append(value.toString());
        return;
    }

    QJsonObject obj = value.toObject();

    if(obj.value(QLatin1String("filter")).isString()) {
        d->filters.append(obj.value(QLatin1String("filter")).toString());
    }
    else if(obj.value(QLatin1String("filter")).isArray()) {
        QJsonArray filters = obj.value(QLatin1String("filter")).toArray();
        for(auto it = filters.constBegin(); it != filters.constEnd(); ++it) {
            d->filters.append((*it).toString());
        }
    }

    if(obj.value(QLatin1String("remote")).isString()) {
        d->remotes.append(obj.value(QLatin1String("remote")).toString());
    }
    else if(obj.value(QLatin1String("remote")).isArray()) {
        QJsonArray filters = obj.value(QLatin1String("remote")).toArray();
        for(auto it = filters.constBegin(); it != filters.constEnd(); ++it) {
            d->remotes.append((*it).toString());
        }
    }

    if(obj.contains(QLatin1String("visibility"))) {
        QString v = obj.value(QLatin1String("visibility")).toString().trimmed().toLower();
        QMetaEnum e = QMetaEnum::fromType<Visibility>();
        for(int i = 0; i < e.keyCount(); ++i) {
            if(QString::fromLatin1(e.key(i)).compare(v, Qt::CaseInsensitive) == 0) {
                d->visibility = static_cast<Visibility>(e.value(i));
                break;
            }
        }
    }

    d->priority = obj.value(QLatin1String("priority")).toInt();
}

BranchFilter::BranchFilter(const BranchFilter &other)
  : d(other.d)
{
}

BranchFilter &BranchFilter::operator=(const BranchFilter &rhs)
{
    if (Q_LIKELY(this != &rhs)) {
        d.operator=(rhs.d);
    }
    return *this;
}

BranchFilter::~BranchFilter()
{
}

bool BranchFilter::isEmpty() const
{
    return d->filters.isEmpty();
}


QStringList BranchFilter::filters() const
{
    return d->filters;
}

void BranchFilter::setFilters(QStringList filters)
{
    d->filters = filters;
}

QStringList BranchFilter::remotes() const
{
    return d->remotes;
}

void BranchFilter::setRemotes(QStringList remotes)
{
    d->remotes = remotes;
}

BranchFilter::Visibility BranchFilter::visibility() const
{
    return d->visibility;
}

void BranchFilter::setVisibility(Visibility visibility)
{
    d->visibility = visibility;
}

int BranchFilter::priority() const
{
    return d->priority;
}

void BranchFilter::setPriority(int priority)
{
    d->priority = priority;
}

QJsonObject BranchFilter::toJson() const
{
    QJsonObject obj;

    if(!d->filters.isEmpty()) {
        if(d->filters.size() == 1) {
            obj.insert(QStringLiteral("filter"), d->filters.first());
        }
        else {
            obj.insert(QStringLiteral("filter"), QJsonArray::fromStringList(d->filters));
        }
    }
    if(!d->remotes.isEmpty()) {
        if(d->remotes.size() == 1) {
            obj.insert(QStringLiteral("remote"), d->remotes.first());
        }
        else {
            obj.insert(QStringLiteral("remote"), QJsonArray::fromStringList(d->remotes));
        }
    }

    if(d->visibility != Visibility::Always) {
        QMetaEnum e = QMetaEnum::fromType<Visibility>();
        obj.insert(QStringLiteral("visibility"), e.valueToKey(static_cast<int>(d->visibility)));
    }

    if(d->priority != 0) {
        obj.insert(QStringLiteral("priority"), d->priority);
    }

    return obj;
}

bool operator==(const BranchFilter &lhs, const BranchFilter &rhs)
{
    return lhs.filters() == rhs.filters()
        && lhs.remotes() == rhs.remotes()
        && lhs.visibility() == rhs.visibility()
        && lhs.priority() == rhs.priority();
}
