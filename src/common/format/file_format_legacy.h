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

#ifndef FILE_FORMAT_LEGACY_H
#define FILE_FORMAT_LEGACY_H

#include "file_format.h"

class FileFormatLegacy : public FileFormat
{
public:
    FileFormatLegacy();
    ~FileFormatLegacy();

public:
    void writeHeaderToStream(QDataStream& stream);
    bool readHeaderFromStream(QDataStream& stream);

public:
    void writeMessageToStream(QDataStream& stream, const QByteArray& data, qint64 time, MessageType type);
    bool readMessageFromStream(QDataStream& stream, QByteArray& data, qint64& time, MessageType& type);
};

#endif // FILE_FORMAT_LEGACY_H
