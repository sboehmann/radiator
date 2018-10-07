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
import QtQuick.Layouts 1.3
import Radiator 1.0

import "."

Item {
    id: root
    property alias title : title.text
    property alias description: description.text
    //property string description
    property alias number: number.text
    property alias statusText: statusText.text
    property alias culprit: culprit.text
    property int status: Build.Unknown
    property bool building: false
    property int progress: 0
    property var historicalBuilds

    property url integrationServerLink
    property url consoleLogLink
    property url codeBrowserLink

    implicitHeight: 20
    implicitWidth: 100

    Rectangle {
        id: background
        anchors.fill: parent
        color: Fonts.getJobBackgroundColor(root.status, root.building ? 0.2 : 0.6)

        radius: 10
        border.width: 2
        border.color: "black"
    }

    Item {
        id: buildOverlay
        visible: root.building
        anchors.fill: parent
        anchors.margins: background.border.width

        Rectangle {
            width: parent.width / 100.0 * root.progress
            color: Fonts.getJobBackgroundColor(root.status, 0.6)
            height: parent.height
        }

        Timer {
            interval: buildOverlay.opacity > 0.9 ? 1000 : 200
            running: root.building
            repeat: true
            onTriggered: {
                buildOverlay.opacity = buildOverlay.opacity > 0.9 ? 0.4 : 1
            }
        }

        Behavior on opacity {
            OpacityAnimator { duration: buildOverlay.opacity > 0.9 ? 200 : 600 }
        }
    }

    ColumnLayout {
        id: layout
        anchors.fill: parent
        anchors.margins: parent.height > 40 ? 10 : 0

        RowLayout {
            Layout.fillWidth: true
            spacing: 4

            Item {
                Layout.fillWidth: true
            }

            Repeater {
                model: historicalBuilds

                SubBuildItem {
                    Layout.preferredWidth: layout.height / 8
                    Layout.preferredHeight: layout.height / 8
                    status: modelData.status
                    buildNumber: modelData.buildNumber
                    border.color: "black"
                    border.width: 1
                }
            }
        }

        MargueeText {
            id: title
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.rightMargin: 10
            Layout.leftMargin: 10
            color: root.building ? "lightgrey" : "white"

            font.bold: true
            font.pixelSize: 220
            font.family: Fonts.firaCodeBold
            font.strikeout: root.status === Build.Disabled

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            minimumPixelSize: 8

            marquee: _config.radiator.marquee > 0
            velocity: _config.radiator.marquee
            elide: _config.radiator.elide
        }

        Text {
            id: description
            visible: text.length > 0
            Layout.fillWidth: true
            color: root.building ? "lightgrey" : "white"
            font.pixelSize: root.height / 6.0
            font.strikeout: root.status === Build.Disabled

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            elide: Text.ElideRight
        }

        RowLayout {
            visible: parent.height > 40
            Layout.fillWidth: true
            Layout.rightMargin: 10
            Layout.leftMargin: 10

            Text {
                id: number
                Layout.fillWidth: true
                Layout.preferredWidth: Math.max(number.width, statusText.width)
                font.family: Fonts.firaCodeRegular
                font.pixelSize: root.height / 8.0
                horizontalAlignment: Text.AlignLeft
                color: "lightgray"
            }

            Text {
                id: culprit
                Layout.fillWidth: true
                font.family: Fonts.firaCodeRegular
                font.pixelSize: root.height / 8.0
                horizontalAlignment: Text.AlignHCenter
                color: "lightgray"
            }

            Text {
                id: statusText
                Layout.fillWidth: true
                Layout.preferredWidth: Math.max(number.width, statusText.width)
                font.family: Fonts.firaCodeRegular
                font.pixelSize: root.height / 8.0
                horizontalAlignment: Text.AlignRight
                color: "lightgray"
            }
        }
    }
}
