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

import ".."

Item {
    id: root

    implicitWidth: childrenRect.width
    implicitHeight: childrenRect.height

    property bool showTitle : true
    property bool enableRemoveButton: true
    property var branchModel

    signal removeClicked

    ColumnLayout {
        width: parent.width

        RowLayout {
            Layout.fillWidth: true

            TitleBox {
                title: qsTr("Filter")
                showTitle: root.showTitle
                Layout.fillWidth: true

                TextField {
                    text: branchModel.filters.join(';')
                    onTextChanged: branchModel.filters = text.split(';')
                    anchors.fill: parent
                    placeholderText: enableRemoveButton ? ".*" : qsTr("Filter")
                }
            }

            TitleBox {
                title: qsTr("Priority")
                showTitle: root.showTitle

                SpinBox {
                    anchors.fill: parent
                    enabled: enableRemoveButton
                    from: 0
                    to: 99
                    value: branchModel.priority
                    onValueChanged: branchModel.priority = value
                }
            }

            TitleBox {
                title: qsTr("Remote")
                showTitle: root.showTitle

                TextField {
                    enabled: enableRemoveButton
                    text: branchModel.remotes.join(';')
                    onTextChanged: branchModel.remotes = text.split(';')
                    anchors.fill: parent
                    placeholderText: qsTr("All")
                }
            }

            TitleBox {
                title: qsTr("Visibility")
                showTitle: root.showTitle

                ComboBox {
                    id: visibilityComboBox
                    enabled: enableRemoveButton
                    anchors.fill: parent
                    model: branchModel.visibilities
                    displayText: branchModel.visibility
                    onCurrentTextChanged: branchModel.visibility = currentText
                }
            }

            ToolButton {
                id: removeButton
                enabled: enableRemoveButton
                text: "\uf014"
                font.family: Fonts.awesome
                Layout.alignment: Qt.AlignBottom
                onClicked: root.removeClicked()
            }
        }
    }
}
