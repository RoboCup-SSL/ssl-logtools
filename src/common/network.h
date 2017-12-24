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

#ifndef NETWORK_H
#define NETWORK_H

#include <QUdpSocket>

class Network : public QObject
{
    Q_OBJECT

public:
    Network(const QHostAddress &groupAddress, quint16 localPort, quint16 targetPort);
    ~Network();

signals:
    void gotPacket(QByteArray data, qint64 time);

public:
    void writeData(const QByteArray& data);

public slots:
    void connect();
    void disconnect();

private slots:
    void readData();

private:
    QHostAddress m_groupAddress;
    quint16 m_localPort;
    quint16 m_targetPort;
    QUdpSocket *m_socket;
};

#endif // NETWORK_H
