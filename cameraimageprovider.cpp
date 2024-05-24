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

#include "cameraimageprovider.h"
#include <QDebug>
CameraImageProvider::CameraImageProvider(): QQuickImageProvider(QQuickImageProvider::Image)
{

}

QImage CameraImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    return this->img;
}

QPixmap CameraImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    return QPixmap::fromImage(this->img);
}

void showImage::captureImg(QString path)
{
    QDir dir;
    if(!dir.exists("/usr/share/myir/Capture"))
        dir.mkpath("/usr/share/myir/Capture");
    bool ret = this->img.save(path + ".jpg");
    if(ret){
        qDebug()<<"save img:"<<path + ".jpg" << "succ";
        emit callQmlSavePath(path + ".jpg");
    }
    else
        qDebug()<<"save img:"<<path + ".jpg" << "fail";
}

showImage::showImage(QObject *parent) : QObject(parent)
{
    thread = new YUYVQThread;
    cameraImageProvider = new CameraImageProvider;
    thread->set_device(QCameraInfo::defaultCamera().deviceName());
    thread->show_flag = true;
    thread->start();
    connect(thread,SIGNAL(sign_img(QImage)),this,SLOT(slot_img(QImage)));
}

void showImage::slot_img(QImage img)
{
    this->img = img;
    cameraImageProvider->img = img;
    emit callQmlRefreshImage();
}
