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
import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import Radiator 1.0
import Radiator.Config 1.0

import ".."

SettingsPage {
    id: root

    onConfigChanged: branchModel.config = config

    BranchFilterModel {
        id: branchModel
    }

    ModelAccessor {
        id: branchModelAccessor
        sourceModel: branchModel
    }

    ColumnLayout {
        anchors.fill: parent

        Label {
            text: qsTr("Branches")
            font.bold: true
            font.capitalization: Font.AllUppercase
        }

        ListView {
            id: listView
            clip: true
            Layout.fillHeight: true
            Layout.fillWidth: true
            model: branchModel

            delegate: BranchDelegate {
                x: 5
                width: ListView.view.width
                branchModel: model
                showTitle: index === 0
                enableRemoveButton: index < listView.count - 1

                onRemoveClicked: {
                    if(!branchModelAccessor.removeRow(index)) {
                        console.log("failed to remove row");
                    }
                }
            }
        }
    }
}
