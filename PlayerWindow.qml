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

import QtMultimedia 5.9
import QtQuick.Dialogs 1.2
import Qt.labs.folderlistmodel 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import mvideooutput 1.0

SystemWindow {
    id: root
    width: def.win_width
    height: def.win_height

    onVisibleChanged: {
        if(showFlag == false){
            showFlag = true;
            setVideoPath(def.videoDefaultLocation)
        }
        else if(showFlag==true){
            showFlag=false
            videoStop()
        }
    }

    Define {
        id: def
        source_url: video_image.get_url()
    }

    property bool counter: false
    property bool play: false

    function reload() {
        counter = !counter
        image.source = "image://videoImageProvider?id=" + counter
    }

    Image {
        id: image
        anchors.fill: parent
    }

    Connections {
        target: video_image
        onCallQmlRefreshImage:{
            reload()
        }
        onCallQmlRefreshProgress:{
            player.media_duration = duration
            player.media_postion = position
        }
        onCallQmlPlayEnd:{
            videoBackward()
        }
        onCallQmlPlay:{
            player.enabled = true
            backButton.button_text = video_image.get_url()
            btn_play2.visible = false
            player.playing = true
        }

        onCallQmlPause:{
            player.playing = false
            btn_play2.visible = true
        }

        onCallQmlStop:{
            player.playing = false
            btn_play2.visible = true
        }
    }

    //左上角返回按钮显示视频名称
    MyIconButton {
        id: backButton
        icon_code: def.iconCode_back
        button_text: video_image.hasVideo() ? video_image.get_url() : "返回"
        button_color: "white"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 10
        onClicked: {
            videoStop()
            root.close()
        }
    }
/*
    //右上角选择视频文件夹按钮
    MyIconButton {
        id: openButton
        icon_code: def.iconCode_folder
        button_text: qsTr("打开文件")
        button_color: "white"
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 10
        onClicked:
        {
            if(video_image.hasVideo())
                videoPause()
            fileBrowser.nameFilter = def.videoNameFilters
            fileBrowser.defaultLocation = def.videoDefaultLocation
            fileBrowser.showNormal()
        }
    }
*/
    PlayerControlBar {
        id: player
        visible: true
        width: def.win_width
        enabled: video_image.hasVideo() & (folderModel.count!=0)
        anchors.bottom: parent.bottom
        media_duration: video_image.duration()
        onClicked_play:videoPlay()
        onClicked_pause: videoPause()
        onClicked_stop: videoStop()
        onClicked_step_backward: videoStepBackward()
        onClicked_step_forward: videoStepForward()
        onClicked_backward: videoBackward()
        onClicked_forward: videoForward()
        hourFlag: true      //时间标签带小时
//        onMoved_slider:
//        {
//            video_image.seek(slider_value)
//        }
    }

    function videoPlay()
    {
        player.playing = true
        play = true
        video_image.pause(false)
    }
    function videoPause()
    {
        player.playing = false
        play = false
        video_image.pause(true)
    }
    function videoStop()
    {
        player.playing = false
        play = false
        video_image.stop()
    }
    function videoStepForward()
    {
        if(video_image.duration() - video_image.position() > 10000)
            video_image.seek(video_image.position()+10000)
    }
    function videoStepBackward()
    {
        if(video_image.position() > 10000)
            video_image.seek(video_image.position()-10000)
    }
    function videoBackward()
    {
        videoSwitchFlag = true;
        videoStop();
        videoIndex -= 1;
        if(videoIndex < 0)
            videoIndex = getVideoCount()-1;
        video_image.open(getVideoURL(videoIndex))
        videoSwitchFlag = false;
    }
    function videoForward()
    {
        videoSwitchFlag = true;
        videoIndex += 1;
        if(videoIndex === getVideoCount() && getVideoCount()>1){
            videoIndex = 0;
            videoStop();
            video_image.open(getVideoURL(videoIndex))
            videoSwitchFlag = false;
        }
        else if(videoIndex === getVideoCount() && getVideoCount()===1){
            videoIndex = 0;
            videoStop();
            video_image.open(getVideoURL(videoIndex))
            videoSwitchFlag = false;
        }
    }

    //暂停时，视频中央显示的大按钮
    MyToolButton {
        id: btn_play2
        enabled: video_image.hasVideo() & (folderModel.count!=0)
        icon_code: def.iconCode_pause
        icon_size: 40
        icon_color: "#ffffff"
        color: "lightslategray"
        clr_exited: "darkgray"
        clr_entered: "teal"
        width: 80
        height: 80
        anchors.centerIn: parent
        onClicked: videoPlay()
        visible: player.playing ? false : true
    }
    //视频区域单击暂停播放切换
    MouseArea {
        enabled: video_image.hasVideo() && (getVideoCount() > 0)
        anchors.top: backButton.bottom
        anchors.bottom: player.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 50
        onClicked: player.playing ? videoPause() : videoPlay();
    }

    FolderListModel {
        id: folderModel
        objectName: "folderModel"
        showDirs: false
        nameFilters: def.videoNameFilters
        sortField: FolderListModel.Name
        onFolderChanged: {
            if(getVideoCount() === 0) {
                videoIndex = 0;
            }
            else {
                videoSwitchFlag = true;
                videoIndex = 0;
                videoStop()
                video_image.open(getVideoURL(videoIndex))
                videoSwitchFlag = false;
            }
        }
    }

    //自定义文件浏览器
    FileList{
        id: fileBrowser
        backButtonText: root.title
        onRejected: {
            if(video_image.hasVideo())
                videoPlay()
        }
        onAccepted: {
            videoSwitchFlag = true;
            //setVideoPath(fileBrowser.fileUrl + "/")
            //console.log("fileurl:"+fileUrl)
            videoIndex = fileBrowser.fileIndex
            videoStop()
            video_image.open(getVideoURL(videoIndex))
            videoSwitchFlag = false
        }
    }

    function setVideoPath(path)
    {
        folderModel.folder = path;
    }
    function getVideoURL(idx)
    {
        var filepath = folderModel.get(idx, "filePath")
        var path = filepath
        return path;
    }
    function getVideoFolder()
    {
        return folderModel.folder
    }
    function getVideoCount()
    {
        return folderModel.count
    }

    property int videoIndex: 0
    property bool videoSwitchFlag: false
}
