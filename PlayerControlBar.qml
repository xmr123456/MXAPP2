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
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: root
    visible: true
    width: def.win_width
    height: def.win_height/5
    color: "transparent"

    //单位ms
    //作为输入
    property real media_duration;   //400000=400ms=06:40
    property real media_postion;    //120000=120ms=02:00
    //作为输出
    property real slider_value: 0;    //0.7*400000=280000
    property bool playing: false
    property real media_volume: 0.2
    property bool volume_showFlag: false
    property bool hourFlag: false

    signal clicked_play
    signal clicked_pause
    signal clicked_stop
    signal clicked_backward
    signal clicked_step_backward
    signal clicked_step_forward
    signal clicked_forward
    signal moved_slider
    //    signal clicked_volume

    Define {id: def}

    RowLayout {
        id: row_display
        spacing: 10
        anchors.bottom: row_control.top
        anchors.left: root.left
        anchors.right: root.right
        Text {
            id: time_now
            text: (hourFlag) ? def.msToHHMMSS(media_postion) : def.msToMMSS(media_postion)
            color: "white"
        }

        Slider {
            id: progress
            value: media_postion/media_duration
            Layout.fillWidth: true
            onMoved:
            {
                slider_value = value * media_duration;
                root.moved_slider()
                //                root.clicked_play()
            }
        }

        Text {
            id: time_long
            text: (hourFlag) ? def.msToHHMMSS(media_duration) : def.msToMMSS(media_duration)
            color: "white"
        }

        MyToolButton {
            id: btn_volume
            icon_code: (media_volume == 0) ? def.iconCode_volume_mute : ((media_volume < 0.5) ? def.iconCode_volume_down : def.iconCode_volume_up);
            onClicked: {
                root.volume_showFlag = !root.volume_showFlag;
                //                root.volume_showFlag = true;
                //                console.log("clicked")
            }
        }
    }

    RowLayout {
        id: row_control
        anchors.horizontalCenter: root.horizontalCenter
        anchors.bottom: root.bottom

        spacing: 10
        MyToolButton {
            id: btn_backward      //上一曲
            icon_code: def.iconCode_backward
            onClicked: root.clicked_backward()
        }
        MyToolButton {
            id: btn_step_backward        //快退
            icon_code: def.iconCode_step_backward
            onClicked: root.clicked_step_backward()
        }
        MyToolButton {
            id: btn_play
            icon_code: (root.playing) ? def.iconCode_play : def.iconCode_pause;
            onClicked:
            {
                root.playing = !root.playing;
                if(root.playing)
                {
                    root.clicked_play()
                }
                else
                {
                    root.clicked_pause()
                }
            }
        }
        MyToolButton {
            id: btn_stop
            icon_code: def.iconCode_stop
            onClicked:
            {
                root.clicked_stop()
                root.playing = false;
            }
        }
        MyToolButton {
            id: btn_step_forward        //快进
            icon_code: def.iconCode_step_forward
            onClicked: root.clicked_step_forward()
        }
        MyToolButton {
            id: btn_forward             //下一曲
            icon_code: def.iconCode_forward
            onClicked: root.clicked_forward()
        }
    }

    Slider {
        id: volumeSlider
        orientation: Qt.Vertical
        anchors.bottom: root.top
        anchors.right: root.right
        anchors.rightMargin: 10
        visible: root.volume_showFlag
        width: 20
        height: 100
        from: 0
        to: 1
        value: 0.2
        onValueChanged: media_volume = value;
    }
}
