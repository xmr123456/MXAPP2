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

#ifndef CLEARCACHE_H
#define CLEARCACHE_H

#include <QObject>
#include <QDateTime>
#include <QTimer>
class ClearCache : public QObject
{
    Q_OBJECT
public:
    explicit ClearCache(QObject *parent = nullptr);
    QTimer *t1;
    QDateTime last_time = QDateTime::currentDateTime();
    void clear_memory();

};

#endif // CLEARCACHE_H
