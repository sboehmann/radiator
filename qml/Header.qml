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

import "."

Rectangle {
    id: rectangle
    implicitHeight: _config.radiator.headerHeight
    color: "black"

    visible: _config.radiator.headerVisible

    QtObject {
        id: d
        property var locale: Qt.locale()
        property date currentDate: new Date()
    }

    Timer {
        interval: 1000
        running: true
        repeat: true
        onTriggered: d.currentDate = new Date()
    }

    Text {
        id: teamName
        text: _config.radiator.headerTitle
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter

        anchors.fill: parent
        anchors.rightMargin: Math.max((time.visible ? time.contentWidth : 0), (date.visible ? date.contentWidth : 0)) + 20
        anchors.leftMargin: anchors.rightMargin

        fontSizeMode: Text.Fit
        minimumPixelSize: 8
        font.pixelSize: 128
        font.family: Fonts.audiowide
        color: "white"
    }

    ColumnLayout {
        id: dateTimeLayout
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.rightMargin: 20
        spacing: 0

        Text {
            id: time
            visible: _config.radiator.headerShowTime
            Layout.fillWidth: true
            Layout.minimumHeight: 20
            Layout.preferredHeight: 100
            Layout.fillHeight: true

            text: d.currentDate.toLocaleTimeString(locale, qsTr("hh:mm:ss"));
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignBottom
            fontSizeMode: Text.Fit
            minimumPixelSize: 12
            font.pixelSize: 250
            font.bold: true
            font.family: Fonts.firaCodeBold
            color: "white"
        }

        Text {
            id: date
            visible: _config.radiator.headerShowDate
            Layout.fillWidth: true
            Layout.minimumHeight: 10
            Layout.preferredHeight: 50
            Layout.fillHeight: true

            text: d.currentDate.toLocaleDateString(locale, qsTr("dddd dd.MM.yyyy"));
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignTop
            fontSizeMode: Text.Fit
            minimumPixelSize: 10
            font.pixelSize: 100
            font.bold: false
            font.family: Fonts.firaCodeRegular
            color: "white"
        }
    }
}
