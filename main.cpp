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
***********************************************************************/

#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFontDatabase>
#include <QFont>
#include <QTimer>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include "qmlplot.h"
#include "common.h"
#include "myfunction.h"
#include "translator.h"
#include "mvideooutput.h"
#include "qmlprocess.h"
#include "ChargeManage.h"
#include "Charge104.h"
#include "ClearCache.h"
#include "cameraimageprovider.h"
#include "videoimageprovider.h"
#include <QTime>

using namespace std;
using namespace MQP;

void iconFontInit()
{
    //
    int fontId_digital = QFontDatabase::addApplicationFont(":/fonts/DIGITAL/DS-DIGIB.TTF");
    int fontId_fws = QFontDatabase::addApplicationFont(":/fonts/fontawesome-webfont.ttf");
    QString fontName_fws = QFontDatabase::applicationFontFamilies(fontId_fws).at(0);
    QFont iconFont_fws;
    iconFont_fws.setFamily(fontName_fws);
    QApplication::setFont(iconFont_fws);
    iconFont_fws.setPixelSize(20);
}

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QApplication app(argc, argv);

    app.setOrganizationName("MYiR_Electronics");
    app.setApplicationName("MEasy_HMI");
    app.setApplicationVersion("V2.0");

    QDir dir;
    if(!dir.exists("/usr/share/myir"))
        dir.mkpath("/usr/share/myir");
    QFile file(":/ecg/ecg.dat");
    file.copy(":/ecg/ecg.dat","/usr/share/myir/ecg.dat");

    QQmlApplicationEngine engine;

    qmlRegisterType<QmlProcess>("mprocess", 1, 0, "QmlProcess");
    qmlRegisterType<GetSystemInfo>("GetSystemInfoAPI", 1, 0, "GetSystemInfo");
    qmlRegisterType<CustomPlotItem>("CustomPlot", 1, 0, "CustomPlotItem");
    qmlRegisterType<MyFunction>("MyFunction.module", 1, 0, "MyFunction");
    qmlRegisterType<ChargeManage>("ChargeManage",1,0,"ChargeManage");
    qmlRegisterType<Charge104>("Charge104",1,0,"Charge104");
    qmlRegisterType<MVideoOutput>("mvideooutput",1,0, "MVideoOutput");

    Translator *translator = Translator::getInstance();
    translator->set_QQmlEngine(&engine);
    engine.rootContext()->setContextProperty("translator",
                                             translator);

    showImage *show_image = new showImage;
    engine.rootContext()->setContextProperty("show_image",show_image);
    engine.addImageProvider(QLatin1String("cameraImageProvider"), show_image->cameraImageProvider);

    videoImage *video_image = new videoImage;
    engine.rootContext()->setContextProperty("video_image",video_image);
    engine.addImageProvider(QLatin1String("videoImageProvider"), video_image->videoImageProvider);

    ClearCache *clear_cache = new ClearCache;

    //font icon init
    iconFontInit();

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
