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
#include <QString>
#include <QByteArray>
#include <zlib.h>
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


// zlib圧縮関数
QByteArray zlibCompress(const QByteArray& data)
{
    uLongf destLen = compressBound(data.size());
    QByteArray compressed;
    compressed.resize(destLen);
    int res = compress(reinterpret_cast<Bytef*>(compressed.data()), &destLen,
                      reinterpret_cast<const Bytef*>(data.constData()), data.size());
    if (res != Z_OK) {
        return QByteArray();
    }
    compressed.resize(destLen);
    return compressed;
}

void FileFormatLegacy::writeMessageToStream(QDataStream& stream, const QByteArray& data, qint64 time, MessageType type)
{
    stream << time;
    stream << zlibCompress(data);
}


// zlib解凍関数
QByteArray zlibUncompress(const QByteArray& compressed)
{
    // 仮の最大サイズ（必要に応じて調整）
    uLongf destLen = compressed.size() * 10;
    QByteArray uncompressed;
    uncompressed.resize(destLen);
    int res = uncompress(reinterpret_cast<Bytef*>(uncompressed.data()), &destLen,
                        reinterpret_cast<const Bytef*>(compressed.constData()), compressed.size());
    if (res != Z_OK) {
        return QByteArray();
    }
    uncompressed.resize(destLen);
    return uncompressed;
}

bool FileFormatLegacy::readMessageFromStream(QDataStream& stream, QByteArray& data, qint64& time, MessageType& type)
{
    type = MESSAGE_UNKNOWN;

    stream >> time;
    QByteArray compressedPacket;
    stream >> compressedPacket;
    data = zlibUncompress(compressedPacket);

    return true;
}
