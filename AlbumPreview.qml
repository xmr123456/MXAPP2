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

import QtQuick 2.5
import QtMultimedia 5.9
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
SystemWindow {
    id: root
    width: def.win_width
    height: def.win_height

    property string imageName: qsTr("返回")
    property alias imageUrl: backgroundImage.source

    signal clicked_previous
    signal clicked_next
    signal clicked_delete
    signal clicked_cancel

    Define {id: def}

//    color: "black"

    property real x1: 0
    property real x2: 0
    property bool mouseDragEnable: true

    //鼠标左右滑动效果
    MouseArea {
        id: mouse
        anchors.fill: backgroundImage
        enabled: true
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton

        onPressed: x1 = mouseX;
        onReleased: {
            x2 = mouseX;
            if(mouseDragEnable)
            {
                if(x2 - x1 < def.slidLength * (-1))
                    nextImg();
                else if(x2 - x1 > def.slidLength)
                    previousImg();
            }
        }
    }

    //最底层背景图片
    Image {
        id: backgroundImage
        anchors.fill: parent
        width: parent.width
        height: parent.height
        source: def.url_img_preview
        fillMode: Image.PreserveAspectFit
        clip: true
    }

    //删除图片时上层灰色阴影
    Rectangle {
        id: blackTransparent
        visible: false
        anchors.fill: parent
        width: parent.width
        height: parent.height
        color: "black"
        opacity: 0.4        //透明度
    }

    MyIconButton {
        id: backButton
        icon_code: def.iconCode_back
//        button_text: "返回"
        button_text: imageName
        button_color: "white"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 10
        onClicked: root.close()
    }
    MyIconButton {
        id: albumButton
        icon_code: def.iconCode_album
        button_text: qsTr("所有图片")
        button_color: "white"
        button_size: 18
        anchors.right: deleteButton.left
        anchors.top: parent.top
        anchors.margins: 10
        onClicked: close()
    }

    //删除按钮
    MyIconButton {
        id: deleteButton
        icon_code: def.iconCode_trash
        button_text: qsTr("删除图片")
        button_color: "white"
        button_size: 18

        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 10
        onClicked: {
            blackTransparent.visible = true
            delDialog.visible = true
            setButtonEnable(false)
        }
    }
    //上一张图片按钮
    MyToolButton {
        id: previousButton
        icon_code: def.iconCode_left
        icon_size: 45
        offsetV: -3
        offsetH: -4
        width: 60
        opacity: 0.7
        cirFlag: true
        color: "silver"
        clr_backgroud: "silver"
        clr_exited: "silver"
        clr_entered: "darkgray"
        clr_pressed: "gray"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 10
        onClicked: previousImg();
    }
    MyToolButton {
        id: nextButton
        icon_code: def.iconCode_right
        icon_size: 45
        offsetV: -3
        offsetH: 4
        width: 60
        opacity: 0.7
        cirFlag: true
        color: "silver"
        clr_backgroud: "silver"
        clr_exited: "silver"
        clr_entered: "darkgray"
        clr_pressed: "gray"
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 10
        onClicked: nextImg()
    }

    DeleteDialog{
        id: delDialog
        anchors.centerIn: parent
        onClicked_cancel:
        {
            delDialog.visible = false;
            blackTransparent.visible = false;
            setButtonEnable(true)
            root.clicked_cancel()
            console.log("取消删除")
        }
        onClicked_delete:
        {
            delDialog.visible = false;
            blackTransparent.visible = false;
            setButtonEnable(true)
            root.clicked_delete()
            console.log("确定删除")
        }
    }

    function nextImg()
    {
        root.clicked_next();
    }
    function previousImg()
    {
        root.clicked_previous()
    }
    //弹出删除窗口时，底部按钮无效
    function setButtonEnable(enable)
    {
        backButton.enabled = enable
        albumButton.enabled = enable
        deleteButton.enabled = enable
        nextButton.enabled = enable
        previousButton.enabled = enable
        mouseDragEnable = enable
    }
}
