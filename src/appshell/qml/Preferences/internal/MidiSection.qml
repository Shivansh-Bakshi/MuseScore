/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
import QtQuick 2.15

import MuseScore.UiComponents 1.0
import MuseScore.Preferences 1.0

Column {
    spacing: 18

    property var preferencesModel: null

    StyledTextLabel {
        text: qsTrc("appshell", "MIDI")
        font: ui.theme.bodyBoldFont
    }

    ComboBoxWithTitle {
        title: qsTrc("appshell", "Shortest note:")
        titleWidth: 220

        currentIndex: control.indexOfValue(preferencesModel.currentShortestNote)
        model: preferencesModel.shortestNotes()

        control.textRoleName: "title"
        control.valueRoleName: "value"

        onValueEdited: {
            preferencesModel.currentShortestNote = newValue
        }
    }
}
