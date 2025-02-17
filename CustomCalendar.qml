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

    Additional permission under GNU Lesser General Public License version 3.0
    See <https://www.gnu.org/licenses/lgpl-3.0.html> for more details.
***********************************************************************/

import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1

Rectangle {
    id:mx_calendar
    property alias calendar_control:m_calendar

    Calendar{
        id: m_calendar
        //anchors.centerIn: parent
        frameVisible: false
        navigationBarVisible: false
        weekNumbersVisible: false
        minimumDate: new Date(2015, 0, 1);
        maximumDate: new Date(2025, 0, 1);

        anchors{
            fill: parent
        }
        onClicked:
        {
            //m_calendar.visible = false;
        }

        style: CalendarStyle {
            gridVisible: false

            background: Image {//日历背景
                id: bg
                anchors.fill: parent
                //                    source: "./image/dataRect.png"
            }

            dayOfWeekDelegate://周的显示
                              Rectangle{
                id: rec1
                color: "transparent"
                height: 20

                Text {
                    id: weekTxt
                    font.pixelSize: 15
                    text:Qt.locale().dayName(styleData.dayOfWeek, control.dayOfWeekFormat)//转换为自己想要的周的内容的表达
                    anchors.centerIn: rec1
                    color: styleData.selected?"green":"gray"
                    font.family: "Microsoft YaHei"
                }
            }

            navigationBar:

                Rectangle {//导航控制栏，控制日期上下选择等
                color: "transparent"
                height: 40
            }

            dayDelegate://显示日期
                        Rectangle{
                color: "transparent"

                Image
                {
                    id: day_bg
                    height: 28
                    width: 28
                    anchors.centerIn: parent
                    //                                        anchors.centerIn: parent
                    source: styleData.selected ? "images/wvga/system/current-dat-bg.png" : ""
                }

                Label {
                    id: m_label
                    text: styleData.date.getDate()
                    font.pixelSize: 15
                    font.family: "Microsoft YaHei"
                    anchors.centerIn: parent
                    color: styleData.selected ? "yellow" :  (styleData.visibleMonth && styleData.valid ? "lightblue" : "grey");
                }
            }
        }
    }
}
