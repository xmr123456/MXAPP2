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

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    Additional permission under GNU Lesser General Public License version 3.0
    See <https://www.gnu.org/licenses/lgpl-3.0.html> for more details.
***********************************************************************/

import QtQuick 2.5
import QtMultimedia 5.9
import QtQuick.Dialogs 1.0
import Qt.labs.folderlistmodel 2.1
import MyFunction.module 1.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
SystemWindow {
    id: root
    width: def.win_width
    height: def.win_height
    onVisibleChanged: {
        if(showFlag == false)
        {
            showFlag = true;
            setImageFolder(def.imageDefaultLocation)
        }
    }

    Define {
        id: def
        source_url: w_preview.imageUrl
    }
    MyFunction {id: myFunction}

    //左上角返回按钮
    MyIconButton {
        id: backButton
        icon_code: def.iconCode_back
        button_text: qsTr("返回")
        button_color: "white"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 10
        onClicked: root.close()
    }

    //中央网格图片显示
    Rectangle {
        width: def.win_width
        height: def.win_height - backButton.height
        color: "transparent"        //透明
        anchors.top: backButton.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        //        border.color: "lightgreen"       //边框颜色
        //        border.width: 1
        GridView {
            id: grid
            width: parent.width - 15*2  //左右各15间距
            height: parent.height - 5*2 //上下各5间距
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            cellHeight: grid.width / def.albumNumber
            cellWidth: cellHeight
            clip: true
            focus: false
            //model提供数据
            model: folderModel
            //delegate显示数据
            delegate: imageDelegate
        }
        Component {
            id: imageDelegate
            Rectangle {
                //图片之间的间距为albumMargins*2
                width: grid.cellWidth - def.albumMargins/2
                height: width
                color: "transparent"
                //圆角矩形
                CircularImage {
                    id: image
                    width: parent.width
                    height: parent.height
                    source: getImageFolder() + fileName
                    anchors.fill: parent
                    radius: 5
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        grid.currentIndex = index
                        imageIndex = index
                        console.log(image.source)
                        w_preview.imageUrl = getImageURL(imageIndex)
                        w_preview.imageName = fileName
                        w_preview.showNormal();
                    }
                }
            }
        }
    }

    //右上角选择图片文件夹按钮
    MyIconButton {
        id: openButton
        icon_code: def.iconCode_folder
        button_text: qsTr("打开文件")
        button_color: "white"
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 10
        onClicked: {
            fileBrowser.nameFilter = def.imageNameFilters
            fileBrowser.defaultLocation = def.imageDefaultLocation
            fileBrowser.showNormal()
//            fileDialog.open()
        }
    }

    FolderListModel {
        id: folderModel
        objectName: "folderModel"
        showDirs: false
        nameFilters: def.imageNameFilters
        sortField: FolderListModel.Name
//        onStatusChanged: console.log("status:", folderModel.status);
        onFolderChanged: {
            if(folderModel.count == 0){
                console.log("所选文件夹无图片文件:" + getImageFolder())
            }
            else {
                console.log("共发现" + count + "张图片文件");
            }
        }
    }

    //自定义文件浏览器
    FileList {
        id: fileBrowser
        backButtonText: root.title
        onRejected: console.log("没有选择任何文件夹")
        onAccepted: {
            setImageFolder(fileUrl + "/");
            console.log("设置图片文件夹为:", fileUrl + "/");
        }
    }

    AlbumPreview {
        id: w_preview
        onClicked_next: nextImage()
        onClicked_previous: previousImage()
        onClicked_delete: {
            var imgPath = folderModel.get(imageIndex, "filePath")
            myFunction.deleteFile(imgPath);
            if(getImageCount() != 0)
                previousImg()
            else
                w_preview.close();
        }
    }

    function previousImage()
    {
        imageIndex--;
        if(imageIndex < 0)
            imageIndex = getImageCount()-1
        w_preview.imageUrl = getImageURL(imageIndex)
        w_preview.imageName = def.getFileName()
        console.log("上一张:" + (imageIndex+1) + "/"  + getImageCount() + ":" + w_preview.imageUrl);
    }
    function nextImage()
    {
        imageIndex++;
        if(imageIndex == getImageCount())
            imageIndex = 0;
        w_preview.imageUrl = getImageURL(imageIndex)
        w_preview.imageName = def.getFileName()
        console.log("下一张:" + (imageIndex+1) + "/"  + getImageCount() + ":" + w_preview.imageUrl);
    }

    function setImageFolder(path)
    {
        folderModel.folder = path
    }
    function getImageURL(idx)
    {
        var path = "file://";
        var filepath = folderModel.get(idx, "filePath")
//        if (filepath[1] === ':') // Windows drive logic, see QUrl::fromLocalFile()
//            path += '/';
        path += filepath;

        console.log(path)
        return path;
    }
    function getImageFolder()
    {
        return folderModel.folder
    }
    function getImageCount()
    {
        return folderModel.count
    }
    property int imageIndex: 0
}

