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
import QtQuick 2.7
import QtQuick.Layouts 1.3
import MuseScore.UiComponents 1.0
import MuseScore.UserScores 1.0

import "internal"

FocusScope {
    id: root

    QtObject {
        id: privateProperties

        readonly property int sideMargin: 133
        readonly property int buttonWidth: 134
    }

    RecentScoresModel {
        id: recentScoresModel
    }

    Rectangle {
        id: background
        anchors.fill: parent

        color: ui.theme.backgroundSecondaryColor

        MouseArea {
            anchors.fill: parent
            onClicked: {
                forceActiveFocus()
            }
        }
    }

    RowLayout {
        id: topLayout

        anchors.top: parent.top
        anchors.topMargin: 66
        anchors.left: parent.left
        anchors.leftMargin: privateProperties.sideMargin
        anchors.right: parent.right
        anchors.rightMargin: privateProperties.sideMargin

        spacing: 12

        StyledTextLabel {
            id: pageTitle

            text: qsTrc("userscores", "Scores")
            font: ui.theme.titleBoldFont
        }

        SearchField {
            id: searchField

            Layout.maximumWidth: width
            Layout.alignment: Qt.AlignHCenter
        }

        Item {
            Layout.preferredWidth: pageTitle.width
        }
    }

    Rectangle {
        anchors.top: view.top

        width: parent.width
        height: 8
        z: 1

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: background.color
            }

            GradientStop {
                position: 1.0
                color: "transparent"
            }
        }
    }

    RecentScoresView {
        id: view

        anchors.top: topLayout.bottom
        anchors.topMargin: 74
        anchors.left: parent.left
        anchors.leftMargin: privateProperties.sideMargin - view.sideMargin
        anchors.right: parent.right
        anchors.rightMargin: privateProperties.sideMargin - view.sideMargin
        anchors.bottom: buttonsPanel.top

        backgroundColor: background.color

        model: SortFilterProxyModel {
            sourceModel: recentScoresModel

            filters: [
                FilterValue {
                    roleName: "title"
                    roleValue: searchField.searchText
                    compareType: CompareType.Contains
                }
            ]
        }

        onAddNewScoreRequested: {
            forceActiveFocus()

            recentScoresModel.addNewScore()
        }

        onOpenScoreRequested: {
            forceActiveFocus()

            recentScoresModel.openRecentScore(scorePath)
        }
    }

    Rectangle {
        id: buttonsPanel

        anchors.bottom: parent.bottom

        height: 114
        width: parent.width

        color: ui.theme.popupBackgroundColor

        Row {
            anchors.right : parent.right
            anchors.rightMargin: privateProperties.sideMargin
            anchors.verticalCenter: parent.verticalCenter

            spacing: 22

            FlatButton {
                width: privateProperties.buttonWidth
                text: qsTrc("userscores", "New")

                onClicked: {
                    recentScoresModel.addNewScore()
                }
            }

            FlatButton {
                width: privateProperties.buttonWidth
                text: qsTrc("userscores", "Open other...")

                onClicked: {
                    recentScoresModel.openScore()
                }
            }
        }
    }
}
