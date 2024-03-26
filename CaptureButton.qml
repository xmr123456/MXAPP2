﻿/***********************************************************************
 *  This file is part of MXAPP2

    Copyright (C) 2020-2024 XuMR <2801739946@qq.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
***********************************************************************/

import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle {
    id: root
    width: 50
    height: width
    radius: width/2
    color: "white"

    signal captureImage

    signal captureVideoStart
    signal captureVideoStop

    Rectangle {
        id: shotButton
        width: root.width-6
        height: width
        radius: width/2
        color: "black"
        anchors.centerIn: parent
        Rectangle {
            id: captureRec
            width: root.width-12
            height: width
            radius: width/2
            color: "white"
            anchors.centerIn: parent
            MouseArea {
                enabled: true
                anchors.fill: parent
                onClicked: root.captureImage()
                onPressed: captureRec.color = "black"
                onExited: captureRec.color = "white"
            }
        }
    }
}
