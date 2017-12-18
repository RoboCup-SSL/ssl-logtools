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

#include "recorder.h"
#include "protobuf/ssl_referee.pb.h"
#include "protobuf/messages_robocup_ssl_wrapper.pb.h"
#include "protobuf/messages_robocup_ssl_wrapper_legacy.pb.h"
#include <iostream>
#include <iomanip>

Recorder::Recorder() :
    m_logFile(NULL),
    m_referee(NULL),
    m_vision(NULL),
    m_legacyVision(NULL),
    m_networkThread(NULL)
{
    m_networkThread = new QThread(this);
}

Recorder::~Recorder()
{
    stop();

    std::cout << "\r" << "Recorded " << m_visionFrames << " vision packets, " << m_legacyVisionFrames << " legacy vision packets and " << m_refereeFrames << " referee packets." << std::endl;
    std::cout << "Finished recording." << std::endl;
}

bool Recorder::start(const QString& filename, bool compress, int formatVersion)
{
    m_visionFrames = 0;
    m_legacyVisionFrames = 0;
    m_refereeFrames = 0;

    // open writeable log file
    Q_ASSERT(m_logFile == NULL);
    m_logFile = new LogFile(filename, compress, formatVersion);
    if (!m_logFile->openWrite()) {
        return false;
    }

    // create referee socket
    Q_ASSERT(m_referee == NULL);
    m_referee = new Network(QHostAddress("224.5.23.1"), 10003, 0);
    m_referee->moveToThread(m_networkThread);
    QObject::connect(m_networkThread, SIGNAL(started()), m_referee, SLOT(connect()));
    QObject::connect(m_networkThread, SIGNAL(finished()), m_referee, SLOT(disconnect()));
    QObject::connect(m_referee, SIGNAL(gotPacket(QByteArray, qint64)), SLOT(recordRefereePacket(QByteArray, qint64)));

    // create vision socket
    Q_ASSERT(m_vision == NULL);
    m_vision = new Network(QHostAddress("224.5.23.2"), 10006, 0);
    m_vision->moveToThread(m_networkThread);
    QObject::connect(m_networkThread, SIGNAL(started()), m_vision, SLOT(connect()));
    QObject::connect(m_networkThread, SIGNAL(finished()), m_vision, SLOT(disconnect()));
    QObject::connect(m_vision, SIGNAL(gotPacket(QByteArray, qint64)), SLOT(recordVisionPacket(QByteArray, qint64)));

    // create legacy vision socket
    Q_ASSERT(m_legacyVision == NULL);
    m_legacyVision = new Network(QHostAddress("224.5.23.2"), 10005, 0);
    m_legacyVision->moveToThread(m_networkThread);
    QObject::connect(m_networkThread, SIGNAL(started()), m_legacyVision, SLOT(connect()));
    QObject::connect(m_networkThread, SIGNAL(finished()), m_legacyVision, SLOT(disconnect()));
    QObject::connect(m_legacyVision, SIGNAL(gotPacket(QByteArray, qint64)), SLOT(recordLegacyVisionPacket(QByteArray, qint64)));

    m_networkThread->start();

    std::cout << "Started recording to \"" << filename.toStdString() << "\" using file format version " << formatVersion << "." << std::endl;

    return true;
}

void Recorder::stop()
{
    m_networkThread->quit();
    m_networkThread->wait();

    delete m_referee;
    m_referee = NULL;

    delete m_vision;
    m_vision = NULL;

    delete m_legacyVision;
    m_legacyVision = NULL;

    delete m_logFile;
    m_logFile = NULL;
}

void Recorder::recordRefereePacket(const QByteArray& data, qint64 time)
{
    // check if packet is valid
    SSL_Referee packet;
    if (!packet.ParseFromArray(data.data(), data.size())) {
        std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(3) << (double) time / 1e9 << ": Error failed to parse referee packet!" << std::endl << std::flush;
        return;
    }

    // save packet to log file
    if (m_logFile == NULL) {
        return;
    }
    m_logFile->saveMessage(data, time, MESSAGE_SSL_REFBOX_2013);

    m_refereeFrames++;
    std::cout << "\r" << "Recorded " << m_visionFrames << " vision packets, " << m_legacyVisionFrames << " legacy vision packets and " << m_refereeFrames << " referee packets." << std::flush;
}

void Recorder::recordVisionPacket(const QByteArray& data, qint64 time)
{
    // check if packet is valid
    SSL_WrapperPacket packet;
    if (!packet.ParseFromArray(data.data(), data.size())) {
        std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(3) << (double) time / 1e9 << ": Error failed to parse legacy vision packet!" << std::endl << std::flush;
        return;
    }

    // save packet to logfile
    if (m_logFile == NULL) {
        return;
    }
    m_logFile->saveMessage(data, time, MESSAGE_SSL_VISION_2014);
    m_visionFrames++;
    std::cout << "\r" << "Recorded " << m_visionFrames << " vision packets, " << m_legacyVisionFrames << " legacy vision packets and " << m_refereeFrames << " referee packets." << std::flush;
}

void Recorder::recordLegacyVisionPacket(const QByteArray& data, qint64 time)
{
    // check if packet is valid
    RoboCup2014Legacy::Wrapper::SSL_WrapperPacket packet;
    if (!packet.ParseFromArray(data.data(), data.size())) {
        std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(3) << (double) time / 1e9 << ": Error failed to parse vision packet!" << std::endl << std::flush;
        return;
    }

    // save packet to logfile
    if (m_logFile == NULL) {
        return;
    }
    m_logFile->saveMessage(data, time, MESSAGE_SSL_VISION_2010);
    m_legacyVisionFrames++;
    std::cout << "\r" << "Recorded "  << m_visionFrames << " vision packets, " << m_legacyVisionFrames << " legacy vision packets and " << m_refereeFrames << " referee packets." << std::flush;
}
