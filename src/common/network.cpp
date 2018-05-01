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

#include "qt/multicastsocket.h"
#include "network.h"
#include "timer.h"
#include <QtGlobal>

/*!
 * \class Network
 * \brief UDP multicast receiver and transmitter
 *
 * This class is designed to timestamp an incoming packet as early as possible
 * by being moved to a dedicated worker thread.
 */

/*!
 * \fn void Network::gotPacket(QByteArray data, qint64 time)
 * \brief This signal is emitted whenever a new packet has been received
 * \param data  The received packet
 * \param time  Timestamp at which the packet has been received
 */

/*!
 * \brief Constructor
 * \param groupAddress Address of the multicast group to listen on
 * \param port Port to listen on
 */
Network::Network(const QHostAddress &groupAddress, quint16 localPort, quint16 targetPort) :
    m_groupAddress(groupAddress),
    m_localPort(localPort),
    m_targetPort(targetPort),
    m_socket(NULL)
{
}

/*!
 * \brief Destructor
 */
Network::~Network()
{
    disconnect();
}

/*!
 * \brief Start listening on the socket
 */
void Network::connect()
{
    disconnect();

    m_socket = new MulticastSocket(this);
    QObject::connect(m_socket, SIGNAL(readyRead()), SLOT(readData()));
#if QT_VERSION >= 0x050000
    m_socket->bind(QHostAddress::AnyIPv4, m_localPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
#else
    m_socket->bind(QHostAddress::Any, m_localPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
#endif
    m_socket->joinMulticastGroup(m_groupAddress);

    if (m_socket->state() != QAbstractSocket::BoundState) {
        foreach (const QNetworkInterface& iface, QNetworkInterface::allInterfaces()) {
            m_socket->joinMulticastGroup(m_groupAddress, iface);
        }
    }

    m_socket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, QVariant(1));
}

/*!
 * \brief Stop listening on the socket
 */
void Network::disconnect()
{
    delete m_socket;
    m_socket = NULL;
}

/*!
 * \brief Read a packet from the socket and emit \ref gotPacket
 */
void Network::readData()
{
    QByteArray data;
    data.resize(m_socket->pendingDatagramSize());
    m_socket->readDatagram(data.data(), data.size());
    emit gotPacket(data, Timer::systemTime());
}

/*!
 * \brief Write a packet to the socket
 */
void Network::writeData(const QByteArray& data)
{
    m_socket->writeDatagram(data, m_groupAddress, m_targetPort);
}
