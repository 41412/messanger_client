#ifndef MCPACKET_H
#define MCPACKET_H

#include <QObject>
#include <QtNetwork>
#include <mcdebug.h>

#define SEPARATOR 1

static unsigned const char PACKET_HEAD[8] = {0xFF, 0xEE, 'M', 'R', '2', '0', '2', '0'};

class McPacket : public QObject
{

public:
    bool isReceiving;

    explicit McPacket();
    virtual ~McPacket();

    void initPacketBuffer();
    void removeHeader(QByteArray &packet);
    void extractReadPacket(QByteArray &packet);
    void extractReadPacketTotalSize(QByteArray &packet);
    int getTotalSize();
    void setTotalSize(int size);
    int getRemainSize();
    void setRemainSize(int size);
    void extractReadPacketProtocol(QByteArray &packet);
    QString getProtocol();
    void extractReadPacketData(QByteArray &packet);
    QString getData();
    void setData(QString data);
    bool isReadCompleted();

    static void writePacket(QTcpSocket *socket, QString protocol)
    {
        if(!isConnected(socket))
        {
            return;
        }
        QByteArray packet = QByteArray(reinterpret_cast<const char*>(PACKET_HEAD));
        packet += intToArray(protocol.size()) + protocol;
        socket->write(packet);
    }
    static void writePacket(QTcpSocket *socket, QString protocol, QString data)
    {
        if(!isConnected(socket))
        {
            return;
        }
        QByteArray packet = QByteArray(reinterpret_cast<const char*>(PACKET_HEAD));
        packet += intToArray(protocol.size() + data.size() + SEPARATOR) + protocol + " " + data;
        socket->write(packet);
    }

    static bool isValidSizePacket(QByteArray packet)
    {
        return packet.size() > 0;
    }
    static bool isNewPacket(QByteArray header)
    {
        return memcmp(header.constData(), PACKET_HEAD, 8) == 0;
    }

    static bool isConnected(QTcpSocket *socket)
    {
        return (socket->state() == QAbstractSocket::ConnectedState);
    }
    static QByteArray intToArray(qint32 source)
    {
        // Avoid use of cast, this is the Qt way to serialize objects
        QByteArray temp;
        QDataStream data(&temp, QIODevice::ReadWrite);
        data << source;
        return temp;
    }

private:
    McDebug *debugger;
    int totalsize;
    int remainsize;
    QString protocol;
    QString data;
};

#endif // MCPACKET_H
