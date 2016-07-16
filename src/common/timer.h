/***************************************************************************
 *   Copyright (c) 2013 Robotics Erlangen e.V.                             *
 *   http://www.robotics-erlangen.de/                                      *
 *   info@robotics-erlangen.de                                             *
 *                                                                         *
 *   This file may be licensed under the terms of the GNU General Public   *
 *   License Version 3, as published by the Free Software Foundation.      *
 *   You can find it here: http://www.gnu.org/licenses/gpl.html            *
 *                                                                         *
 ***************************************************************************/

#ifndef TIMER_H
#define TIMER_H

#include <QtGlobal>

class Timer
{
public:
    Timer();

public:
    void setScaling(double scaling);
    void reset();
    qint64 currentTime() const;

public:
    static qint64 systemTime();

private:
    double m_scaling;
    qint64 m_start;
};

#endif // TIMER_H
