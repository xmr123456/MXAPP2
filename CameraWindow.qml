/***********************************************************************
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

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    Additional permission under GNU Lesser General Public License version 3.0
    See <https://www.gnu.org/licenses/lgpl-3.0.html> for more details.
***********************************************************************/

import QtQuick 2.5
import QtMultimedia 5.9
import mvideooutput 1.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
SystemWindow {
    id: root
    property int adaptive_width: Screen.desktopAvailableWidth
    property int adaptive_height: Screen.desktopAvailableHeight
    width: adaptive_width
    height: adaptive_height

    onVisibleChanged: {
        if(showFlag == false)
        {
            showFlag = true
            console.log("相机窗口被激活")
            show_image.start_captrue()
        }
    }

    Define {id: def}
    Album {id: w_album}
    property bool counter: false
    function reload() {
        counter = !counter
        image.source = "image://cameraImageProvider?id=" + counter
    }
    Image {
        id: image
        anchors.fill: parent
    }

    Connections {
        target: show_image
        onCallQmlRefreshImage:{
            reload()
        }
        onCallQmlSavePath:{
            bar.img_src = "file://" + path
        }
    }

    //Camera {id: mediaDevices}

    ListModel {id: imagePaths}

    //相机界面，左上角返回按钮
    MyIconButton {
        id: backButton
        icon_code: def.iconCode_back
        button_text: QtMultimedia.defaultCamera.displayName
        button_color: "white"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 10
        onClicked: {
            root.close()
            showFlag = false
            show_image.stop_capture()
        }
    }

    //右上角跳转到图库按钮
    MyIconButton {
        id: albumButton
        icon_code: def.iconCode_album
        button_text: qsTr("所有图片")
        button_color: "white"
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 10
        onClicked: {
            w_album.setImageFolder(def.imageDefaultLocation)
            w_album.showNormal()
        }
    }

    //底部拍照功能区域：预览图片+矩形+按钮
    CaptureBar {
        id: bar
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        //img_src: QtMultimedia.
        onCaptureImage: {
            //保存照片到指定位置
            var savePath = def.captureSavePath + def.captureSaveHead + def.getCurrentTime();
            show_image.captureImg(savePath)
        }

        //以下为小视频录制功能
        onCaptureVideoStart: {
            console.log("capture video start")
        }
        onCaptureVideoStop: {
            console.log("capture video stop")
        }
    }
}
