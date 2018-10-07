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

import Radiator.Config 1.0

import ".."

Dialog {
    id: root
    title: qsTr("Settings")
    modal: true
    focus: true
    standardButtons: Dialog.Save | Dialog.Cancel

    Config {
        id: cfg
    }

    onVisibleChanged: if(visible) cfg.reload();
    onAccepted: cfg.save();

    Item {
        anchors.fill: parent

        Item {
            id: sideBar
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 140


            ColumnLayout {
                id: sideBarLayout
                anchors.fill: parent

                Repeater {
                    model: [ qsTr("Radiator"), qsTr("Remotes"), qsTr("Branches"), qsTr("Proxy") ]

                    TabButton {
                        text: modelData
                        Layout.fillWidth: true
                        checkable: true
                        checked: swipeView.currentIndex === index

                        onCheckedChanged: {
                            if(checked) {
                                swipeView.setCurrentIndex(index)
                            }
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                }
            }
        }

        SwipeView {
            id: swipeView

            clip: true
            interactive: false
            orientation: Qt.Vertical
            anchors.left: sideBar.right
            anchors.leftMargin: 5
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            RadiatorPage {
                id: radiatorPage
                config: cfg
            }

            RemotePage {
                id: remotePage
                config: cfg
            }

            BranchesPage {
                id: branchesPage
                config: cfg
            }

            ProxyPage {
                id: proxyPage
                config: cfg
            }
        }
    }
}
