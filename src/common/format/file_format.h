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

#ifndef FILE_FORMAT_H
#define FILE_FORMAT_H

#include "message_type.h"
#include <QDataStream>

class FileFormat
{
public:
    FileFormat(int version) : m_version(version) {}

public:
    int version() { return m_version; }

public:
    virtual void writeHeaderToStream(QDataStream& stream) = 0;
    virtual bool readHeaderFromStream(QDataStream& stream) = 0;

public:
    virtual void writeMessageToStream(QDataStream& stream, const QByteArray& data, qint64 time, MessageType type) = 0;
    virtual bool readMessageFromStream(QDataStream& stream, QByteArray& data, qint64& time, MessageType& type) = 0;

private:
    int m_version;
};

#endif // FILE_FORMAT_H
