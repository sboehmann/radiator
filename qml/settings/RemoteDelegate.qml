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
    property var remoteModel

    signal removeClicked

    ColumnLayout {
        width: parent.width

        RowLayout {
            Layout.fillWidth: true

            CheckBox {
                id: checkbox
                Layout.alignment: Qt.AlignBottom

                checked: remoteModel.enabled
                onCheckedChanged: remoteModel.enabled = checked
            }

            TitleBox {
                title: qsTr("Name")
                showTitle: root.showTitle
                Layout.fillWidth: true

                TextField {
                    anchors.fill: parent
                    font.italic: !checkbox.checked

                    text: remoteModel.name
                    onTextChanged: remoteModel.name = text
                }
            }

            TitleBox {
                title: qsTr("Host")
                showTitle: root.showTitle
                Layout.fillWidth: true

                TextField {
                    anchors.fill: parent
                    font.italic: !checkbox.checked

                    text: remoteModel.url
                    onTextChanged: remoteModel.url = text
                }
            }

            TitleBox {
                title: qsTr("User")
                showTitle: root.showTitle
                Layout.fillWidth: true

                TextField {
                    anchors.fill: parent
                    enabled: enableRemoveButton
                    font.italic: !checkbox.checked

                    text: remoteModel.user.username
                    onTextChanged: remoteModel.user.username = text
                }
            }

            TitleBox {
                title: qsTr("Password")
                showTitle: root.showTitle
                Layout.fillWidth: true

                TextField {
                    anchors.fill: parent
                    enabled: enableRemoveButton
                    echoMode: TextInput.Password
                    font.italic: !checkbox.checked

                    text: remoteModel.user.password
                    onTextChanged: remoteModel.user.password = text
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
