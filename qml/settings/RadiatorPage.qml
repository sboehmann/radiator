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

SettingsPage {
    id: root

    GridLayout {
        columns: 2
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 10

        Label {
            text: qsTr("Fullscreen:")
        }

        CheckBox {
            id: fullScreen
            checked: config.radiator.fullScreenMode
            onCheckedChanged: config.radiator.fullScreenMode = checked
        }

        Label {
            text: qsTr("Blame:")
        }

        CheckBox {
            id: blame
            checked: config.radiator.blameEnabled
            onCheckedChanged: config.radiator.blameEnabled = checked
        }

        Label {
            text: qsTr("Job refresh interval:")
        }

        SpinBox {
            id: jobPollingInterval
            from: 0
            to: 60 * 60
            stepSize: value == 0 ? 60 : value < 10 ? 1 : value < 60 ? 5 : 10
            editable: true

            value: config.radiator.jobPollingInterval
            onValueChanged: config.radiator.jobPollingInterval = value

            textFromValue: function(value, locale) {
                if(value === 0) {
                    return qsTr("disabled");
                }

                return qsTr("%n second(s)", "", value);
            }
        }

        Label {
            text: qsTr("Build refresh interval:")
        }

        SpinBox {
            id: buildPollingInterval
            from: 0
            to: 60 * 60
            stepSize: value == 0 ? 60 : value < 10 ? 1 : value < 60 ? 5 : 10
            editable: true

            value: config.radiator.buildPollingInterval
            onValueChanged: config.radiator.buildPollingInterval = value

            textFromValue: function(value, locale) {
                if(value === 0) {
                    return qsTr("disabled");
                }

                return qsTr("%n second(s)", "", value);
            }
        }

        Label {
            text: qsTr("History:")
        }

        SpinBox {
            id: history
            from: 0
            to: 99
            stepSize: 1

            value: config.radiator.historicalBuildCount
            onValueChanged: config.radiator.historicalBuildCount = value

            textFromValue: function(value, locale) {
                if(value === 0) {
                    return qsTr("disabled");
                }

                return value;
            }
        }

        Label {
            text: qsTr("Show Header:")
        }

        CheckBox {
            id: showHeader
            checked: config.radiator.headerVisible
            onCheckedChanged: config.radiator.headerVisible = checked
        }

        Label {
            text: qsTr("Header Title:")
        }

        TextField {
            id: headerTitle
            text: config.radiator.headerTitle
            onTextChanged: config.radiator.headerTitle = text
        }

        Label {
            text: qsTr("Header Height:")
        }

        SpinBox {
            id: headerHeight
            from: 1
            to: 500
            value: config.radiator.headerHeight
            onValueChanged: config.radiator.headerHeight = value
        }

        Label {
            text: qsTr("Header show date:")
        }

        CheckBox {
            checked: config.radiator.headerShowDate
            onCheckedChanged: config.radiator.headerShowDate = checked
        }

        Label {
            text: qsTr("Header show time:")
        }

        CheckBox {
            checked: config.radiator.headerShowTime
            onCheckedChanged: config.radiator.headerShowTime = checked
        }
    }
}
