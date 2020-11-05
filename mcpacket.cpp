#include "mcpacket.h"

McPacket::McPacket()
{

}

McPacket::~McPacket()
{

}

void McPacket::extractReadPacket(QByteArray &packet)
{
    extractReadPacketSize(packet);
    extractReadPacketProtocol(packet);
    extractReadPacketData(packet);
}

void McPacket::extractReadPacketSize(QByteArray &packet)
{
    bool ok;
    int size = packet.mid(0, sizeof(int)).toHex().toInt(&ok, 16);
    packet.remove(0, sizeof(int));
    debugger->debugMessage("size", size);
}

void McPacket::extractReadPacketProtocol(QByteArray &packet)
{
    if(packet.indexOf(' ') == -1)
    {
        protocol = packet;
        packet = "";
    }
    else
    {
        protocol = packet.left(packet.indexOf(' '));
        packet.remove(0, packet.indexOf(' ') + SEPARATOR);
    }
    debugger->debugMessage("protocol", protocol);
}
QString McPacket::getProtocol()
{
    return protocol;
}

void McPacket::extractReadPacketData(QByteArray &packet)
{
    data = QString::fromLocal8Bit(packet.data(), packet.size()).toUtf8();
    debugger->debugMessage("data", data);
}
QString McPacket::getData()
{
    return data;
}
void McPacket::setData(QString data)
{
    this->data = data;
}
