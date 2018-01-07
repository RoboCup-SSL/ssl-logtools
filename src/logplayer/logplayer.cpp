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

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("logplayer");
    app.setOrganizationName("ssl-logtools");

    MainWindow window;
    window.show();

    if(argc > 1)
    {
        QString filename(argv[1]);
        window.loadFile(&filename);
        window.setStopped(false);
    }

    return app.exec();
}
