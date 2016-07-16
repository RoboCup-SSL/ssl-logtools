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

#ifndef RECORDER_H
#define RECORDER_H

#include "network.h"
#include "logfile.h"
#include <QThread>

class Recorder : public QObject
{
    Q_OBJECT

public:
    Recorder();
    ~Recorder();

public:
    bool start(const QString& filename, bool compress = false, int formatVersion = LogFile::DEFAULT_FILE_FORMAT_VERSION);
    void stop();

private slots:
    void recordRefereePacket(const QByteArray& data, qint64 time);
    void recordVisionPacket(const QByteArray& data, qint64 time);
    void recordLegacyVisionPacket(const QByteArray& data, qint64 time);

private:
    LogFile* m_logFile;
    Network* m_referee;
    Network* m_vision;
    Network* m_legacyVision;
    QThread* m_networkThread;
    long m_visionFrames;
    long m_legacyVisionFrames;
    long m_refereeFrames;
};

#endif // RECORDER_H
