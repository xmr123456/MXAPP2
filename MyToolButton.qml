﻿/***********************************************************************
 *  This file is part of MXAPP2

    Copyright (C) 2020-2024

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    Additional permission under GNU Lesser General Public License version 3.0
    See <https://www.gnu.org/licenses/lgpl-3.0.html> for more details.
***********************************************************************/

import QtQuick 2.0

/* 左边图标*/
Rectangle {
    id: root
    visible: true
    width: 40
    height: width
    radius: cirFlag ? width/2 : width/10;
    color: "transparent"

//    border.color: "green"
//    border.width: 1

    property alias icon_code: icon.text
    property alias icon_size: icon.font.pixelSize
    property alias icon_color: icon.color

    property color clr_backgroud: "transparent" //背景透明色
    property color clr_entered: "darkgray"
    property color clr_exited: clr_backgroud
    property color clr_pressed: "greenyellow"        //点击效果

    property int offsetV: 0
    property int offsetH: 0

    property bool cirFlag: false     //是矩形还是圆形标志位

    property int margin: 10

    signal clicked

    Text {
        id: icon
        font.family: "FontAwesome"
        font.pixelSize: 20
        text: Define.iconCode_back //返回图标
        color: "white"
        opacity: 1.0        //不透明
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        anchors.verticalCenterOffset: offsetV
        anchors.horizontalCenterOffset: offsetH
        anchors.centerIn: root
    }

    MouseArea {
        anchors.fill: root
        acceptedButtons: Qt.LeftButton; //只接受左键
        hoverEnabled: true
        onClicked: root.clicked()
        onEntered:
        {
            root.color = clr_entered
            icon.scale = 1.3
        }
        onExited:
        {
            root.color = clr_exited
            icon.scale = 1
        }
        onReleased: root.color = clr_entered
//        onReleased: root.color = "transparent"
        onPressed: root.color = clr_pressed;
    }
}
