/******************************************************************************
 * @文件名     VideoThread.h
 * @功能       读取视频图像数据线程，在线程中解码视频
 *
 * @开发者     mhf
 * @邮箱       1603291350@qq.com
 * @时间       2022/09/15
 * @备注
 *****************************************************************************/
#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QElapsedTimer>
#include <QThread>
#include <QTime>

class VideoDecode;

class VideoThread : public QThread
{
    Q_OBJECT
public:
    enum PlayState      // 视频播放状态
    {
        play,
        end
    };
public:
    explicit VideoThread(QObject *parent = nullptr);
    ~VideoThread() override;

    void open(const QString& url = QString());  // 打开视频
    void pause(bool flag);                      // 暂停视频
    void close();                               // 关闭视频
    void seek(int value);
    const QString& url();                       // 获取打开的视频地址
    bool hasVideo();
    const qint64& pts();
    const qint64& duration();

protected:
    void run() override;

signals:
    void sign_img(const QImage &image);      // 将读取到的视频图像发送出去
    void sign_play();
    void sign_pause();
    void sign_stop();
    void sign_progress(int duration,int position);
    void sign_end();

private:
    VideoDecode* m_videoDecode = nullptr;       // 视频解码类
    QString m_url;                              // 打开的视频地址
    bool m_play   = false;                      // 播放控制
    bool m_pause  = false;                      // 暂停控制
    QElapsedTimer m_etime1;                     // 控制视频播放速度（更精确，但不支持视频后退）
    QTime         m_etime2;                     // 控制视频播放速度（支持视频后退）
    bool has_video;
};

#endif // VIDEOTHREAD_H
