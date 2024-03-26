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

#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>
#include <QQmlApplicationEngine>
class QTranslator;

class Translator : public QObject
{
    Q_OBJECT
public:
    QQmlApplicationEngine *m_engine;
    static Translator* getInstance();
    void set_QQmlEngine(QQmlApplicationEngine *engine);

    QString m_current_language;
private:
    explicit Translator(QObject *parent = 0);
    ~Translator();

signals:
    void languageChanged(QString lang);

public slots:
    void loadLanguage(QString lang);
    QString get_current_language();

private:
    QTranslator*  m_translator;
};

#endif // TRANSLATOR_H
