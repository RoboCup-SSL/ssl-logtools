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

#ifndef LOGFILE_H
#define LOGFILE_H

#include "format/file_format.h"
#include <QMap>
#include <QFile>

class QtIOCompressor;

class LogFile
{
public:
    LogFile(const QString& filename, bool compressed = false, int formatVersion = DEFAULT_FILE_FORMAT_VERSION);
    ~LogFile();

public:
    bool openRead();
    bool openWrite();
    void close();

public:
    void saveMessage(const QByteArray& data, qint64 time, MessageType type);
    bool readMessage(QByteArray& data, qint64& time, MessageType& type);

private:
    bool addFormat(FileFormat* format);

private:
    QString m_filename;
    bool m_compressed;
    int m_formatVersion;
    QMap<int, FileFormat*> m_formatMap;
    QIODevice* m_io;
    QFile* m_file;
    QtIOCompressor* m_compressor;

public:
    static const int DEFAULT_FILE_FORMAT_VERSION = 1;
};

#endif // LOG_FILE_H
