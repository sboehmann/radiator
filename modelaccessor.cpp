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
#include "modelaccessor.h"

ModelAccessor::ModelAccessor(QObject *parent)
  : QObject(parent)
{
}

int ModelAccessor::rowCount() const
{
    if(!m_sourceModel) {
        return 0;
    }

    return m_sourceModel->rowCount();
}

bool ModelAccessor::removeRow(int row)
{
    if(!m_sourceModel) {
        return false;
    }

    return m_sourceModel->removeRow(row);
}

QAbstractItemModel *ModelAccessor::sourceModel() const
{
    return m_sourceModel;
}

void ModelAccessor::setSourceModel(QAbstractItemModel *sourceModel)
{
    if(m_sourceModel == sourceModel) {
        return;
    }

    if(m_sourceModel) {
        m_sourceModel->disconnect(this);
    }

    m_sourceModel = sourceModel;
    if(m_sourceModel) {
        connect(m_sourceModel, &QAbstractItemModel::rowsInserted, this, &ModelAccessor::rowCountChanged);
        connect(m_sourceModel, &QAbstractItemModel::rowsRemoved, this, &ModelAccessor::rowCountChanged);
        connect(m_sourceModel, &QObject::destroyed, this, &ModelAccessor::rowCountChanged);
    }

    emit sourceModelChanged();
}
