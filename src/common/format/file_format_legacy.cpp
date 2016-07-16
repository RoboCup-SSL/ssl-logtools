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

#include "file_format_legacy.h"

FileFormatLegacy::FileFormatLegacy() :
    FileFormat(0)
{

}

FileFormatLegacy::~FileFormatLegacy()
{

}

void FileFormatLegacy::writeHeaderToStream(QDataStream& stream)
{
    stream << QString("SSL_LOG_FILE");
    stream << version();
}

bool FileFormatLegacy::readHeaderFromStream(QDataStream& stream)
{
    QString name;
    stream >> name;

    int format;
    stream >> format;

    if (name == "SSL_LOG_FILE" && format == version()) {
        return true;
    }

    return false;
}

void FileFormatLegacy::writeMessageToStream(QDataStream& stream, const QByteArray& data, qint64 time, MessageType type)
{
    stream << time;
    stream << qCompress(data);
}

bool FileFormatLegacy::readMessageFromStream(QDataStream& stream, QByteArray& data, qint64& time, MessageType& type)
{
    type = MESSAGE_UNKNOWN;

    stream >> time;
    QByteArray compressedPacket;
    stream >> compressedPacket;
    data = qUncompress(compressedPacket);

    return true;
}
