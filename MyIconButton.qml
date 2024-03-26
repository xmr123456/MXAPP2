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

/* 桌面按钮的实现，左边图标，右边文本 */
Rectangle {
    id: root
    visible: true
   /* width: 140
    height: 40*/
    width: icon.implicitWidth + button.implicitWidth + root.margin*2
    height: icon.implicitHeight + button.implicitHeight-5
    radius: 5

    color: clr_exited
//    border.color: "brown"
//    border.width: 1

    property alias icon_code: icon.text
    property alias icon_size: icon.width
    property alias button_text: button.text
    property color button_color: "white"
    property alias button_size: button.font.pixelSize

    property color clr_backgroud: "transparent" //背景透明色
    property color clr_entered: "lightgray"
    property color clr_exited: "transparent"
    property color clr_pressed: "teal"        //点击效果

    property int margin: 10

    signal clicked
    signal doubleClicked

    Text {
        id: icon
        font.family: "FontAwesome"
        font.pixelSize: 25
        text: Define.iconCode_back //返回图标
        color: button_color
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        anchors.verticalCenter: root.verticalCenter
        anchors.left: root.left
        anchors.leftMargin: root.margin
    }

    Text {
        id: button
        text: qsTr("返回")
        color: "white"
        font.bold: false
        font.family: "Microsoft Yahei"
        font.pixelSize: 18
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        anchors.verticalCenter: root.verticalCenter
        anchors.verticalCenterOffset: -2
        anchors.left: icon.right
        anchors.leftMargin: root.margin/2
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton; //只接受左键
        hoverEnabled: true
        onClicked: root.clicked()
        onEntered: root.color = clr_entered
        onExited: root.color = clr_exited
        onReleased: root.color = clr_exited
        onPressed: root.color = clr_pressed;
        onDoubleClicked: {
            root.doubleClicked();
//            root.color = clr_entered;
        }
    }
}
