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

Item {
    id: root
    implicitHeight: scrollingText.height
    clip: true

    property alias text: scrollingText.text
    property alias font: scrollingText.font
    property alias color: scrollingText.color
    property alias minimumPixelSize: scrollingText.minimumPixelSize
    property alias verticalAlignment: scrollingText.verticalAlignment
    property alias horizontalAlignment: scrollingText.horizontalAlignment

    property int elide: Text.ElideRight
    property bool marquee: true
    property int velocity: 25

    Text {
        id: scrollingText
        width: parent.width
        height: parent.height
        elide: marquee ? Text.ElideNone : root.elide
        fontSizeMode: marquee || scrollingText.elide != Text.ElideNone ? Text.VerticalFit : Text.Fit
    }

    SequentialAnimation {
        running: marquee && scrollingText.contentWidth > root.width
        onRunningChanged: {
            scrollingText.x = 0;
        }

        loops: Animation.Infinite

        PropertyAction {
            target: scrollingText
            property: "x"
            value: scrollingText.width
        }

        NumberAnimation {
            target: scrollingText
            property: "x"
            to: -scrollingText.contentWidth
            duration: Math.max(0, (scrollingText.contentWidth - scrollingText.width) * velocity);
        }
    }
}
