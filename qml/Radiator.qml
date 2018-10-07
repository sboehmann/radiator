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
import QtQuick.Window 2.3
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import Radiator 1.0

import "settings"

ApplicationWindow {
    id: root
    title: qsTr("Radiator")
    visible: true
    width: 1024
    height: 768
    visibility: Window.Windowed

    Component.onCompleted: {
         visibility = _config.radiator.fullScreenMode ? Window.FullScreen :  Window.Windowed;
    }

    SettingsDialog {
        id: settingsDialog
        x: (root.width - width) / 2
        y: (root.height - height) / 2
        width: root.width * 0.9
        height: root.height * 0.9
    }

    Shortcut {
        context: Qt.ApplicationShortcut
        sequence: "Ctrl+Q"
        onActivated: Qt.quit()
    }

    Shortcut {
        context: Qt.ApplicationShortcut
        sequence: "Ctrl+S"
        onActivated: settingsDialog.open()
    }

    Shortcut {
        context: Qt.ApplicationShortcut
        sequence: "Ctrl+D"
        onActivated: buildModel.dump()
    }

    Shortcut {
        context: Qt.ApplicationShortcut
        sequence: "Ctrl+F"
        onActivated: {
            console.log("Fullscreen toggle")
            visibility = visibility & Window.FullScreen ? Window.Windowed : Window.FullScreen
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Header {
            Layout.fillWidth: true
        }

        Rectangle {
            color: "black"
            Layout.fillHeight: true
            Layout.fillWidth: true

            BusyIndicator {
                visible: repeater.count == 0
                running: repeater.count == 0
                anchors.centerIn: parent
                width: Math.min(parent.height, parent.width) * 0.5
                height: width
            }

            BuildModel {
                id: buildModel
            }

            GridLayout {
                id: layout
                rowSpacing: 12
                columnSpacing: 12
                columns: repeater.count > 80 ? 6 : repeater.count > 40 ? 5 : repeater.count > 20 ? 4 : repeater.count > 10 ? 3 : repeater.count > 5 ? 2 : 1
                anchors.fill: parent
                anchors.margins: 10

                Repeater {
                    id: repeater
                    model: buildModel

                    BuildItem {
                        title: model.title
                        description: model.description

                        integrationServerLink: model.integrationServerLink
                        consoleLogLink: model.consoleLogLink
                        codeBrowserLink: model.codeBrowserLink

                        status: model.status
                        culprit: model.culprit
                        number: model.number
                        building: model.building
                        progress: model.progress
                        statusText: model.statusText
                        historicalBuilds: model.historicalBuilds

                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                }
            }
        }

        RowLayout {
            spacing: 0

            ToolButton {
                text: "\uf1de"
                font.family: Font.awesome
                font.pixelSize: 16
                onClicked: settingsDialog.open()
            }

            ToolButton {
                text: "\uf0b2"
                font.pixelSize: 16
                font.family: Font.awesome
                onClicked: root.visibility = root.visibility & Window.FullScreen ? Window.Windowed : Window.FullScreen
            }

            Item {
                Layout.fillWidth: true
            }

            ToolButton {
                text: "\uf08b"
                font.pixelSize: 16
                font.family: Font.awesome
                onClicked: Qt.quit()
            }
        }
    }
}
