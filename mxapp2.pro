QT += qml quick core gui printsupport testlib quickcontrols2 multimedia multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
CONFIG += qcamera-v4l2
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Charge104.cpp \
    ChargeManage.cpp \
    ClearCache.cpp \
    iec104_class.cpp \
    logmsg.cpp \
    main.cpp \
    qcustomplot.cpp \
    qiec104.cpp \
    qmlplot.cpp \
    common.cpp \
    myfunction.cpp \
    qmlprocess.cpp \
    translator.cpp \
    mvideooutput.cpp \
    videowidgetsurface.cpp \
    yuyv_qthread.cpp \
    cameraimageprovider.cpp \
    videodecode.cpp \
    videothread.cpp \
    videoimageprovider.cpp

RESOURCES += qml.qrc
CONFIG += disable-desktop
static {
    QT += svg
    QTPLUGIN += qtvirtualkeyboardplugin
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Charge104.h \
    ChargeManage.h \
    ClearCache.h \
    iec104_class.h \
    iec104_types.h \
    logmsg.h \
    qcustomplot.h \
    qiec104.h \
    qmlplot.h \
    common.h \
    myfunction.h \
    qmlprocess.h \
    translator.h \
    mvideooutput.h \
    videowidgetsurface.h \
    yuyv_qthread.h \
    cameraimageprovider.h \
    videodecode.h \
    videothread.h \
    videoimageprovider.h
TRANSLATIONS = languages/language_zh.ts \
               languages/language_en.ts

LIBS += -L $$PWD/ffmpeg/usr/lib/libavfilter.so
LIBS += -L $$PWD/ffmpeg/usr/lib/libavcodec.so
LIBS += -L $$PWD/ffmpeg/usr/lib/libavformat.so
LIBS += -L $$PWD/ffmpeg/usr/lib/libswscale.so
LIBS += -L $$PWD/ffmpeg/usr/lib/libavutil.so
LIBS += -L $$PWD/ffmpeg/usr/lib/libswresample.so

INCLUDEPATH += $$PWD/ffmpeg/usr/include
DEPENDPATH += $$PWD/ffmpeg/usr/include
