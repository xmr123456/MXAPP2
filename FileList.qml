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
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.0
import Qt.labs.folderlistmodel 2.1
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1

SystemWindow {
    id: root
    width: def.win_width
    height: def.win_height

    //输入
    property var nameFilter: def.audioNameFilters
    property string defaultLocation: def.audioDefaultLocation
//    property alias backButtonText: backButton.button_text
    property string backButtonText: "File Browser"

    //输出
    property url fileUrl;
    property int fileIndex: 0

    signal accepted
    signal rejected

//    onClosing: {
//        console.log("FileBrowser is closing");
//    }
    function showNormal(){
        open()
    }

    Define {
        id: def
    }
    //左上角返回按钮
    MyIconButton {
        id: backButton
        icon_code: def.iconCode_back
//        button_text: folderModel.status == FolderListModel.Ready ? "媒体文件" : "正在加载"
        button_text: folderModel.folder
        button_color: "white"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 10
        onClicked: {
            root.rejected()
            close()
        }
    }

    Rectangle {
        id: rootRectangle
        width: def.win_width-100
        height: def.win_height-backButton.height-20
        visible: true
        color: "transparent"
        opacity: 0.8
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: backButton.bottom
        //矩形上面返回上一级按钮
        MyIconButton {
            id: parentButton
            icon_code: def.iconCode_file_folder
            width: rootRectangle.width - 20
            anchors.horizontalCenter: parent.horizontalCenter
            button_text: "......"
            button_color: "#FFCF2B"
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 10
            onClicked: {
                if(folderModel.parentFolder != ""){
                    folderModel.folder = folderModel.parentFolder;
                    fileUrl=folderModel.folder
                }
            }
        }
        //磁盘文件浏览器视图
        ListView {
            id: listFolder
            visible: true
          //  width: 800
           // height: 480
//            clip: true
            spacing: 10

            anchors.top: parentButton.bottom
            anchors.topMargin: 10
            anchors.right: parentButton.right
            anchors.left: parentButton.left
	    anchors.bottom:parent.bottom
	    anchors.bottomMargin: 10

            focus: false
            //model提供数据
            model: folderModel
            //delegate显示数据
            delegate: component
        }
        Component {
            id: component
            //文件和文件夹列表
            MyIconButton {
                icon_code: fileIsDir ? def.iconCode_file_folder : def.getFileIconCode(fileName)
                width: rootRectangle.width - 20
                button_text: fileName
                button_color: fileIsDir ? "#FFCF2B" : "white"
                onClicked: {
                    if(fileIsDir){
                        folderModel.folder = folderModel.get(index, "fileURL")
                        fileUrl=folderModel.folder
                    }
                    else {
                        fileIndex = index;
                        fileUrl = folderModel.folder;
                        console.log("index:", index, ":" , fileUrl);
                        root.accepted()
                        close()
                        console.log(filePath)
                        console.log(def.getFileSuffix(fileName), ":", fileName);
                    }


                }
                onDoubleClicked: {
                    if(!fileIsDir)  //是一个文件
                    {
                        fileIndex = index;
                        fileUrl = folderModel.folder;
                        console.log("index:", index, ":" , fileUrl);
                        root.accepted()
                        close()
                    }
                }
            }
        }
        FolderListModel {
            id: folderModel
            objectName: "folderModel"
            showDirs: true

            nameFilters: nameFilter
            folder: defaultLocation

            sortField: FolderListModel.Type
            onFolderChanged: {
                console.log("current folder:", folderModel.folder)
                fileUrl = folderModel.folder;
            }
            //            onStatusChanged: console.log("status:", folderModel.status);
        }
    }
}
