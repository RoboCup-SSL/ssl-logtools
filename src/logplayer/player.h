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

#ifndef PLAYER_H
#define PLAYER_H

#include "common/logfile.h"
#include "common/network.h"
#include <QThread>

class Player : public QThread
{
    Q_OBJECT

public:
    struct Frame {
        qint64 time;
        MessageType type;
        QByteArray data;
    };

public:
    Player();
    ~Player();

signals:
    void positionChanged(int frame, double time);
    void finished();

public:
    bool load(const QString& filename, int& maxFrame, double& duration);
    bool start(int position);
    void stop();
    void pause(bool pause);
    bool good();

private:
    void run();
    void sendMessage(const Frame* packet);

private:
    QList<Frame*> packets;
    Network* m_referee;
    Network* m_vision;
    Network* m_legacyVision;
    bool m_mayRun;
    int m_currentFrame;
};

#endif // PLAYER_H
