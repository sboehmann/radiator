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

SettingsPage {
    id: root

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        RadioButton {
            id: systemProxy
            text: qsTr("Use System Settings")

            checked: !config.proxy.enabled
            //onCheckedChanged: config.proxy.enabled = !checked
        }

        RadioButton {
            id: manuelProxy
            text: qsTr("Manuel Configuration")

            checked: config.proxy.enabled
            onCheckedChanged: config.proxy.enabled = checked
        }

        Item {
            Layout.topMargin: 10
            Layout.leftMargin: 30
            Layout.fillWidth: true
            visible: manuelProxy.checked

            ColumnLayout {
                anchors.fill: parent

                RowLayout {
                    Layout.fillWidth: true

                    TitleBox {
                        title: qsTr("Host")
                        Layout.fillWidth: true

                        TextField {
                            id: host
                            anchors.fill: parent
                            text: config.proxy.host
                            onTextChanged: config.proxy.host = text
                        }
                    }

                    TitleBox {
                        title: qsTr("Port")

                        SpinBox {
                            id: port
                            anchors.fill: parent
                            editable: true
                            from: -1
                            to: 65535
                            value: config.proxy.port
                            onValueChanged: config.proxy.port = value

                            textFromValue: function(value) {
                                return value < 0 ? "" : value;
                            }
                        }
                    }
                }

                TitleBox {
                    title: qsTr("User")
                    Layout.fillWidth: true

                    TextField {
                        id: username
                        anchors.fill: parent
                        text: config.proxy.username
                        onTextChanged: config.proxy.username = text
                    }
                }

                TitleBox {
                    title: qsTr("Password")
                    Layout.fillWidth: true

                    TextField {
                        id: password
                        anchors.fill: parent
                        text: config.proxy.password
                        onTextChanged: config.proxy.password = text
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
