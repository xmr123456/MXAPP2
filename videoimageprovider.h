#ifndef VIDEOIMAGEPROVIDER_H
#define VIDEOIMAGEPROVIDER_H

#include <QWidget>
#include <QQuickImageProvider>
#include <QImage>
#include <QCameraInfo>
#include <QPixmap>
#include <QQmlEngine>
#include <QDir>
#include "videothread.h"

class VideoImageProvider : public QQuickImageProvider
{
public:
    VideoImageProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
    QImage img;
};

class videoImage : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE void seek(int value);
    Q_INVOKABLE void pause(bool flag);
    Q_INVOKABLE void stop();
    Q_INVOKABLE void open(QString filepath);
    Q_INVOKABLE int duration();
    Q_INVOKABLE int position();
    Q_INVOKABLE QString get_url();
    Q_INVOKABLE bool hasVideo();

    videoImage(QObject *parent = nullptr);

    VideoThread *thread;
    QImage img;
    VideoImageProvider *videoImageProvider;
signals:
    void callQmlRefreshImage();
    void callQmlRefreshProgress(int duration,int position);
    void callQmlPlayEnd();
    void callQmlPlay();
    void callQmlPause();
    void callQmlStop();
private slots:
    void slot_img(QImage img);
};

#endif // VIDEOIMAGEPROVIDER_H
