/***********************************************************************
 *  This file is part of MXAPP2

    Copyright (C) 2020-2024 XuMR <2801739946@qq.com>

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

#ifndef QMLPROCESS_H
#define QMLPROCESS_H

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QString>
#include <QStringList>

namespace MQP{
    class Private;
    class QmlProcess;


class QmlProcess : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QmlProcess)
    Q_PROPERTY(QString shell READ shell WRITE setShell NOTIFY shellChanged)
    Q_PROPERTY(QString command READ command WRITE setCommand NOTIFY commandChanged)
public:
    QmlProcess(QObject *parent = 0);
    ~QmlProcess();

    const QString &shell() const;
    const QString &command() const;

    Q_INVOKABLE QString readAllStandardError();
    Q_INVOKABLE QString readAllStandardOutput();

public slots:
    void doSometing(){
        qDebug() << "QmlProces::doSomething() is called.";
    }
    void setShell(const QString &shell);
    void setCommand(const QString &command);

    void start();
    void terminate();
    void kill();

signals:
    void begin();
    void shellChanged(const QString &shell);
    void commandChanged(const QString &command);

    void finished(int exitCode, QProcess::ExitStatus exitStatus);
    void readyReadStandardError(QPrivateSignal);
    void readyReadStandardOutput(QPrivateSignal);
    void started(QPrivateSignal);

private:
    Private *d;
};

class Private:public QProcess{
    Q_OBJECT
    public:
        Private(QmlProcess *parent);
        QString shell;
        QString command;
    private:
        QmlProcess *q;
    };
};
#endif // QMLPROCESS_H
