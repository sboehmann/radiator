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
pragma Singleton
import QtQuick 2.0
import Radiator 1.0

Item {
    property alias firaCodeBold : firaCodeBold.name
    property alias firaCodeRegular : firaCodeRegular.name
    property alias awesome : awesome.name
    property alias nosifer : nosifer.name
    property alias audiowide : audiowide.name

    FontLoader {
        id: firaCodeBold
        source: "qrc:/fonts/FiraCode-Bold.ttf"
    }

    FontLoader {
        id: firaCodeRegular
        source: "qrc:/fonts/FiraCode-Regular.ttf"
    }

    FontLoader {
        id: awesome
        source: "qrc:/fonts/fontawesome-webfont.ttf"
    }

    FontLoader {
        id: nosifer
        source: "qrc:/fonts/Nosifer-Regular.ttf"
    }

    FontLoader {
        id: audiowide
        source: "qrc:/fonts/Audiowide-Regular.ttf"
    }

    function getJobBackgroundColor(status, opacity) {
        switch(status) {
        case Build.Unknown:
            return Qt.rgba(0.8, 0.8, 0.8, opacity);
        case Build.Success:
            return Qt.rgba(0, 1, 0, opacity);
        case Build.Unstable:
            return Qt.rgba(1, 1, 0, opacity);
        case Build.Failure:
            return Qt.rgba(1, 0, 0, opacity);
        case Build.Disabled:
            return Qt.rgba(0.4, 0.4, 0.4, opacity);
        case Build.NoBuild:
            return Qt.rgba(0.6, 1, 0.8, opacity);
        case Build.Aborted:
            return Qt.rgba(1, 0.4, 0.8, opacity);
        case Build.Cancelled:
            return Qt.rgba(0.6, 0.5, 0.2, opacity);
        }

        console.log("Fonts.getJobBackgroundColor unknown status:", status);
        return "transparent";
    }
}
