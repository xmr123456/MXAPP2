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

#include "yuyv_qthread.h"
#include <unistd.h>

YUYVQThread::YUYVQThread(QWidget *parent):QThread(parent){
    show_flag = false;
}

YUYVQThread::~YUYVQThread()
{
}

int YUYVQThread::xioctl(int fh, int request, void *arg)
{
    int r;
    int i = 0;
    do {
        r = ioctl(fh, request, arg);
    } while (-1 == r && EINTR == errno);

    return r;
}

void YUYVQThread::stop_capturing()
{
    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type)) {
        qDebug()<<"close stream failed";
    }
    else{
        qDebug()<<"close stream succ";
    }
}

int YUYVQThread::mainloop(void)
{
    fd_set fds;
    struct timeval tv;
    int r;

    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    /* Timeout. */
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    r = select(fd + 1, &fds, NULL, NULL, &tv);

    if (-1 == r) {
        if (EINTR == errno){
            fprintf(stderr, "select error\n");
        }
    }
    else if (0 == r) {
        fprintf(stderr, "select timeout\n");
    }
    return r;
}

int YUYVQThread::start_capturing()
{
    printf("start camera");
    enum v4l2_buf_type type;

    /* 入队 */
    for(unsigned int i=0;i<n_buffers;i++){
        struct v4l2_buffer buf;
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        if (0 > ioctl(fd, VIDIOC_QBUF, &buf)) {
            qDebug("入队失败\n");
            return -1;
        }
    }

    /* 开启视频流 */
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (0 > ioctl(fd, VIDIOC_STREAMON, &type)) {
        qDebug("open stream failed\n");
        return -1;
    }
    qDebug()<<"open stream succ";

    return 0;
}

void YUYVQThread::uninit_device()
{
    unsigned int i;
    for(i = 0;i < n_buffers; i++)
    {
        if(-1 == munmap(usb_buffers[i].start, usb_buffers[i].length)){
            qDebug()<<"munmap fail";
            return;
        }
    }
    qDebug()<<"munmap succ";
}

int YUYVQThread::init_device()
{
    struct v4l2_format fmt;
    struct v4l2_capability cap;

    if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
        fprintf(stderr, "VIDIOC_QUERYCAP: error - %d\n", errno);
        return -1;
    }

    /* 设置采集格式 */
    if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)){
        fprintf(stderr, "%s is no video capture device\n",dev_name);
        return -1;
    }

    CLEAR(fmt);
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1  == ioctl(fd, VIDIOC_G_FMT, &fmt)) {
        qDebug("get format failed\n");
        return -1;
    }
    qDebug()<<"get format succ";

    qDebug()<<"fmt.type:"<<fmt.type;
    qDebug()<<"fmt.fmt.pix.width:"<<fmt.fmt.pix.width;
    qDebug()<<"fmt.fmt.pix.height:"<<fmt.fmt.pix.height;
    qDebug()<<"fmt.fmt.pix.pixelformat:"<<fmt.fmt.pix.pixelformat;
    qDebug()<<"fmt.fmt.pix.colorspace:"<<fmt.fmt.pix.colorspace;
    qDebug()<<"fmt.fmt.pix.quantization:"<<fmt.fmt.pix.quantization;
    qDebug()<<"fmt.fmt.pix.xfer_func:"<<fmt.fmt.pix.xfer_func;
    qDebug()<<"fmt.fmt.pix.ycbcr_enc:"<<fmt.fmt.pix.ycbcr_enc;

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = PIXWIDTH;
    fmt.fmt.pix.height = PIXHEIGHT;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB565;

    if (ioctl(fd, VIDIOC_S_FMT, &fmt) == -1) {
        qDebug("set format failed\n");
        return -1;
    }
    qDebug()<<"set format succ";

    /* 获取实际的帧宽高度 */
    struct v4l2_requestbuffers reqbuf;
    CLEAR(reqbuf);
    reqbuf.count = FRAMEBUFFER_COUNT;       //帧缓冲的数量
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.memory = V4L2_MEMORY_MMAP;
    if (0 > ioctl(fd, VIDIOC_REQBUFS, &reqbuf)) {
        qDebug("request buffer failed\n");
        return -1;
    }
    qDebug()<<"request buffer success";

    /* 建立内存映射 */
    for(n_buffers = 0;n_buffers < FRAMEBUFFER_COUNT;n_buffers++){
        struct v4l2_buffer buf;
        CLEAR(buf);
        buf.index = n_buffers;
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        if(0 > ioctl(fd, VIDIOC_QUERYBUF, &buf)){
            qDebug("VIDIOC_QUERYBUF failed\n");
            return -1;
        }
        qDebug()<<"VIDIOC_QUERYBUF succ";

        usb_buffers[n_buffers].length = buf.length;
        usb_buffers[n_buffers].start = mmap(NULL,buf.length,PROT_READ | PROT_WRITE, MAP_SHARED,fd, buf.m.offset);
        if (MAP_FAILED == usb_buffers[n_buffers].start) {
            qDebug("mmap error\n");
            return -1;
        }
        qDebug()<<"mmap succ";
    }
    return 0;
}

void YUYVQThread::close_device(void)
{
    if (-1 == close(fd)) {
        qDebug("Failed to close device\n");
    }
    fd = -1;
}

int YUYVQThread::open_device(void)
{
    fd = open(dev_name, O_RDWR, 0);

    if (fd == -1) {
        fprintf(stderr, "Cannot open %s\n", dev_name);
        return -1;
    }
    return 0;
}

void YUYVQThread::run()
{
    if(open_device() == -1){
        return;
    }
    if(init_device() == -1){
        return;
    }
    if(start_capturing() == -1){
        return;
    }

    while(show_flag){
        ret = mainloop();
        if(ret == 0){
            qDebug("select I/O timeout\n");
            continue;
        }
        else if(ret == -1){
            qDebug("select I/O error\n");
            continue;
        }
        //帧数据处理
        unsigned char onebuf[640*480*3];
        ret = handleData(onebuf);
        if(ret == -1){
            continue;
        }
        QImage img = QImage(onebuf,640,480,QImage::Format_RGB16);
        if(!img.isNull())
            emit sign_img(img);
    }

    stop_capturing();
    uninit_device();
    close_device();

    qDebug()<<"thread exit";
}

int YUYVQThread::handleData(unsigned char *bufData)
{
    struct v4l2_buffer buf;
    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    /* 出队 */
    if(0 > ioctl(fd,VIDIOC_DQBUF,&buf)){
        qDebug("出队失败\n");
        return -1;
    }

    //数据处理
    memcpy(bufData, usb_buffers[buf.index].start, buf.bytesused);

    /* 再次入队*/
    if (0 > ioctl(fd, VIDIOC_QBUF, &buf)) {
        qDebug("入队失败\n");
        return -1;
    }
    return 0;
}

void YUYVQThread::set_device(QString device)
{
    strcpy(dev_name,device.toUtf8().data());
    strcpy(dev_name,"/dev/x_video");
}
