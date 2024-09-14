#include "videoimageprovider.h"
#include <QDebug>
VideoImageProvider::VideoImageProvider(): QQuickImageProvider(QQuickImageProvider::Image)
{

}

QImage VideoImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    return this->img;
}

QPixmap VideoImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    return QPixmap::fromImage(this->img);
}

void videoImage::seek(int value)
{
    thread->seek(value);
}

void videoImage::pause(bool flag)
{
    thread->pause(flag);
}

void videoImage::stop()
{
    thread->close();
}

void videoImage::open(QString filepath)
{
    thread->open(filepath);
    qDebug()<<filepath;
}

int videoImage::duration()
{
    return thread->duration();
}

int videoImage::position()
{
    return thread->pts();
}

QString videoImage::get_url()
{
    return thread->url();
}

bool videoImage::hasVideo()
{
    return thread->hasVideo();
}

videoImage::videoImage(QObject *parent)
{
    thread = new VideoThread;
    videoImageProvider = new VideoImageProvider;
    connect(thread,SIGNAL(sign_img(QImage)),this,SLOT(slot_img(QImage)));
    connect(thread,SIGNAL(sign_progress(int,int)),this,SIGNAL(callQmlRefreshProgress(int,int)));
    connect(thread,SIGNAL(sign_end()),this,SIGNAL(callQmlPlayEnd()));
    connect(thread,SIGNAL(sign_play()),this,SIGNAL(callQmlPlay()));
    connect(thread,SIGNAL(sign_pause()),this,SIGNAL(callQmlPause()));
//    connect(thread,SIGNAL(sign_stop()),this,SIGNAL(callQmlStop()));
}

void videoImage::slot_img(QImage img)
{
    this->img = img;
    videoImageProvider->img = img;
    emit callQmlRefreshImage();
}
