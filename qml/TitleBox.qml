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

GroupBox {
    id: control

    property bool showTitle : true

    padding: 0
    spacing: 0
    topPadding: padding + (label ? label.implicitHeight + spacing : 0)

    label: Label {
        id: textLabel
        visible: control.showTitle
        x: control.leftPadding
        width: control.availableWidth
        text: control.title
        font.pixelSize: fm.font.pixelSize - 1
        font.capitalization: Font.AllUppercase
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        opacity: 0.7

        FontMetrics {
            id: fm
        }
    }

    background: Item {}
}
