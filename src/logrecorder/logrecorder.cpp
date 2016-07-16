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

#include "common/recorder.h"
#include <signal.h>
#include <QCoreApplication>
#include <QStringList>
#include <iostream>
#include <boost/program_options.hpp>

void signalHandler(int)
{
    QCoreApplication::quit();
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    app.setApplicationName("logrecorder");
    app.setOrganizationName("ssl-logtools");

    signal(SIGABRT, &signalHandler);
    signal(SIGTERM, &signalHandler);
    signal(SIGINT, &signalHandler);

    std::string filename;
    int formatVersion;

    boost::program_options::options_description desc("Available options");
    desc.add_options()
        ("help,h", "Print help message.")
        ("compress,c", "Use gzip compression.")
        ("format,f", boost::program_options::value(&formatVersion)->default_value(LogFile::DEFAULT_FILE_FORMAT_VERSION), "File format version.")
        ("output,o", boost::program_options::value(&filename)->default_value(""), "Output file.")
    ;

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }

    if (filename.size() < 1) {
        std::cout << "No output file specified!" << std::endl;
        std::cout << desc << std::endl;
        return -1;
    }

    Recorder recorder;
    if (!recorder.start(QString::fromStdString(filename), vm.count("compress"), formatVersion)) {
        return -1;
    }

    return app.exec();
}
