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

#include "common/logfile.h"
#include "protobuf/ssl_referee.pb.h"
#include "protobuf/messages_robocup_ssl_wrapper_legacy.pb.h"
#include <QCoreApplication>
#include <iostream>
#include <boost/program_options.hpp>
#include <QFileInfo>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    app.setApplicationName("logconvert");
    app.setOrganizationName("ssl-logtools");

    std::string fileIn;
    std::string fileOut;
    int formatVersion;

    boost::program_options::options_description desc("Available options");
    desc.add_options()
        ("help,h", "Print help message.")
        ("compress,c", "Use gzip compression.")
        ("format,f", boost::program_options::value(&formatVersion)->default_value(LogFile::DEFAULT_FILE_FORMAT_VERSION), "File format version.")
        ("input,i", boost::program_options::value(&fileIn)->default_value(""), "Input file.")
        ("output,o", boost::program_options::value(&fileOut)->default_value(""), "Output file.")
    ;

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }

    if (fileIn.size() < 1) {
        std::cout << "No input file specified!" << std::endl;
        std::cout << desc << std::endl;
        return -1;
    }

    if (fileOut.size() < 1) {
        std::cout << "No output file specified!" << std::endl;
        std::cout << desc << std::endl;
        return -1;
    }

    QString filenameIn = QString::fromStdString(fileIn);
    QString filenameOut = QString::fromStdString(fileOut);

    if (filenameIn == filenameOut) {
        std::cout << "Output file must be different from input file!" << std::endl;
        std::cout << desc << std::endl;
        return -1;
    }

    QFileInfo fileInfo(filenameIn);
    bool compressed = false;
    if (fileInfo.suffix() == "gz") {
        compressed = true;
    }

    LogFile in(filenameIn, compressed);
    LogFile out(filenameOut, vm.count("compressed"), formatVersion);

    if (!in.openRead() || !out.openWrite()) {
        return -1;
    }

    QByteArray data;
    qint64 time;
    MessageType type;

    RoboCup2014Legacy::Wrapper::SSL_WrapperPacket visionPacket;
    SSL_Referee refereePacket;

    while (in.readMessage(data, time, type)) {
        // OK, let's try to figure this out by parsing the message
        if (type == MESSAGE_UNKNOWN) {
            if (refereePacket.ParseFromArray(data.data(), data.size())) {
                type = MESSAGE_SSL_REFBOX_2013;
            } else if (visionPacket.ParseFromArray(data.data(), data.size())) {
                type = MESSAGE_SSL_VISION_2010;
            }
        }

        out.saveMessage(data, time, type);
    }

    return 0;
}
