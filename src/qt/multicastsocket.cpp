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

#include "multicastsocket.h"
#include <cerrno>

#ifdef Q_OS_WIN
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
#endif

/*!
 * \class MulticastSocket
 * \brief Extension of QUdpSocket to provide UDP multicast features
 *
 * This class implements multicast features from Qt 4.7. As soon as
 * the dependency is raised to 4.7 this class can be removed.
 */

MulticastSocket::MulticastSocket(QObject *parent) :
    QUdpSocket(parent)
{
}

bool MulticastSocket::joinMulticastGroup(const QHostAddress &groupAddress)
{
    return joinMulticastGroup(groupAddress, QNetworkInterface());
}

bool MulticastSocket::joinMulticastGroup(const QHostAddress& groupAddress, const QNetworkInterface& iface)
{
    bool success = false;

    foreach (const QNetworkAddressEntry& addressEntry, iface.addressEntries()) {
        const QHostAddress interfaceAddress = addressEntry.ip();

        if (interfaceAddress.protocol() == IPv4Protocol) {
            ip_mreq mreq;
            mreq.imr_multiaddr.s_addr = htonl(groupAddress.toIPv4Address());
            mreq.imr_interface.s_addr = htonl(interfaceAddress.toIPv4Address());

            const bool error = setsockopt(socketDescriptor(), IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*) &mreq, sizeof(mreq)) == -1;
            if (error)
                setErrorString(strerror(errno));
            else
                success = true;
        }
    }

    return success;
}
