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

#include "player.h"
#include "common/timer.h"
#include "protobuf/ssl_referee.pb.h"
#include "protobuf/messages_robocup_ssl_wrapper_legacy.pb.h"
#include <iostream>
#include <QFileInfo>

Player::Player() :
    m_referee(NULL),
    m_vision(NULL),
    m_legacyVision(NULL)
{
    // create referee socket
    Q_ASSERT(m_referee == NULL);
    m_referee = new Network(QHostAddress("224.5.23.1"), 0, 10003);
    m_referee->connect();

    // create vision socket
    Q_ASSERT(m_vision == NULL);
    m_vision = new Network(QHostAddress("224.5.23.2"), 0, 10006);
    m_vision->connect();

    // create legacy vision socket
    Q_ASSERT(m_legacyVision == NULL);
    m_legacyVision = new Network(QHostAddress("224.5.23.2"), 0, 10005);
    m_legacyVision->connect();
}

Player::~Player()
{
    stop();

    delete m_referee;
    m_referee = NULL;

    delete m_vision;
    m_vision = NULL;

    delete m_legacyVision;
    m_legacyVision = NULL;

    qDeleteAll(packets);
    packets.clear();
}

bool Player::load(const QString& filename, int& maxFrame, double& duration) {
    QFileInfo fileInfo(filename);

    bool compressed = false;

    if (fileInfo.suffix() == "gz") {
        compressed = true;
    }

    LogFile file(filename, compressed);

    if (!file.openRead()) {
        return false;
    }

    qDeleteAll(packets);
    packets.clear();

    for (;;) {
        Frame* packet = new Frame;

        if (!file.readMessage(packet->data, packet->time, packet->type)) {
            delete packet;
            break;
        }

        packets.append(packet);
    }

    maxFrame = packets.size() - 1;
    duration = packets.last()->time - packets.first()->time;
    return true;
}

bool Player::start(int position)
{  
    if (position > packets.size() - 1) {
        return false;
    }

    m_currentFrame = position;

    m_mayRun = true;
    QThread::start();

    return true;
}

void Player::stop()
{
    m_mayRun = false;
    wait();
}

bool Player::good()
{
    return packets.size() > 0;
}

void Player::sendMessage(const Frame* packet)
{
    RoboCup2014Legacy::Wrapper::SSL_WrapperPacket legacyVisionPacket;
    SSL_Referee refereePacket;

    if (packet->type == MESSAGE_BLANK) {
        // ignore
    } else if (packet->type == MESSAGE_UNKNOWN) {
        // OK, let's try to figure this out by parsing the message
        if (refereePacket.ParseFromArray(packet->data.data(), packet->data.size())) {
            m_referee->writeData(packet->data);
        } else if (legacyVisionPacket.ParseFromArray(packet->data.data(), packet->data.size())) {
            m_legacyVision->writeData(packet->data);
        } else {
            std::cout << "Error unsupported or corrupt packet found in log file!" << std::endl;
        }
    } else if (packet->type == MESSAGE_SSL_VISION_2010) {
        m_legacyVision->writeData(packet->data);
    } else if (packet->type == MESSAGE_SSL_REFBOX_2013) {
        m_referee->writeData(packet->data);
    } else if (packet->type == MESSAGE_SSL_VISION_2014) {
        m_vision->writeData(packet->data);
    } else {
        std::cout << "Error unsupported message type found in log file!" << std::endl;
    }
}

void Player::run()
{
    sendMessage(packets.at(m_currentFrame));

    const qint64 startTime = Timer::systemTime();
    const qint64 referenceTime = packets.at(m_currentFrame)->time;

    while (m_mayRun && ++m_currentFrame < packets.size() && this->isRunning()) {
        Frame* packet = packets.at(m_currentFrame);

        qint64 sleepTime = ((packet->time - referenceTime) - (Timer::systemTime() - startTime)) / 1000;

        if (sleepTime > 0) {
            QThread::currentThread()->usleep(sleepTime);
        }

        sendMessage(packet);

        emit positionChanged(m_currentFrame, packet->time - packets.first()->time);
    }

    emit finished();
}
