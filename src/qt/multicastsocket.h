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

#ifndef MULTICASTSOCKET_H
#define MULTICASTSOCKET_H

#include <QNetworkInterface>
#include <QUdpSocket>

class MulticastSocket : public QUdpSocket
{
public:
    explicit MulticastSocket(QObject *parent = 0);

public:
    bool joinMulticastGroup(const QHostAddress &groupAddress);
    bool joinMulticastGroup(const QHostAddress &groupAddress, const QNetworkInterface &iface);
};

#endif // MULTICASTSOCKET_H
