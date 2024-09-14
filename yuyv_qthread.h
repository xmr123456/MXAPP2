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

#ifndef YUYV_QTHREAD_H
#define YUYV_QTHREAD_H

#include <QThread>
#include <QDebug>
#include <QWidget>
#include <QCoreApplication>
#include <QMutex>
#include <QImage>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>
#include <linux/videodev2.h>
#include <stdbool.h>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <linux/videodev2.h>

#define FRAMEBUFFER_COUNT 4

#define CLEAR(x) memset(&(x), 0, sizeof(x))
#define NUM_PLANES 1

/* 命名空间 */
using namespace std;

struct usb_buffer{
    void *  start;
    size_t  length;
};

class YUYVQThread : public QThread{
    Q_OBJECT
public:
    explicit YUYVQThread(QWidget *parent = nullptr);
    ~YUYVQThread();

    /* 视频显示标志位信号 */
    bool show_flag;

    /* v4l2结构体 */
    /*
    struct v4l2_format              fmt;
    struct v4l2_buffer              buf;
    struct v4l2_requestbuffers      reqbuf;
    struct v4l2_capability          cap;
    enum v4l2_buf_type              type;
    fd_set                          fds;
    struct timeval                  tv;
    int                             ret, v4l2_fd;    //返回值ret和v4l2描述符v4l2_fd
    unsigned int                    n_buffers;
    QString                         dev_name;
    */

    int PIXWIDTH = 640;
    int PIXHEIGHT = 480;

    enum vb2_memory {
        VB2_MEMORY_UNKNOWN      = 0,
        VB2_MEMORY_MMAP         = 1,
    };

    char dev_name[20];
    int fd = -1;

    unsigned int n_buffers;

    int status;
    int ret;

    struct usb_buffer usb_buffers[FRAMEBUFFER_COUNT];

    QImage m_img;

    /* QThread 虚函数run */
    void run();

    int handleData(unsigned char *bufData);
    void yuv_to_rgb(unsigned char* yuv, unsigned char *rgb);
    void set_device(QString device);
    int xioctl(int fh, int request, void *arg);

    void stop_capturing();
    int mainloop(void);
    int start_capturing();
    void uninit_device();
    int init_device();
    void close_device(void);
    int open_device(void);
signals:
    void sign_img(QImage image);
};
#endif // YUYV_QTHREAD_H
