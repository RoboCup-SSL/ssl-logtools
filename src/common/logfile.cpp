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

#include "logfile.h"
#include "qt/qtiocompressor.h"
#include "format/file_format_legacy.h"
#include "format/file_format_timestamp_type_size_raw_message.h"
#include <QDataStream>
#include <iostream>

const int LogFile::DEFAULT_FILE_FORMAT_VERSION;

LogFile::LogFile(const QString& filename, bool compressed, int formatVersion) :
    m_filename(filename),
    m_compressed(compressed),
    m_formatVersion(formatVersion),
    m_io(NULL),
    m_file(NULL),
    m_compressor(NULL)
{
    addFormat(new FileFormatLegacy());
    addFormat(new FileFormatTimestampTypeSizeRawMessage());
}

LogFile::~LogFile()
{
    close();
    qDeleteAll(m_formatMap);
}

bool LogFile::addFormat(FileFormat* format)
{
    if (m_formatMap.contains(format->version())) {
        std::cout << "Error adding log format!" << std::endl;
        std::cout << "Format version " << format->version() << " has been used twice." << std::endl;

        return false;
    }

    m_formatMap[format->version()] = format;

    return true;
}

bool LogFile::openRead()
{
    foreach (FileFormat* format, m_formatMap) {
        close();

        Q_ASSERT(m_file == NULL);
        m_file = new QFile(m_filename);

        if (!m_file->open(QIODevice::ReadOnly)) {
            std::cout << "Error opening log file \"" << m_filename.toStdString() << "\"!" << std::endl;
            close();

            return false;
        }

        if (m_compressed) {
            Q_ASSERT(m_compressor == NULL);
            m_compressor = new QtIOCompressor(m_file);
            m_compressor->setStreamFormat(QtIOCompressor::GzipFormat);
            m_compressor->open(QIODevice::ReadOnly);

            m_io = m_compressor;
        } else {
            m_io = m_file;
        }

        QDataStream stream(m_io);

        if (format->readHeaderFromStream(stream)) {
            std::cout << "Detected log file format version " << format->version() << "." << std::endl;
            m_formatVersion = format->version();

            return true;
        }
    }

    std::cout << "Error log file corrupted or format is not supported!" << std::endl;
    close();

    return false;
}

bool LogFile::openWrite()
{
    close();

    FileFormat* format = m_formatMap.value(m_formatVersion, NULL);

    if (format == NULL) {
        std::cout << "Error log file format is not supported!" << std::endl;

        return false;
    }

    Q_ASSERT(m_file == NULL);
    m_file = new QFile(m_filename);

    if (!m_file->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        std::cout << "Error opening log file \"" << m_filename.toStdString() << "\"!" << std::endl;

        return false;
    }

    if (m_compressed) {
        Q_ASSERT(m_compressor == NULL);
        m_compressor = new QtIOCompressor(m_file);
        m_compressor->setStreamFormat(QtIOCompressor::GzipFormat);
        m_compressor->open(QIODevice::WriteOnly);

        m_io = m_compressor;
    } else {
        m_io = m_file;
    }

    QDataStream stream(m_io);
    format->writeHeaderToStream(stream);

    return true;
}

void LogFile::close()
{
    m_io = NULL;

    delete m_compressor;
    m_compressor = NULL;

    delete m_file;
    m_file = NULL;
}

void LogFile::saveMessage(const QByteArray& data, qint64 time, MessageType type)
{
    if (m_io == NULL || !m_io->isWritable()) {
        return;
    }

    FileFormat* format = m_formatMap.value(m_formatVersion, NULL);

    if (format == NULL) {
        std::cout << "Error log file format is not supported!" << std::endl;

        return;
    }

    QDataStream stream(m_io);
    stream.setVersion(QDataStream::Qt_4_6);

    format->writeMessageToStream(stream, data, time, type);
}

bool LogFile::readMessage(QByteArray& data, qint64& time, MessageType& type)
{
    if (m_io == NULL || !m_io->isReadable()) {
        return false;
    }

    FileFormat* format = m_formatMap.value(m_formatVersion, NULL);

    if (format == NULL) {
        std::cout << "Error log file format is not supported!" << std::endl;

        return false;
    }

    QDataStream stream(m_io);
    stream.setVersion(QDataStream::Qt_4_6);

    if (stream.atEnd()) {
        return false;
    }

    return format->readMessageFromStream(stream, data, time, type);
}
